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

void BlockComponent::SetGameType(GameType type)
{
	m_Type = type;
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
	m_TextureComponent->SetTexture(assetFile);
	if (width > 0)
		m_TextureComponent->SetSize(width, height);
}

void BlockComponent::SetColorOrder(std::vector<int> colorOrder)
{
	m_ColorOrder = colorOrder;
	m_CurrentColor = 0;
	m_HasReachedFinalColorOnce = false;
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

ColorState BlockComponent::ChangeColor(bool reset)
{
	if (reset)
	{
		m_CurrentColor = 0;
		int col = 0, row = 0;
		if (!m_isSide)
			col = 1;
		row = m_ColorOrder[m_CurrentColor];
		m_TextureComponent->SetSource(col, row, 2, 6);
		return ColorState::START;
	}
	//change the bool, so no extra points are given in the repeat mode
	if (m_CurrentColor == m_ColorOrder.size() - 1)
	{
		m_HasReachedFinalColorOnce = true;
	}
	if (m_Type == GameType::RepeatSingleColor || m_CurrentColor != m_ColorOrder.size() - 1)
	{
		++m_CurrentColor;
		m_CurrentColor = m_CurrentColor % m_ColorOrder.size();
		ColorState newColorState = ColorState::START;
		//don't add score when you are in the repeat
		if (!m_HasReachedFinalColorOnce && m_CurrentColor != m_ColorOrder.size() - 1)
		{
			newColorState = ColorState::INTERMEDIAT;
		}
		else if (!m_HasReachedFinalColorOnce && m_CurrentColor == m_ColorOrder.size() - 1)
		{
			newColorState = ColorState::FINAL;
		}

		int col = 0, row = 0;
		if (!m_isSide)
			col = 1;
		row = m_ColorOrder[m_CurrentColor];
		m_TextureComponent->SetSource(col, row, 2, 6);
		return newColorState;
	}
}

bool BlockComponent::IsRightColor() const
{
	if (m_CurrentColor == m_ColorOrder.size() - 1)
		return true;
	return false;
}
