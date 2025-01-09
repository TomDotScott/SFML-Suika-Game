#include "Fruit.h"

#include "../Engine/Timer.h"
#include "../Engine/Globals.h"


Fruit::Fruit(const eFruitType type, const sf::Vector2f& position) :
	GameObject(position),
	m_direction(static_cast<float>(RNG.Next()), static_cast<float>(RNG.Next()))
{
	m_direction = m_direction.normalized();

	float radius = 0.0f;

	// TODO: Make better
	switch (type)
	{
	case eFruitType::Cherry:
		radius = 5;
		break;
	case eFruitType::Strawberry:
		radius = 10;
		break;
	case eFruitType::Grapes:
		radius = 15;
		break;
	case eFruitType::Dekopon:
		radius = 20;
		break;
	case eFruitType::Orange:
		radius = 30;
		break;
	case eFruitType::Apple:
		radius = 40;
		break;
	case eFruitType::Pear:
		radius = 50;
		break;
	case eFruitType::Peach:
		radius = 60;
		break;
	case eFruitType::Pineapple:
		radius = 70;
		break;
	case eFruitType::Melon:
		radius = 75;
		break;
	case eFruitType::Watermelon:
		radius = 100;
		break;
	}

	m_shape.setRadius(radius);

	m_shape.setFillColor(GetColour(type));

	m_shape.setPosition(m_position);


	// Set the point to the centre
	m_shape.setOrigin({ m_shape.getGlobalBounds().size.x / 2.f, m_shape.getGlobalBounds().size.y / 2.f });

	m_drawables.emplace_back(&m_shape);
}

Fruit::~Fruit()
{
}

void Fruit::Update()
{
	constexpr float speed = 500.f;

	m_position = m_position + m_direction * speed * Timer::Get().DeltaTime();

	if (m_position.x - m_shape.getRadius() < 0)
	{
		m_shape.setPosition({ m_shape.getRadius(), m_shape.getPosition().y });
		m_direction.x = -m_direction.x;
	}

	if (m_position.x + m_shape.getRadius() > static_cast<float>(GRAPHIC_SETTINGS.GetScreenDetails().m_ScreenSize.x))
	{
		m_shape.setPosition({ static_cast<float>(GRAPHIC_SETTINGS.GetScreenDetails().m_ScreenSize.x) - m_shape.getRadius(), m_shape.getPosition().y });
		m_direction.x = -m_direction.x;
	}

	if (m_position.y - m_shape.getRadius() < 0)
	{
		m_shape.setPosition({ m_shape.getPosition().x, m_shape.getRadius() });
		m_direction.y = -m_direction.y;
	}
	if (m_position.y + m_shape.getRadius() > static_cast<float>(GRAPHIC_SETTINGS.GetScreenDetails().m_ScreenSize.y))
	{
		m_shape.setPosition({ m_shape.getPosition().x, static_cast<float>(GRAPHIC_SETTINGS.GetScreenDetails().m_ScreenSize.y) - m_shape.getRadius() });
		m_direction.y = -m_direction.y;
	}

	m_shape.setPosition(m_position);
}

sf::Color Fruit::GetColour(const eFruitType type)
{
	switch (type)
	{
	case eFruitType::Cherry:
		return sf::Color(0xF20C3AFF);
	case eFruitType::Strawberry:
		return sf::Color(0xF51D00FF);
	case eFruitType::Grapes:
		return sf::Color(0x750CF2FF);
	case eFruitType::Dekopon:
		return sf::Color(0xF4C860FF);
	case eFruitType::Orange:
		return sf::Color(0xF2690CFF);
	case eFruitType::Apple:
		return sf::Color(0x97F20CFF);
	case eFruitType::Pear:
		return sf::Color(0x0CF285FF);
	case eFruitType::Peach:
		return sf::Color(0xF5C6E5FF);
	case eFruitType::Pineapple:
		return sf::Color(0xFAF36BFF);
	case eFruitType::Melon:
		return sf::Color(0xB8F5A4FF);
	case eFruitType::Watermelon:
		return sf::Color(0x002801FF);
	}

	return sf::Color::Magenta;
}


