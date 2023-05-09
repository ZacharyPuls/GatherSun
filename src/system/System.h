//
// Created by zach on 2023-04-29.
//

#ifndef GATHERSUN_SYSTEM_H
#define GATHERSUN_SYSTEM_H

#include "Core.h"

namespace gathersun::system {

    class System {
    public:
        explicit System(entt::registry &registry) : registry_(registry) {}

        virtual ~System() = default;

        virtual void Run(double dt) = 0;

    protected:
        entt::registry &registry_;
    };
}

#endif // GATHERSUN_SYSTEM_H
