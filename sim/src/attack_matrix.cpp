#include "attack_matrix.hpp"

AttackMatrix::AttackMatrix(Formation& formation)
    : _formation(formation), _row(0), _row_count(_formation.get_biggest_slot_size())
{
}

void AttackMatrix::advance()
{
    _row++;
}

bool AttackMatrix::is_done()
{
    return _row >= _row_count;
}

MeleeAttackMatrix::MeleeAttackMatrix(Formation& formation) : AttackMatrix(formation) {}

AttackInfo MeleeAttackMatrix::calc_row_damage()
{
    AttackInfo info = {0, 0};
    for (std::size_t i = 0; i < _formation.size(); i++) {
        Slot& attacking = _formation[i];
        if (attacking.count > static_cast<int>(_row)) {
            info.damage += attacking.meta->attack;
            info.unit_count++;
        }
    }
    return info;
}

NativeAttackMatrix::NativeAttackMatrix(Formation& formation) : AttackMatrix(formation) {}

AttackInfo NativeAttackMatrix::calc_row_damage()
{
    AttackInfo info = {0, 0};
    for (std::size_t i = 0; i < _formation.size(); i++) {
        Slot& attacking = _formation[i];
        if (attacking.count > static_cast<int>(_row)) {
            if (attacking.meta->is_ranged()) {
                info.damage += attacking.meta->ranged_attack;
                attacking.ammo_pool--;
            } else {
                info.damage += attacking.meta->attack;
            }
            info.unit_count++;
        }
    }
    return info;
}
