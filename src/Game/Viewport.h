#pragma once

struct Viewport
{
	int windowWidth = 0;
	int windowHeight = 0;
	Camera camera;
	glm::mat4 perspective;
};