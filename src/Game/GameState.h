#pragma once

#include "IState.h"

class GameState : public IState
{
public:
	bool Create() final;
	void Destroy() final;

	void Update(float deltaTime) final;
	void Draw() final;

private:
	void cameraControl(float deltaTime);
};