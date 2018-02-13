#pragma once

#include <glm/glm.hpp>

enum CubeType { dirt, grass, water, air };

class Cube
{
public:
	
	static const float size;

	Cube(CubeType type = CubeType::dirt);

	CubeType Type() const;
	void SetType(CubeType type);

	bool IsEnabled() const;
	bool IsSolid() const;

	void SetEnabled(bool state);

private:
	CubeType m_type;

	bool m_enabled = true;
	bool m_solid = true;

};