#ifndef GAME_H
#define GAME_H
#include <set>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Fruit.h"
#include "Player.h"
#include "../Engine/UI/UiManager.h"
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
	sf::RectangleShape m_playerGuideLine;

	FruitManager::eType m_currentPlayerFruitType;
	FruitManager::eType m_nextPlayerFruitType;

#if !BUILD_MASTER
	template<typename... Args>
	static void DrawText(sf::RenderWindow& window, const sf::Vector2f& position, const int size, const char* fmt, Args... args)
	{
		if (auto* debugUi = UIMANAGER.GetUiText("DEBUG_TEXT"))
		{
			debugUi->SetPosition(position);
			debugUi->SetTextSize(size);
			debugUi->SetText(fmt, args...);

			UIMANAGER.DrawDebugText(window);
		}
	}
#endif

	static void DrawFruit(const Fruit& fruit, sf::RenderWindow& window);

	void HandleCollisions();
	static bool CircleRectCollision(Fruit& fruit, const Boundary& boundary);
	static bool CircleCircleCollision(Fruit& fruit, Fruit& otherFruit);
};


#endif
