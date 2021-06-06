#pragma once
#include <GameController.h>
#include "MainMenuLevel.h"
#include "MainLevel.h"
#include "ScoreLevel.h"

enum class QbertGameState
{
    MAINMENU,
    LEVEL,
    PAUSE,
    GAMEOVER,
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

    void SetScore(const int& scores, const int& player) { m_Scores[player] = scores; }
    const std::vector<int>& GetScores() { return m_Scores; }
private:
    std::shared_ptr<MainMenuLevel> m_pMainMenu;
    std::shared_ptr<MainLevel> m_pGame;
    std::shared_ptr<ScoreLevel> m_pEnd;

    QbertGameState m_State = QbertGameState::MAINMENU;
    QbertGameState m_PreviousState = QbertGameState::MAINMENU;
    GameMode m_Mode;
    bool m_StateUpdated = true;

    std::vector<int> m_Scores = { 0,0 };
};

