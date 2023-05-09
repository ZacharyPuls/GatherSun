//
// Created by zach on 2023-04-27.
//

#ifndef GATHERSUN_GAME_GAMESTATE_H
#define GATHERSUN_GAME_GAMESTATE_H

namespace gathersun::game {

    enum class GameState {
        RUNNING,
        PAUSED,
        STOPPING,
        STOPPED
    };
}

#endif // GATHERSUN_GAME_GAMESTATE_H
