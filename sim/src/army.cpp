#include "army.hpp"

Army::Army() : _ammo_pools{0}
{
}

void Army::reinforce(Unit unit, int count)
{
    _units[unit] = count;
    _ammo_pools[unit] = UNITS_META[unit].ammo * count;
}

void Army::reinforce(Unit unit, int count, int ammo)
{
    _units[unit] = count;
    _ammo_pools[unit] = ammo;
}

int Army::get_squad(Unit unit, int slot_size)
{
    int size = UNITS_META[unit].size;
    int count = std::min(_units[unit], slot_size / size);
    if (is_ranged(unit)) {
        count = std::min(count, _ammo_pools[unit]);
    }

    _units[unit] -= count;
    return count;
}

Army::json Army::get_units_json()
{
    return json(_units);
}

Army::json Army::get_ammo_json()
{
    json serialized;
    for (std::size_t i = 0; i < _ammo_pools.size(); i++) {
        serialized[std::to_string(i)] = _ammo_pools[i];
    }
    return serialized;
}