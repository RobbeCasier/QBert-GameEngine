#include "pch.h"
#include "MainMenuSDLButtons.h"
#include <GameContext.h>
#include <SceneManager.h>
#include <ServiceLocator.h>
#include "QbertGameController.h"

#include <SDL.h>
#include <imgui.h>
#include <backends\imgui_impl_sdl.h>
#include <backends\imgui_impl_opengl2.h>

void MainMenuSDLButtons::Render(const glm::vec3& position)
{
	UNREFERENCED_PARAMETER(position);
	auto pWindow = GameContext::GetInstance().GetWindow();
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(pWindow);
	ImGui::NewFrame();
	int x, y;
	SDL_GetWindowSize(pWindow, &x, &y);

	ImGui::SetWindowSize({ 120, 100 });
	ImGui::SetWindowPos({ float(x / 2 - 60), float(y / 2 + 50) });
	if (ImGui::Button("Single Player"))
	{
		QbertGameController& qgc = (QbertGameController&)ServiceLocator::GetGameController();
		qgc.SetQbertState(QbertGameState::LEVEL);
		qgc.SetQbertGameMode(GameMode::SOLO);
	}
	if (ImGui::Button("Co-Op"))
	{
		QbertGameController& qgc = (QbertGameController&)ServiceLocator::GetGameController();
		qgc.SetQbertState(QbertGameState::LEVEL);
		qgc.SetQbertGameMode(GameMode::COOP);
	}
	if (ImGui::Button("Versus"))
	{
		QbertGameController& qgc = (QbertGameController&)ServiceLocator::GetGameController();
		qgc.SetQbertState(QbertGameState::LEVEL);
		qgc.SetQbertGameMode(GameMode::VS);
	}
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}
