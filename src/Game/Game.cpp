#include "Game.h"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "../Engine/Globals.h"
#include "../Engine/Timer.h"

float Game::Boundary::m_Damping = -750.f;

Game::Game() :
	m_boundaries{ // TODO: This breaks in anything other than debug config...
	/*{{200.f, 0.f}, {200.f, 520.f}, VECTOR2F_RIGHT },
	{{1080.f, 0.f}, {1080.f, 520.f}, VECTOR2F_LEFT },*/
	{{200.f, 520.f}, {1080.f, 520.f}, VECTOR2F_DOWN }, },
	m_fruit(Fruit::eFruitType::Cherry, { static_cast<float>(GRAPHIC_SETTINGS.GetScreenDetails().m_ScreenCentre.x), 0.f })
{
	sf::Vector2f randomForce = { static_cast<float>(RNG.Next()), static_cast<float>(RNG.Next()) };
	randomForce = randomForce.normalized();
	// m_fruit.ApplyForce(randomForce * m_fruit.GetMass());
}

void Game::Update()
{
	// Gravity
	m_fruit.ApplyForce(sf::Vector2f(0.f, 1.f) * m_fruit.GetMass());

	m_fruit.Update();

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

	char buffer[512];
	sprintf(buffer, "Velocity: {%.3f,%.3f}\nAcceleration: {%.3f,%.3f}\nPosition: {%.3f,%.3f}", m_fruit.GetVelocity().x, m_fruit.GetVelocity().y, m_fruit.GetAcceleration().x, m_fruit.GetAcceleration().y, m_fruit.GetPosition().x, m_fruit.GetPosition().y);

	DrawText(buffer, { 0.f, 200.f }, window);
#endif

	m_fruit.Render(window);

#if !BUILD_MASTER
	/*if (static int frameCount = 0; ++frameCount % 2 == 0)
	{*/
	DrawText(std::to_string(static_cast<int>(Timer::Get().Fps())) + "fps", VECTOR2F_ZERO, window);
	//}
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
	for (const auto& boundary : m_boundaries)
	{
		if (CircleLineCollision(m_fruit, boundary))
		{
			std::cout << "COLLISION OCCURRED!" << std::endl;
		}
	}

	// If the fruit is stationary, make sure it isn't overlapping
	if (m_fruit.IsStationary())
	{
		if (m_fruit.GetPosition().y + m_fruit.GetRadius() > m_boundaries[0].m_P1.y)
		{
			m_fruit.SetPosition({ m_fruit.GetPosition().x, m_boundaries[0].m_P1.y - m_fruit.GetRadius() });
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

	printf("DISTANCE: %.2f\n", dist);
	printf("SIGNED DISTANCE: %.2f\n", signedDist);

	if (dist > fruit.GetRadius())
	{
		return false;
	}

	// We are close enough and heading in the right direction! We have a collision
	const sf::Vector2f reflectedVelocity = fruit.GetVelocity() - 2 * velocityInOffsetDirection * planeNormal;
	fruit.SetVelocity(reflectedVelocity);

	const sf::Vector2f dampener = Boundary::m_Damping * -planeNormal;
	fruit.ApplyForce(dampener);

	// Apply an up force to prevent the fruit from falling through the boundary
	const sf::Vector2f upForce = (fruit.GetRadius() - dist) * planeNormal;
	fruit.ApplyForce(upForce * fruit.GetMass());

	return true;
}

