#pragma once

#include "Viewport.h"

class IState
{
public:
	IState(Viewport& view) : m_view(view) {}
	virtual ~IState() = default;

	virtual bool Create() = 0;
	virtual void Destroy() = 0;

	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;

protected:
	Viewport& m_view;
};