#include "pch.h"
#include "Disc.h"
#include <GameTime.h>

Disc::~Disc()
{
	m_GameObject->RemoveComponent(m_TextureComponent);
}

void Disc::Initialize()
{
	m_TextureComponent = m_GameObject->AddComponent<TextureComponent>();
}

void Disc::Update()
{
	m_TimeElapsed += Time::GetInstance().GetElapsedTime();
	if (m_TimeElapsed > m_TimeNextCol)
	{
		m_TimeElapsed -= m_TimeNextCol;
		m_CurrentCol = ++m_CurrentCol % m_MaxColsAnimation;
		m_TextureComponent->SetSource(m_CurrentCol, 0, 5, 2);
	}
}


void Disc::SetTexture(SideColor color, int width, int height)
{
	std::string assetFile;
	switch (color)
	{
	case SideColor::aqua:
		assetFile = "aquaDisc.png";
		break;
	case SideColor::orange:
		assetFile = "orangeDisc.png";
		break;
	case SideColor::grey:
		assetFile = "greyDisc.png";
		break;
	case SideColor::greyBlue:
		assetFile = "greyBlueDisc.png";
		break;
	case SideColor::black:
		assetFile = "blackDisc.png";
		break;
	case SideColor::yellowOrange:
		assetFile = "yellowOrangeDisc.png";
		break;
	case SideColor::yellowBlue:
		assetFile = "yellowBlueDisc.png";
		break;
	case SideColor::green:
		assetFile = "greenDisc.png";
		break;
	case SideColor::blackBlack:
		assetFile = "blackBlackDisc.png";
		break;
	default:
		break;
	}
	m_TextureComponent->SetTexture(assetFile);
	m_SizeX = width * ((m_TextureComponent->GetWidth() / static_cast<float>(5)) / width) * 2;
	m_SizeY = height * ((m_TextureComponent->GetHeight() / static_cast<float>(2)) / height) * 2;
	m_TextureComponent->SetSize(m_SizeX, m_SizeY);
	m_TextureComponent->SetSource(m_CurrentCol, 0, 5, 2);
}

void Disc::SetTextureDestination(glm::vec3 dst)
{
	m_TextureRect.x = dst.x;
	m_TextureRect.y = dst.y;
	m_TextureRect.w = (int)m_SizeX;
	m_TextureRect.h = (int)m_SizeY;
	m_TextureComponent->SetPosition(m_TextureRect.x - m_SizeX/2, m_TextureRect.y - m_SizeY/2);
}
