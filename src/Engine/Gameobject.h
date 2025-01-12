#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class GameObject
{
public:
	explicit GameObject(sf::Vector2f position = { 0.f, 0.f });

	virtual ~GameObject() = default;

	virtual void Update() = 0;

	void Render(sf::RenderWindow& window) const;

	const sf::Vector2f& GetPosition() const { return m_position; }
	sf::Vector2f GetPosition() { return m_position; }
	void SetPosition(const sf::Vector2f& position) { m_position = position; }

	uint64_t GetID() const;

protected:
	const uint64_t m_id;

	sf::Vector2f m_position;

	std::vector<sf::Drawable*> m_drawables;
};

#endif
