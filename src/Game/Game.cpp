#include "Game.h"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Fruit.h"
#include "../Engine/Globals.h"
#include "../Engine/Timer.h"

float Game::Boundary::m_Damping = 0.8f;

Game::Game() :
	m_boundaries{ // TODO: This breaks in anything other than debug config...
	/*{{200.f, 0.f}, {200.f, 520.f}, VECTOR2F_RIGHT },
	{{1080.f, 0.f}, {1080.f, 520.f}, VECTOR2F_LEFT },*/
	{{200.f, 520.f}, {1080.f, 520.f}, VECTOR2F_DOWN }, }
{
	constexpr int fruitAmount = 10;

	m_fruit.reserve(fruitAmount);

	constexpr float farLeft = 200.f;
	constexpr float farRight = 1080.f;
	constexpr float difference = farRight - farLeft;

	for (int i = 1; i <= fruitAmount; ++i)
	{
		const float xPosition = farLeft + static_cast<float>(i - 1) * (difference / fruitAmount);

		m_fruit.emplace_back(static_cast<Fruit::eFruitType>(i - 1), sf::Vector2f{ xPosition, 0.f });
	}
}

void Game::Update()
{
	// Gravity
	for (auto& fruit : m_fruit)
	{
		fruit.ApplyForce(sf::Vector2f(0.f, 1.f) * fruit.GetMass());
		fruit.Update();
	}

	HandleCollisions();
}

void Game::Render(sf::RenderWindow& window) const
{
#if !BUILD_MASTER
	for (const auto& [point1, point2, normal] : m_boundaries)
	{
		const sf::Vertex line[] = { {point1, sf::Color::Magenta}, {point2, sf::Color::Magenta} };
		window.draw(line, 2, sf::PrimitiveType::Lines);
	}
#endif

	for (const auto& fruit : m_fruit)
	{
#if !BUILD_MASTER
		char buffer[512];

		sprintf(buffer,
			"%s\n"
			"Vel{%.3f,%.3f}\n"
			"Mass:%.2f\n"
			"Acc{%.3f,%.3f}\n"
			"Pos{%.3f,%.3f}",
			fruit.GetTypeName().c_str(),
			fruit.GetVelocity().x, fruit.GetVelocity().y,
			fruit.GetMass(),
			fruit.GetAcceleration().x, fruit.GetAcceleration().y,
			fruit.GetPosition().x, fruit.GetPosition().y
		);

		DrawText(buffer, { fruit.GetPosition().x - fruit.GetRadius(), fruit.GetPosition().y - fruit.GetRadius() - 80.f }, window);
#endif
		fruit.Render(window);
	}

#if !BUILD_MASTER
	DrawText(std::to_string(static_cast<int>(Timer::Get().Fps())) + "fps", VECTOR2F_ZERO, window);
#endif
}

void Game::DrawText(const std::string& string, const sf::Vector2f& position, sf::RenderWindow& window)
{
	// TODO: Make a proper text renderer
	const static sf::Font defaultFont{ "fonts/FiraCode-Regular.ttf" };

	sf::Text text(defaultFont, string, 10);
	text.setPosition(position);
	text.setFillColor(sf::Color::White);

	window.draw(text);
}

void Game::HandleCollisions()
{
	for (auto& fruit : m_fruit)
	{
		for (const auto& boundary : m_boundaries)
		{
			if (CircleLineCollision(fruit, boundary))
			{
				std::cout << "COLLISION OCCURRED!" << std::endl;
			}
		}

		// If the fruit is stationary, make sure it isn't overlapping
		if (fruit.IsStationary())
		{
			if (fruit.GetPosition().y + fruit.GetRadius() > m_boundaries[0].m_P1.y)
			{
				fruit.SetPosition({ fruit.GetPosition().x, m_boundaries[0].m_P1.y - fruit.GetRadius() });
			}
		}
	}
}

bool Game::CircleLineCollision(Fruit& fruit, const Boundary& boundary)
{
	const sf::Vector2f planeNormal = boundary.m_Normal;

	// Check that the fruit is headed in the right direction
	const float velocityInOffsetDirection = fruit.GetVelocity().dot(planeNormal);
	if (velocityInOffsetDirection > 0.0f)
	{
		return false;
	}

	// Calculate the closest point on the boundary line segment
	const sf::Vector2f lineVector = boundary.m_P2 - boundary.m_P1;
	const sf::Vector2f normalizedLineVector = lineVector / std::sqrt(lineVector.dot(lineVector));
	const sf::Vector2f fruitToLineStart = fruit.GetPosition() - boundary.m_P1;
	const float projection = fruitToLineStart.dot(normalizedLineVector);
	const float clampedProjection = std::clamp(projection, 0.0f, lineVector.dot(normalizedLineVector));
	const sf::Vector2f closestPoint = boundary.m_P1 + clampedProjection * normalizedLineVector;
	const sf::Vector2f fruitToBoundary = fruit.GetPosition() - closestPoint;

	// Check if the distance to the boundary is within the fruit's radius
	const float dist = std::sqrt(fruitToBoundary.dot(fruitToBoundary));
	const float signedDist = fruitToBoundary.dot(planeNormal);

	if (dist > fruit.GetRadius())
	{
		return false;
	}

	// We are close enough and heading in the right direction! We have a collision
	const sf::Vector2f reflectedVelocity = fruit.GetVelocity() - 2 * velocityInOffsetDirection * planeNormal;
	fruit.SetVelocity(reflectedVelocity * Boundary::m_Damping);

	// Fix any penetration
	const sf::Vector2f distanceVector = fruit.GetPosition() - closestPoint;
	const float distanceSquared = distanceVector.lengthSquared();
	const sf::Vector2f collisionNormal = distanceVector.normalized();
	const float penetrationDepth = fruit.GetRadius() - std::sqrt(distanceSquared);

	printf("PENETRATION DEPTH: %.2f\n", penetrationDepth);

	// Resolve overlap by moving the circle away from the collision
	fruit.SetPosition(fruit.GetPosition() + collisionNormal * penetrationDepth);

	return true;
}

