#include "pch.h"
#include "BlockComponent.h"
#include <RenderComponent.h>
#include <GameObject.h>
#include "PlayerController.h"
#include "GridController.h"

void BlockComponent::Initialize()
{
	m_TextureComponent = m_GameObject->AddComponent<TextureComponent>();
}

void BlockComponent::AddPlayerCommand(std::shared_ptr<PlayerComponent> player)
{
	m_BlockChangePlayer = std::make_unique<ChangeColorPlayer>(player);
}

void BlockComponent::AddGridCommand(std::shared_ptr<Grid> grid)
{
	m_BlockChangeGrid = std::make_unique<ChangeColorGrid>(grid);
}

void BlockComponent::SetIsSide(bool isSide)
{
	m_isSide = isSide;
}

void BlockComponent::SetTexture(SideColor color, int width, int height)
{
	m_SideColor = color;
	std::string assetFile;
	switch (m_SideColor)
	{
	case SideColor::aqua:
		assetFile = "aqua.png";
		break;
	case SideColor::orange:
		assetFile = "orange.png";
		break;
	case SideColor::grey:
		assetFile = "grey.png";
		break;
	case SideColor::greyBlue:
		assetFile = "greyBlue.png";
		break;
	case SideColor::black:
		assetFile = "black.png";
		break;
	case SideColor::yellowOrange:
		assetFile = "yellowOrange.png";
		break;
	case SideColor::yellowBlue:
		assetFile = "yellowBlue.png";
		break;
	case SideColor::green:
		assetFile = "green.png";
		break;
	case SideColor::blackBlack:
		assetFile = "blackBlack.png";
		break;
	default:
		break;
	}
	m_TextureComponent->SetTexture(assetFile, width, height);
}

void BlockComponent::SetColorOrder(std::vector<int> colorOrder)
{
	m_ColorOrder = colorOrder;
	m_CurrentColor = 0;
	//set the sourceRect
	//col 0 is the side
	int col = 0, row = 0;
	if (!m_isSide)
		col = 1;
	row = m_ColorOrder[0];
	m_TextureComponent->SetSource(col, row, 2, 6);
}

void BlockComponent::SetTextureDestination(glm::vec3 dst)
{
	m_TextureDst = dst;
	m_TextureComponent->SetPosition(m_TextureDst.x, m_TextureDst.y);
}

void BlockComponent::ChangeColor()
{
	if (m_CurrentColor == m_ColorOrder.size() - 1)
		return;
	else
	{
		++m_CurrentColor;
		m_BlockChangePlayer->Execute();
		m_BlockChangeGrid->Execute();
		int col = 0, row = 0;
		if (!m_isSide)
			col = 1;
		row = m_ColorOrder[m_CurrentColor];
		m_TextureComponent->SetSource(col, row, 2, 6);
	}
}

bool BlockComponent::IsRightColor() const
{
	if (m_CurrentColor == m_ColorOrder.size() - 1)
		return true;
	return false;
}