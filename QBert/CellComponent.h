#pragma once
#include <BaseComponent.h>
#include <Transform.h>
#include "BlockComponent.h"
#include <PlayerComponent.h>

class Grid;
class CellComponent : public BaseComponent
{
public:
	explicit CellComponent() = default;
	virtual ~CellComponent() = default;

	void SetCube(std::shared_ptr<PlayerComponent> player, std::shared_ptr<Grid> grid, SideColor color, std::vector<int> colorOrder, bool isSide = false);
	void SetPosition(const float& x, const float& y);
	void SetSize(const float& size);

	glm::vec2 GetSpritePos() const;

	virtual void Initialize() override;
	virtual void Update() override {};

	bool IsCube() const;
	void ChangeColor();
	std::shared_ptr<BlockComponent> GetBlock() const { return m_BlockComponent; }
private:
	float m_Width = 0.f;
	float m_Height = 0.f;
	bool m_isCube = false;
	std::shared_ptr<Transform> m_TransformComponent = nullptr;
	std::shared_ptr<BlockComponent> m_BlockComponent = nullptr;
};

