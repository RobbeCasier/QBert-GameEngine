#pragma once

namespace dae
{
    class GameController
    {
    public:
        GameController() = default;
        virtual ~GameController() = default;

        virtual void Initialize() = 0;
        virtual void Update() = 0;
    };
}

