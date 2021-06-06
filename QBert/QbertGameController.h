#pragma once
#include <GameController.h>
#include "MainMenuLevel.h"
#include "MainLevel.h"

enum class QbertGameState
{
    MAINMENU,
    LEVEL,
    PAUSE,
    END
};
enum class GameMode
{
    SOLO,
    COOP,
    VS
};
class QbertGameController :
    public dae::GameController
{
public:
    QbertGameController() = default;
    virtual ~QbertGameController() = default;
    virtual void Initialize() override;
    virtual void Update() override;

    void SetQbertState(const QbertGameState& state) { m_State = state; m_StateUpdated = true; }
    QbertGameState GetQbertState() { return m_State; }
    void SetQbertGameMode(const GameMode& type) { m_Mode = type; }
    GameMode GetQbertGameMode() { return m_Mode; }
private:
    std::shared_ptr<MainMenuLevel> m_pMainMenu;
    std::shared_ptr<MainLevel> m_pGame;
    QbertGameState m_State = QbertGameState::MAINMENU;
    GameMode m_Mode;
    bool m_StateUpdated = true;
};

