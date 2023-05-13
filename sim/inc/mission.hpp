#pragma once

#include <optional>
#include <ctime>
#include <chrono>

using namespace std::chrono;

struct Mission {
    enum State {
        LOADING,
        EN_ROUTE,
        IN_BATTLE,
        RETURNING,
        DISPERSED,
    };

    enum Type {
        TRANSPORT,
        STATION,
        STATION_FLEET,
        DEFEND,
        DEFEND_PORT,
        PLUNDER,
        OCCUPY_TOWN,
        OCCUPY_PORT,
        CAPTURING_PORT,
    };

    int id;
    int from;
    int to;
    State state;
    Type type;
    std::time_t next_stage_time;
    std::optional<int> battle_id;

    static constexpr int STAGE_INTERVAL = duration_cast<seconds>(15min).count();
};
