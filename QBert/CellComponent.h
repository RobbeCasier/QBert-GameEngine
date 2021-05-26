#pragma once
#include <BaseComponent.h>
#include <Transform.h>
#include "BlockComponent.h"
#include <PlayerComponent.h>
#include "Disc.h" 

class Grid;
class CellComponent : public BaseComponent
{
public:
	explicit CellComponent() = default;
	virtual ~CellComponent() = default;

	void SetCube(std::shared_ptr<PlayerComponent> player, std::shared_ptr<Grid> grid, SideColor color, std::vector<int> colorOrder, bool isSide = false);
	void SetDisc(SideColor color);
	void SetPosition(const float& x, const float& y);
	void SetSize(const float& size);

	glm::vec2 GetSpritePos() const;

	virtual void Initialize() override;
	virtual void Update() override {};

	bool IsCube() const;
	bool IsDisc() const;
	void ChangeColor(bool reset = false);
	void RemoveDisc();
	std::shared_ptr<BlockComponent> GetBlock() const { return std::dynamic_pointer_cast<BlockComponent>(m_Component); }
private:
	float m_Width = 0.f;
	float m_Height = 0.f;
	bool m_isCube = false;
	bool m_isDisc = false;
	std::shared_ptr<Transform> m_TransformComponent = nullptr;
	std::shared_ptr<BaseComponent> m_Component = nullptr;
};

