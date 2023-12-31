#pragma once

#include <ctime>
#include <list>
#include <optional>
#include <memory>

#include "blobfs/blobfs.hpp"
#include "mission.hpp"
#include "sql/connection.hpp"
#include "unit.hpp"
#include "battle_meta.hpp"
#include "army.hpp"

class Database
{
public:
    Database();

    std::list<std::tuple<Unit, int>> load_defending_units(const Mission& mission);
    std::list<std::tuple<Unit, int>> load_attacking_units(const Mission& mission);

    std::string getTownsUsername(int town_id);
    int get_town_player_id(int town_id);

    BuildingLevels get_buildings(int town_id);
    std::unique_ptr<Army::Improvements> get_army_improvements(int town_id);

    std::list<Mission> get_missions_needing_update(Mission::State state);

    Mission load_mission(int mission_id);

    void store_round_ui(const Mission& mission, const std::string& round);
    void store_round_data(const Mission& mission, const std::string& round);

    void update_arrived(const Mission& mission);

    std::optional<std::string> get_last_round(const Mission& mission);

private:
    void store_round(const Mission& mission, const std::string& round, std::string round_table_name);

    void _create_battle(const Mission& mission);

    sql::Connection _conn;
    BlobFS _blobs;
};
