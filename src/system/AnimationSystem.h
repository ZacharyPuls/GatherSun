//
// Created by zach on 2023-05-07.
//

#ifndef GATHERSUN_SYSTEM_ANIMATIONSYSTEM_H
#define GATHERSUN_SYSTEM_ANIMATIONSYSTEM_H

#include "System.h"

namespace gathersun::system {

    class AnimationSystem : System {
    public:
        void Run(double dt) override;
    };
}

#endif // GATHERSUN_SYSTEM_ANIMATIONSYSTEM_H
