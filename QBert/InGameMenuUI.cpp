#include "pch.h"
#include "InGameMenuUI.h"
#include <GameContext.h>
#include <ServiceLocator.h>
#include "QbertGameController.h"

#include <SDL.h>
#include <imgui.h>
#include <backends\imgui_impl_sdl.h>
#include <backends\imgui_impl_opengl2.h>

void InGameMenuUI::Render(const glm::vec3& position)
{
	auto pWindow = GameContext::GetInstance().GetWindow();
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(pWindow);
	ImGui::NewFrame();
	int x, y;
	SDL_GetWindowSize(pWindow, &x, &y);

	ImGui::SetWindowSize({ 120, 100 });
	ImGui::SetWindowPos({ float(x / 2 - 60), float(y / 2 + 50) });
	if (ImGui::Button("Resume"))
	{
		QbertGameController& qgc = (QbertGameController&)ServiceLocator::GetGameController();
		qgc.SetQbertState(QbertGameState::LEVEL);
	}
	if (ImGui::Button("To Main Menu"))
	{
		QbertGameController& qgc = (QbertGameController&)ServiceLocator::GetGameController();
		qgc.SetQbertState(QbertGameState::MAINMENU);
	}
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}
