#pragma once
#include "BaseComponent.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>

#pragma warning(pop)

class Transform final : public BaseComponent
{
public:
	explicit Transform() = default;
	
	virtual void Initialize() override {};
	virtual void Update() override {};
	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(float x, float y, float z);
private:
	glm::vec3 m_Position;
};
