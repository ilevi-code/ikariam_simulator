#include <algorithm>

#include "formation.hpp"

const std::vector<std::string> Formation::FORMATION_NAMES = {
    "air",
    "bomber",
    "artillery",
    "range",
    "front",
    "flank",
};

static const std::vector<Formation::Type> ATTACK_ORDER[] = {
    {Formation::bomber, Formation::air_defense},
    {Formation::artillery, Formation::long_range, Formation::front, Formation::flank},
    {Formation::front, Formation::flank},
    {Formation::front, Formation::flank, Formation::long_range},
    {Formation::front, Formation::long_range, Formation::artillery, Formation::flank},
    {Formation::flank, Formation::long_range, Formation::artillery, Formation::front},
};

static bool compare_slot_count(const Slot& a, const Slot& b);

Formation::Formation(Type formationType, std::size_t max_slot_count, int slot_size,
    const AcceptableUnits& acceptable_units)
    : _max_slot_count(max_slot_count), _slot_size(slot_size), _type(formationType), _acceptable_units(acceptable_units)
{
}

Formation::Formation(const Formation& other)
    : _slots(other._slots), _max_slot_count(other._max_slot_count), _slot_size(other._slot_size), _type(other._type),
      _acceptable_units(other._acceptable_units)
{
}

const Formation::AcceptableUnits& Formation::getAcceptableUnits() const
{
    return _acceptable_units;
}

const std::vector<Formation::Type>& Formation::get_attack_order() const
{
    return ATTACK_ORDER[_type];
}

std::size_t Formation::get_biggest_slot_size() const
{
    auto found = std::max_element(_slots.begin(), _slots.end(), compare_slot_count);
    if (found == _slots.end()) {
        return 0;
    }
    return found->count;
}

static bool compare_slot_count(const Slot& a, const Slot& b)
{
    return a.count < b.count;
}

std::size_t Formation::size() const
{
    return _slots.size();
}

int Formation::get_units_count() const
{
    int count = 0;
    for (const auto& slot : _slots) {
        count += slot.count;
    }
    return count;
}

int Formation::get_losses_count() const
{
    int count = 0;
    for (const auto& slot : _slots) {
        count += slot.orig_count - slot.count;
    }
    return count;
}

std::list<std::tuple<Unit, int>> Formation::get_first_healths() const
{
    std::list<std::tuple<Unit, int>> healths;
    for (const auto& slot : _slots) {
        healths.push_back(std::make_tuple(slot.meta->type, slot.first_health));
    }
    return healths;
}

int Formation::get_next_occupied_index(int current)
{
    if (is_empty()) {
        throw std::runtime_error("empty formation");
    }

    int hit_slot_index = current;
    int size = _slots.size();
    do {
        hit_slot_index = (hit_slot_index + 1) % size;
    } while (_slots[hit_slot_index].count == 0);
    return hit_slot_index;
}

Slot& Formation::operator[](std::size_t index)
{
    return _slots[index];
}

const Slot& Formation::operator[](std::size_t index) const
{
    return _slots[index];
}

bool Formation::operator==(const Formation& other) const
{
    return _slots == other._slots;
}

bool Formation::is_empty() const
{
    bool is_empty = true;
    for (const auto& slot : _slots) {
        is_empty &= slot.count < 1;
    }
    return is_empty;
}

bool Formation::is_full() const
{
    return _slots.size() == _max_slot_count;
}

void Formation::drain_into(Army& army)
{
    for (auto& slot : _slots) {
        army.return_squad(slot.meta->type, slot.count, slot.first_health);
    }
    _slots.clear();
}

void Formation::fill(Army& army)
{
    for (auto unit_type : getAcceptableUnits()) {
        fill(army, unit_type);
    }
}

void Formation::fill(Army& army, Unit unit_type)
{
    while (!is_full()) {
        auto squad = army.borrow_squad(unit_type, _slot_size);
        if (!squad.has_value()) {
            return;
        }
        auto [slot_allowance, first_health, meta] = squad.value();
        int& ammo_pool = army.get_ammo_pool(unit_type);

        fill_slot(meta, slot_allowance, first_health, ammo_pool);
    }
}

void Formation::fill_slot(const UnitMeta* meta, int count, int first_health, int& ammo_pool)
{
    _slots.push_back(Slot{meta, count, count, first_health, ammo_pool});
}

std::span<const Slot> Formation::get_slots() const
{
    return {_slots.data(), _slots.size()};
}
