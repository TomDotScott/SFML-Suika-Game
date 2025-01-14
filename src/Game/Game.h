#ifndef GAME_H
#define GAME_H
#include "Fruit.h"
#include "../Engine/ObjectPool.h"

class Game
{
public:
	Game();

	void Update();

	void Render(sf::RenderWindow& window) const;

private:
	struct Boundary
	{
		sf::Vector2f m_P1;
		sf::Vector2f m_P2;
		sf::Vector2f m_Normal;
		static float m_Damping;
	};

	Boundary m_boundaries[3];

	ObjectPool<Fruit, 100> m_fruit;

	static void DrawText(const std::string& string, const sf::Vector2f& position, sf::RenderWindow& window);

	void HandleCollisions();
	static bool CircleLineCollision(Fruit& fruit, const Boundary& boundary);
	static bool CircleCircleCollision(Fruit& fruit, Fruit& otherFruit);
};


#endif
