#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <functional>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class GameObject
{
public:
	explicit GameObject(sf::Vector2f position = { 0.f, 0.f });

	virtual ~GameObject() = default;

	virtual void Update() = 0;

	void OnActivate();

	void OnDeactivate();

	void Render(sf::RenderWindow& window) const;

	const sf::Vector2f& GetPosition() const { return m_position; }
	sf::Vector2f GetPosition() { return m_position; }
	void SetPosition(const sf::Vector2f& position);

	uint64_t GetID() const;

	bool IsActive() const { return m_isActive; }

protected:
	sf::Vector2f m_position;

	std::vector<sf::Drawable*> m_drawables;

private:
	bool m_isActive;
	uint64_t m_id;
};

#endif
