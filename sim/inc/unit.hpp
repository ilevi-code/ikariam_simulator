#pragma once

#include <map>

enum Unit
{
    hoplite,
    steam_giant,
    spearmen,
    swordsman,
    slinger,
    archer,
    carbineer,
    ram,
    catapult,
    mortar,
    gyrocopter,
    bombardier,
    cook,
    doctor,
    barbarian,
    ram_ship,
    fire_ship,
    paddle_speedboat,
    ballista_ship,
    catapult_ship,
    mortar_ship,
    diving_boat,
    steam_ram,
    rocket_ship,
    balloon_carrier,
    tender,
    wall,
    type_count,
};

struct UnitMeta {
    Unit type;
    int health;
    int attack;
    int ranged_attack;
    int ammo;
    int armor;
    int size;
    bool isHuman;
};

extern const std::map<std::string, Unit> UNIT_NAMES;

extern const UnitMeta UNITS_META[];