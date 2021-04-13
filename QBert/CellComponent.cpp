#include "pch.h"
#include "CellComponent.h"
#include <GameObject.h>

void CellComponent::SetCube(std::shared_ptr<PlayerComponent> player, std::shared_ptr<Grid> grid, SideColor color, std::vector<int> colorOrder, bool isSide)
{
	m_BlockComponent = m_GameObject->AddComponent<BlockComponent>();
	m_isCube = true;
	m_BlockComponent->SetIsSide(isSide);
	m_BlockComponent->SetTexture(color, (int)m_Width, (int)m_Height);
	m_BlockComponent->SetColorOrder(colorOrder);
	glm::vec3 dst = m_TransformComponent->GetPosition();
	dst.y -= m_Height/4;
	dst.x -= m_Width / 2;
	m_BlockComponent->SetTextureDestination(dst);

	m_BlockComponent->AddPlayerCommand(player);
	m_BlockComponent->AddGridCommand(grid);
}

void CellComponent::SetPosition(const float& x, const float& y)
{
	if (m_TransformComponent == nullptr)
		m_TransformComponent = std::make_shared<Transform>();

	m_TransformComponent->SetPosition(x, y, 0.0f);
}

void CellComponent::SetSize(const float& size)
{
	m_Width = size;
	m_Height = size;
}

glm::vec2 CellComponent::GetSpritePos() const
{
	//sprites are halve the cubes

	glm::vec2 pos;
	pos.x = m_TransformComponent->GetPosition().x;
	pos.y = m_TransformComponent->GetPosition().y;

	//set to the position of the top left/right corner
	pos.y -= m_Height / 2.f;
	pos.x -= m_Height / 4.f;

	return pos;
}

void CellComponent::Initialize()
{
}

bool CellComponent::IsCube() const
{
	return m_isCube;
}

void CellComponent::ChangeColor()
{
	if (m_BlockComponent == nullptr)
		return;
	m_BlockComponent->ChangeColor();
}
