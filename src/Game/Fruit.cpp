#include "Fruit.h"

#include "../Engine/Timer.h"
#include "../Engine/Globals.h"


Fruit::Fruit(const eFruitType type, const sf::Vector2f& position) :
	GameObject(position),
	m_type(type),
	m_stationary(false)
{
	float radius = 0.0f;

	// TODO: Make better
	switch (m_type)
	{
	case eFruitType::Cherry:
		radius = 10;
		break;
	case eFruitType::Strawberry:
		radius = 20;
		break;
	case eFruitType::Grapes:
		radius = 25;
		break;
	case eFruitType::Dekopon:
		radius = 30;
		break;
	case eFruitType::Orange:
		radius = 40;
		break;
	case eFruitType::Apple:
		radius = 50;
		break;
	case eFruitType::Pear:
		radius = 55;
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

	m_mass = radius * 10.f;

	m_shape.setRadius(radius);

	m_shape.setFillColor(GetColour(type));

	m_shape.setPosition(m_position);


	// Set the point to the centre
	m_shape.setOrigin({ m_shape.getGlobalBounds().size.x / 2.f, m_shape.getGlobalBounds().size.y / 2.f });

	m_drawables.emplace_back(&m_shape);
}


void Fruit::Update()
{
	if (!m_stationary)
	{
		Move();
	}
	else
	{
		m_velocity = VECTOR2F_ZERO;
	}

	m_acceleration = VECTOR2F_ZERO;

	m_shape.setPosition(m_position);
}

float Fruit::GetRadius() const
{
	return m_shape.getRadius();
}

void Fruit::ApplyForce(const sf::Vector2f& force)
{
	m_acceleration += force / m_mass;
}

sf::Vector2f Fruit::GetVelocity() const
{
	return m_velocity;
}

void Fruit::SetVelocity(const sf::Vector2f velocity)
{
	m_stationary = velocity.length() <= 5.f;

	m_velocity = velocity;
}

float Fruit::GetMass() const
{
	return m_mass;
}

sf::Vector2f Fruit::GetAcceleration() const
{
	return m_acceleration;
}

bool Fruit::IsStationary() const
{
	return m_stationary;
}

Fruit::eFruitType Fruit::GetType() const
{
	return m_type;
}

std::string Fruit::GetTypeName() const
{
	switch (m_type) {
	case eFruitType::Cherry:
		return "Cherry";
	case eFruitType::Strawberry:
		return "Strawberry";
	case eFruitType::Grapes:
		return "Grapes";
	case eFruitType::Dekopon:
		return "Dekopon";
	case eFruitType::Orange:
		return "Orange";
	case eFruitType::Apple:
		return "Apple";
	case eFruitType::Pear:
		return "Pear";
	case eFruitType::Peach:
		return "Peach";
	case eFruitType::Pineapple:
		return "Pineapple";
	case eFruitType::Melon:
		return "Melon";
	case eFruitType::Watermelon:
		return "Watermelon";
	}

	return "UNKNOWN";
}

void Fruit::Move()
{
	m_velocity -= m_acceleration;

	m_position -= m_velocity /** Timer::Get().DeltaTime()*/;
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


