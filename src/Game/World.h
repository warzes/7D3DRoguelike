#pragma once

#include "Viewport.h"

class World
{
public:
	bool Create();
	void Destroy();

	void Update(float deltaTime);
	void Draw(const Viewport& view);
};