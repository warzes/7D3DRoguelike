#include "stdafx.h"
#include "GameApp.h"
//-----------------------------------------------------------------------------
bool GameApp::Create()
{
	if (!m_gameState.Create())
		return false;

	m_currentState = &m_gameState;

	return true;
}
//-----------------------------------------------------------------------------
void GameApp::Destroy()
{
	m_gameState.Destroy();
}
//-----------------------------------------------------------------------------
void GameApp::Render()
{
	auto& renderSystem = GetRenderSystem();

	if (m_view.windowWidth != GetWindowWidth() || m_view.windowHeight != GetWindowHeight())
	{
		m_view.windowWidth = GetWindowWidth();
		m_view.windowHeight = GetWindowHeight();
		renderSystem.SetViewport(m_view.windowWidth, m_view.windowHeight);
		m_view.perspective = glm::perspective(glm::radians(65.0f), GetWindowSizeAspect(), 0.01f, 1000.f);
	}
	renderSystem.SetClearColor({ 0.29f, 0.79f, 1.0f });
	renderSystem.ClearFrame();

	m_currentState->Draw();
}
//-----------------------------------------------------------------------------
void GameApp::Update(float deltaTime)
{
	if (GetInputSystem().IsKeyDown(Input::KEY_ESCAPE))
	{
		ExitRequest();
		return;
	}

	m_currentState->Update(deltaTime);
}
//-----------------------------------------------------------------------------