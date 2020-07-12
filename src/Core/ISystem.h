#pragma once

namespace trc
{
    class ISystem
    {
    public:
        virtual ~ISystem() = default;

        virtual void update() = 0;
        virtual void lateUpdate() = 0;
    };
}