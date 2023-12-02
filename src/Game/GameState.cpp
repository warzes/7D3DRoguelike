#include "stdafx.h"
#include "GameState.h"
//-----------------------------------------------------------------------------
bool GameState::Create()
{
	if (!m_world.Create())
		return false;

	m_view.camera.Teleport({ 0.0f, 1.0f, -3.0f });

	return true;
}
//-----------------------------------------------------------------------------
void GameState::Destroy()
{
	m_world.Destroy();
}
//-----------------------------------------------------------------------------
void GameState::Update(float deltaTime)
{
	cameraControl(deltaTime);
	m_world.Update(deltaTime);
}
//-----------------------------------------------------------------------------
void GameState::Draw()
{
	m_world.Draw(m_view);
}
//-----------------------------------------------------------------------------
void GameState::cameraControl(float deltaTime)
{
	const float mouseSensitivity = 10.0f * deltaTime;
	const float moveSpeed = 10.0f * deltaTime;

	auto& input = GetInputSystem();

	if (input.IsMouseButtonDown(Input::MOUSE_RIGHT))
	{
		if (m_cameraVisible)
		{
			m_cameraVisible = false;
			GetInputSystem().SetMouseLock(true);
		}
	}
	else
	{
		if (!m_cameraVisible)
		{
			m_cameraVisible = true;
			GetInputSystem().SetMouseLock(false);
		}
	}


	if (input.IsKeyDown(Input::KEY_W)) m_view.camera.MoveBy(moveSpeed);
	if (input.IsKeyDown(Input::KEY_S)) m_view.camera.MoveBy(-moveSpeed);
	if (input.IsKeyDown(Input::KEY_A)) m_view.camera.StrafeBy(moveSpeed);
	if (input.IsKeyDown(Input::KEY_D)) m_view.camera.StrafeBy(-moveSpeed);

	if (!m_cameraVisible)
	{
		glm::vec2 delta = input.GetMouseDeltaPosition();
		if (delta.x != 0.0f) m_view.camera.RotateLeftRight(delta.x * mouseSensitivity);
		if (delta.y != 0.0f) m_view.camera.RotateUpDown(-delta.y * mouseSensitivity);
	}
}
//-----------------------------------------------------------------------------