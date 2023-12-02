#pragma once

#include "GameState.h"


class GameApp final : public IApp
{
public:
	bool Create() final;
	void Destroy() final;

	void Render() final;
	void Update(float deltaTime) final;

	void SetState(StateType state);

private:
	Viewport m_view;
	Texture2DRef m_textures[2];

	IState* m_currentState = nullptr;
	GameState m_gameState;
};