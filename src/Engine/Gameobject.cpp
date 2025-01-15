#include "Gameobject.h"

#include "Globals.h"

namespace
{
	uint64_t CURRENT_ID = 0;
}

GameObject::GameObject(const sf::Vector2f position) :
	m_isActive(false),
	m_id(++CURRENT_ID)
{
	SetPosition(position);
	m_drawables.reserve(100);
}

void GameObject::OnActivate()
{
	m_isActive = true;
}

void GameObject::OnDeactivate()
{
	m_isActive = false;
}

void GameObject::Render(sf::RenderWindow& window) const
{
	if (!m_isActive)
	{
		return;
	}

	for (const sf::Drawable* drawable : m_drawables)
	{
		window.draw(*drawable);
	}
}

void GameObject::SetPosition(const sf::Vector2f& position)
{
	m_position = position;
}

uint64_t GameObject::GetID() const
{
	return m_id;
}
