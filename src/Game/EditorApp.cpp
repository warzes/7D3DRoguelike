#include "stdafx.h"
#include "GameApp.h"

//-----------------------------------------------------------------------------
bool GameApp::Create()
{
	return true;
}
//-----------------------------------------------------------------------------
void GameApp::Destroy()
{
}
//-----------------------------------------------------------------------------
void GameApp::Render()
{
	auto& renderSystem = GetRenderSystem();
	auto& graphicsSystem = GetGraphicsSystem();

	if (m_windowWidth != GetWindowWidth() || m_windowHeight != GetWindowHeight())
	{
		m_windowWidth = GetWindowWidth();
		m_windowHeight = GetWindowHeight();
		renderSystem.SetViewport(m_windowWidth, m_windowHeight);
	}
	renderSystem.ClearFrame();	
}
//-----------------------------------------------------------------------------
void GameApp::Update(float deltaTime)
{
	if (GetInputSystem().IsKeyDown(Input::KEY_ESCAPE))
	{
		ExitRequest();
		return;
	}
}
//-----------------------------------------------------------------------------