#include "Game.h"
#include <fstream>
#include <set>
#include <SFML/Graphics.hpp>

#include "Fruit.h"
#include "FruitManager.h"
#include "../Engine/TextureManager.h"
#include "../Engine/Globals.h"
#include "../Engine/Timer.h"
#include "../Libs/hoxml.h"

float Game::Boundary::m_Damping = 0.8f;

Game::Game() :
	// TODO: Make this data be read from a file, I am fed up of editing stuff in C++ files
	m_boundaries{
	{ TRANSFORMED_VECTOR(sf::Vector2f(424.f, 84.f)),    TRANSFORMED_VECTOR(sf::Vector2f(455.f, 696.f)), VECTOR2F_RIGHT},
	{ TRANSFORMED_VECTOR(sf::Vector2f(825.f, 84.f)), TRANSFORMED_VECTOR(sf::Vector2f(856.f, 696.f)), VECTOR2F_LEFT },
	{ TRANSFORMED_VECTOR(sf::Vector2f(424.f, 665.f)),  TRANSFORMED_VECTOR(sf::Vector2f(856.f, 696.f)), VECTOR2F_UP } },
	m_fruit(),
	m_player(),
	m_currentPlayerFruitType(FRUIT_MANAGER->GenerateRandomType()),
	m_nextPlayerFruitType(FRUIT_MANAGER->GenerateRandomType())
{
	UIMANAGER.Load("ui.xml");

	m_player.SetPosition({
		static_cast<float>(GRAPHIC_SETTINGS.GetScreenDetails().m_ScreenCentre.x),
		FRUIT_MANAGER->GetFruitDetails(FruitManager::eType::FRUIT_TYPE_Apple).m_Radius + 20.f
		}
	);

	// TODO: This is a bit stinky! I think it would probably be better to make this be another texture
	const sf::Vector2f boxSize = { TRANSFORMED_SCALAR(5.f), TRANSFORMED_SCALAR(static_cast<float>(TEXTUREMANAGER.GetTexture("Box")->getSize().y)) };

	m_playerGuideLine.setSize(boxSize);
	m_playerGuideLine.setFillColor(static_cast<sf::Color>(0xFFFFFF40));
	m_playerGuideLine.setOrigin({ m_playerGuideLine.getLocalBounds().size.x / 2.f, 0.f });
}

Game::~Game()
{
}

void Game::Update()
{
	m_player.Update();

	constexpr static float PADDING = 10.f;

	const bool playerHasFruit = m_currentPlayerFruitType != FruitManager::eType::INVALID;
	if (playerHasFruit)
	{
		const float currentRadius = FRUIT_MANAGER->GetFruitDetails(m_currentPlayerFruitType).m_Radius;
		const float padding = TRANSFORMED_SCALAR(PADDING);

		// Stop the player from going O.O.B.
		if (m_player.GetPosition().x - currentRadius - padding < m_boundaries[Boundary::LEFT].m_BottomRight.x)
		{
			m_player.SetPosition(
				sf::Vector2f{
					m_boundaries[Boundary::LEFT].m_BottomRight.x + currentRadius + padding,
					m_player.GetPosition().y
				}
			);
		}
		else if (m_player.GetPosition().x + currentRadius + padding > m_boundaries[Boundary::RIGHT].m_TopLeft.x)
		{
			m_player.SetPosition(
				sf::Vector2f{
					m_boundaries[Boundary::RIGHT].m_TopLeft.x - currentRadius - padding,
					m_player.GetPosition().y
				}
			);
		}

		m_playerGuideLine.setPosition(m_player.GetPosition());
	}

	static float timer = 0.f;

	if (m_player.WantsDrop() && playerHasFruit)
	{
		m_fruit.ActivateObject(m_currentPlayerFruitType, m_player.GetPosition());

		m_currentPlayerFruitType = FruitManager::eType::INVALID;
		timer = 0.f;
		// TODO: Check for game over
	}


	// Wait a second before generating the next fruit after a drop
	timer += Timer::Get().DeltaTime();

	static constexpr float TIME_BEFORE_SPAWNING_NEW_FRUIT = 0.15f;

	if (timer >= TIME_BEFORE_SPAWNING_NEW_FRUIT && !playerHasFruit)
	{
		m_currentPlayerFruitType = m_nextPlayerFruitType;
		m_nextPlayerFruitType = FRUIT_MANAGER->GenerateRandomType();
	}

	for (auto& fruit : m_fruit)
	{
		// Gravity
		fruit.ApplyForce(sf::Vector2f(0.f, 20.f) * fruit.GetMass());
		fruit.Update();
	}

	static constexpr int RESOLUTION = 5;
	for (int i = 0; i < RESOLUTION; i++)
	{
		HandleCollisions();
	}

	UIMANAGER.GetUiPanel("ScoreBubble_Panel")->GetUiText("ScoreBubble_Score")->SetText(std::to_string(m_player.GetPoints()).c_str());
}

