#pragma once

#include <glm/glm.hpp>

class Block
{
public:
	enum Type { dirt, grass, air, water};

	const static float size;

	Type GetType() const;
	void SetType(Type type);

	bool IsEnabled() const;
	void SetEnabled(bool state);
	bool IsSolid() const;

private:
	Type m_type = Type::dirt;
	bool m_enabled = true;
	bool m_solid = true;
};