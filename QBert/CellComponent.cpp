#include "pch.h"
#include "CellComponent.h"
#include <GameObject.h>

void CellComponent::SetCube(SideColor color, std::vector<int> colorOrder, bool isSide)
{
	auto block = m_GameObject->AddComponent<BlockComponent>();
	m_isCube = true;
	block->SetIsSide(isSide);
	block->SetTexture(color, (int)m_Width, (int)m_Height);
	block->SetColorOrder(colorOrder);
	glm::vec3 dst = m_TransformComponent->GetPosition();
	dst.y -= m_Height/2;
	dst.x -= m_Width / 2;
	block->SetTextureDestination(dst);

	m_Component = block;
}

void CellComponent::SetDisc(SideColor color)
{
	auto disc = m_GameObject->AddComponent<Disc>();
	m_isDisc = true;
	disc->SetTexture(color, (int)m_Width, (int)m_Height);
	glm::vec3 dst = m_TransformComponent->GetPosition();
	disc->SetTextureDestination(dst);

	m_Component = disc;
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
	pos.x -= m_Height/4.f;

	return pos;
}

void CellComponent::Initialize()
{
}

void CellComponent::Render(const glm::vec3& position)
{
	if (m_Component)
		m_Component->Render(m_TransformComponent->GetPosition());
}

bool CellComponent::IsCube() const
{
	return m_isCube;
}

bool CellComponent::IsDisc() const
{
	return m_isDisc;
}

ColorState CellComponent::ChangeColor(bool reset)
{
	if (m_Component == nullptr)
		return ColorState::START;
	return std::dynamic_pointer_cast<BlockComponent>(m_Component)->ChangeColor(reset);
}

void CellComponent::RemoveDisc()
{
	m_isDisc = false;
	m_GameObject->RemoveComponent(std::dynamic_pointer_cast<Disc>(m_Component));
	m_Component.reset();
}