void Game::Render(sf::RenderWindow& window) const
{
	// Draw the background items first
	UIMANAGER.RenderBackground(window);

#if !BUILD_MASTER
	for (const auto& [topLeft, bottomRight, normal] : m_boundaries)
	{
		sf::RectangleShape rect{ bottomRight - topLeft };
		rect.setFillColor(static_cast<sf::Color>(0x808080FF));
		rect.setPosition(topLeft);
		window.draw(rect);
	}
#endif

	// Then the midground stuff
	UIMANAGER.RenderMidground(window);

	// Then the game elements
	window.draw(m_playerGuideLine);

	for (const auto& fruit : m_fruit)
	{
#if !BUILD_MASTER
		DrawText(window,
			{
			fruit.GetPosition().x - fruit.GetRadius(),
			fruit.GetPosition().y - fruit.GetRadius() - 80.f
			},
			10,
			"%s\n"
			"ID: %llu\n"
			"Vel{%.3f,%.3f}\n"
			"Mass:%.2f\n"
			"Acc{%.3f,%.3f}\n"
			"Pos{%.3f,%.3f}",
			fruit.GetCurrentFruitDetails().m_Name.c_str(),
			fruit.GetID(),
			fruit.GetVelocity().x, fruit.GetVelocity().y,
			fruit.GetMass(),
			fruit.GetAcceleration().x, fruit.GetAcceleration().y,
			fruit.GetPosition().x, fruit.GetPosition().y
		);
#endif

		DrawFruit(fruit, window);
	}

	if (m_currentPlayerFruitType != FruitManager::eType::INVALID)
	{
		Fruit dummy;
		dummy.OnActivate(m_currentPlayerFruitType, m_player.GetPosition());
		DrawFruit(dummy, window);
	}

	// Then, in front of everything, the foreground stuff
	UIMANAGER.RenderForeground(window);

#if !BUILD_MASTER
	DrawText(window, VECTOR2F_ZERO, 30, "%.1fFPS", Timer::Get().Fps());
	DrawText(window, VECTOR2F_ZERO + sf::Vector2f{ 0.f, 100.f }, 10, "Active Fruit: %llu", m_fruit.GetInUseCount());
#endif
}

void Game::DrawFruit(const Fruit& fruit, sf::RenderWindow& window)
{
	const sf::Texture* texture = TEXTUREMANAGER.GetTexture(fruit.GetCurrentFruitDetails().m_TextureName);

	const float radius = fruit.GetRadius();
	const sf::Vector2f scaleFactor = static_cast<sf::Vector2f>(texture->getSize()) / (radius * 2);

	sf::Sprite sprite(*texture);

	sprite.setScale({ 1.f / scaleFactor.x, 1.f / scaleFactor.y });
	sprite.setOrigin({ sprite.getLocalBounds().size.x / 2.f, sprite.getLocalBounds().size.y / 2.f });
	sprite.setPosition(fruit.GetPosition());

#if BUILD_DEBUG
	sf::CircleShape circle(radius);
	circle.setOrigin({ circle.getGlobalBounds().size.x / 2.f, circle.getGlobalBounds().size.y / 2.f });
	circle.setPosition(fruit.GetPosition());
	circle.setOutlineColor(fruit.GetCurrentFruitDetails().m_Colour);
	circle.setOutlineThickness(3.f);
	circle.setFillColor(sf::Color::Transparent);
	window.draw(circle);
#endif

#if RENDER_SPRITES
	window.draw(sprite);
#endif
}

