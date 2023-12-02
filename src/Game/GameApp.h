#pragma once

#include "GameState.h"


class GameApp final : public IApp
{
public:
	bool Create() final;
	void Destroy() final;

	void Render() final;
	void Update(float deltaTime) final;

private:
	Viewport m_view;

	IState* m_currentState = nullptr;
	GameState m_gameState{ m_view };
};