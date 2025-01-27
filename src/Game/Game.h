#ifndef GAME_H
#define GAME_H
#include <set>
#include <SFML/Graphics/Sprite.hpp>

#include "Fruit.h"
#include "Player.h"
#include "../Engine/ObjectPool.h"

class Game
{
public:
	Game();
	~Game();

	void Update();

	void Render(sf::RenderWindow& window) const;

private:
	struct Boundary
	{
		enum Placement
		{
			LEFT = 0,
			RIGHT,
			BOTTOM,
			COUNT
		};

		sf::Vector2f m_TopLeft;
		sf::Vector2f m_BottomRight;
		sf::Vector2f m_Normal;
		static float m_Damping;
	};

	Boundary m_boundaries[Boundary::COUNT];

	ObjectPool<Fruit, 100> m_fruit;
	Player m_player;

	FruitManager::eType m_currentPlayerFruitType;
	FruitManager::eType m_nextPlayerFruitType;

	// TODO: Make this a lot better!
	// Create a data structure that can scale we have UI that changes over time
	// And be reused for other projects
	std::unordered_map <std::string, sf::Sprite*> m_uiElements;
	std::set<std::string> m_backgroundElements;
	std::set<std::string> m_midgroundElements;
	std::set<std::string> m_foregroundElements;

	bool LoadUI();

	static void DrawText(const std::string& string, const sf::Vector2f& position, sf::RenderWindow& window);

	static void DrawFruit(const Fruit& fruit, sf::RenderWindow& window);

	void HandleCollisions();
	static bool CircleRectCollision(Fruit& fruit, const Boundary& boundary);
	static bool CircleCircleCollision(Fruit& fruit, Fruit& otherFruit);
};


#endif
