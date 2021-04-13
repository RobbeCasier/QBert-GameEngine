#pragma once
#include "Minigin.h"

class MainGame :
    public dae::Minigin
{
protected:
    virtual void LoadGame() const override;
};

