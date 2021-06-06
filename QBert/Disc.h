#pragma once
#include <BaseComponent.h>
#include "BlockComponent.h"

class Disc :
	public BaseComponent
{
public:
	explicit Disc() = default;
	virtual ~Disc();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(const glm::vec3& position) override;

	void SetTexture(SideColor color, int width = 0, int height = 0);
	void SetTextureDestination(glm::vec3 dst);
private:
	SDL_Rect m_TextureRect;
	std::shared_ptr<TextureComponent> m_TextureComponent = nullptr;

	float m_TimeElapsed = 0.f;
	const float m_TimeNextCol = 0.25;
	int m_CurrentCol = 0;
	float m_SizeX = 0;
	float m_SizeY = 0;
	int m_MaxColsAnimation = 4;

	bool m_Move = false;

	glm::vec2 m_Destination;
	glm::vec2 m_Dir;
};