void Game::HandleCollisions()
{
	std::set<Fruit*> fruitToBeRemoved;
	std::set<Fruit*> fruitToBeUpgraded;

	for (Fruit& fruit : m_fruit)
	{
		// Is the current fruit in the "to be removed" list?
		if (fruitToBeRemoved.find(&fruit) != fruitToBeRemoved.end())
		{
			continue;
		}

		for (Fruit& otherFruit : m_fruit)
		{
			if (fruit.GetID() == otherFruit.GetID())
			{
				continue;
			}

			// Is the other fruit in the "to be removed" list?
			if (fruitToBeRemoved.find(&otherFruit) != fruitToBeRemoved.end())
			{
				continue;
			}

			const bool areFruitTheSame = fruit.GetCurrentFruitDetails().m_Type == otherFruit.GetCurrentFruitDetails().m_Type;
			const bool isWatermelon = fruit.GetCurrentFruitDetails().m_Type == FruitManager::eType::FRUIT_TYPE_Watermelon;

			if (CircleCircleCollision(fruit, otherFruit)
				&& areFruitTheSame
				&& !isWatermelon)
			{
				const sf::Vector2f midpoint = (otherFruit.GetPosition() + fruit.GetPosition()) / 2.f;

				if (fruit.GetPosition().y < otherFruit.GetPosition().y)
				{
					fruitToBeRemoved.insert(&fruit);
					fruitToBeUpgraded.insert(&otherFruit);
					otherFruit.SetPosition(midpoint);
				}
				else
				{
					fruitToBeRemoved.insert(&otherFruit);
					fruitToBeUpgraded.insert(&fruit);
					fruit.SetPosition(midpoint);
				}
			}
		}

		for (const auto& boundary : m_boundaries)
		{
			if (CircleRectCollision(fruit, boundary))
			{
			}
		}
	}

	for (Fruit* removeFruit : fruitToBeRemoved)
	{
		m_fruit.DeactivateObject(removeFruit);
	}

	for (Fruit* upgradeFruit : fruitToBeUpgraded)
	{
		m_player.AddPoints(upgradeFruit->GetCurrentFruitDetails().m_Points);
		upgradeFruit->Upgrade();
	}
}

bool Game::CircleRectCollision(Fruit& fruit, const Boundary& boundary)
{
	const sf::Vector2f closestPointOnRectangle
	{
		std::clamp(fruit.GetPosition().x, boundary.m_TopLeft.x, boundary.m_BottomRight.x),
		std::clamp(fruit.GetPosition().y, boundary.m_TopLeft.y, boundary.m_BottomRight.y)
	};

	const sf::Vector2f fruitToRect = fruit.GetPosition() - closestPointOnRectangle;
	const float distance = fruitToRect.length();

	if (distance > fruit.GetRadius())
	{
		return false;
	}

	const sf::Vector2f normal = boundary.m_Normal;

	//  Work out reflected velocity
	const float impulse = 2 * fruit.GetVelocity().dot(normal);
	fruit.SetVelocity((fruit.GetVelocity() - impulse * normal) * Boundary::m_Damping);

	// Sort out collision if penetrated
	const float penetrationDepth = fruit.GetRadius() - distance;
	fruit.SetPosition(fruit.GetPosition() + (normal * penetrationDepth));
	return true;
}

bool Game::CircleCircleCollision(Fruit& fruit, Fruit& otherFruit)
{
	const sf::Vector2f centreToCentre = fruit.GetPosition() - otherFruit.GetPosition();

	const sf::Vector2f relativeVelocity = otherFruit.GetVelocity() - fruit.GetVelocity();

	const float velocityInOffsetDirection = relativeVelocity.dot(centreToCentre);

	if (velocityInOffsetDirection > 0)
	{
		return false;
	}

	const float distanceBetweenFruit = centreToCentre.length();

	const float sumRadii = fruit.GetRadius() + otherFruit.GetRadius();

	if (distanceBetweenFruit > sumRadii)
	{
		return false;
	}

	const float timeOfImpact = -(distanceBetweenFruit - sumRadii) / velocityInOffsetDirection;

	if (timeOfImpact > Timer::Get().DeltaTime())
	{
		return false;
	}

	const sf::Vector2f collisionNormal = centreToCentre / distanceBetweenFruit;

	const float velocityAlongNormal = relativeVelocity.dot(collisionNormal);

	sf::Vector2f pointOfCollision = fruit.GetPosition() - collisionNormal * fruit.GetRadius();

	constexpr float coefficientOfRestitution = 0.8f;

	// Impulse scalar
	float j = -(1.f + coefficientOfRestitution) * velocityAlongNormal;

	const float fruitInverseMass = 1.f / fruit.GetMass();
	const float otherFruitInverseMass = 1.f / otherFruit.GetMass();
	const float totalInverseMass = fruitInverseMass + otherFruitInverseMass;

	j /= totalInverseMass;

	const sf::Vector2f impulse = j * collisionNormal;

	fruit.SetVelocity(fruit.GetVelocity() - impulse / fruit.GetMass());
	otherFruit.SetVelocity(otherFruit.GetVelocity() + impulse / otherFruit.GetMass());

	// Positional correction
	const float penetrationDepth = sumRadii - distanceBetweenFruit;

	// Adjust positions to resolve overlap, taking mass into account.
	const float fruitCorrectionFactor = fruitInverseMass / totalInverseMass;
	const float otherFruitCorrectionFactor = otherFruitInverseMass / totalInverseMass;

	const sf::Vector2f correction = collisionNormal * penetrationDepth;

	fruit.SetPosition(fruit.GetPosition() + correction * fruitCorrectionFactor);
	otherFruit.SetPosition(otherFruit.GetPosition() - correction * otherFruitCorrectionFactor);

	return true;
}
