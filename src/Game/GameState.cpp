#include "stdafx.h"
#include "GameState.h"
//-----------------------------------------------------------------------------
bool GameState::Create()
{
	m_view.camera.Teleport({ 0.0f, 1.0f, -2.0f });

	if (!m_world.Create())
		return false;

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
	const float mouseSensitivity = 20.0f * deltaTime;
	const float moveSpeed = 20.0f * deltaTime;

	if (GetInputSystem().IsKeyDown(Input::KEY_W)) m_view.camera.MoveBy(moveSpeed);
	if (GetInputSystem().IsKeyDown(Input::KEY_S)) m_view.camera.MoveBy(-moveSpeed);
	if (GetInputSystem().IsKeyDown(Input::KEY_A)) m_view.camera.StrafeBy(moveSpeed);
	if (GetInputSystem().IsKeyDown(Input::KEY_D)) m_view.camera.StrafeBy(-moveSpeed);

	glm::vec2 delta = GetInputSystem().GetMouseDeltaPosition();
	if (delta.x != 0.0f)  m_view.camera.RotateLeftRight(delta.x * mouseSensitivity);
	if (delta.y != 0.0f)  m_view.camera.RotateUpDown(-delta.y * mouseSensitivity);
}
//-----------------------------------------------------------------------------