#include "Gameobject.h"

static uint64_t CURRENT_ID = 0;

GameObject::GameObject(const sf::Vector2f position) :
	m_id(++CURRENT_ID),
	m_position(position),
	m_isActive(false)
{
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

uint64_t GameObject::GetID() const
{
	return m_id;
}
