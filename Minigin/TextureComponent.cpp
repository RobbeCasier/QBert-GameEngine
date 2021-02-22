#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"

void TextureComponent::SetTexture(const std::string& filename)
{
	std::shared_ptr<Texture2D> pTexture = ResourceManager::GetInstance().LoadTexture(filename);
	RenderComponent* renderComp = m_GameObject->GetComponent<RenderComponent>();
	renderComp->SetTexture(pTexture);
}
