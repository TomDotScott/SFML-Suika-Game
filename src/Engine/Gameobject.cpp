#include "Gameobject.h"


GameObject::GameObject(const sf::Vector2f position) :
	m_position(position)
{
	m_drawables.reserve(100);
}


void GameObject::Render(sf::RenderWindow& window) const
{
	for (const sf::Drawable* drawable : m_drawables)
	{
		window.draw(*drawable);
	}
}
