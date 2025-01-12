#include "Fruit.h"

#include "../Engine/Timer.h"
#include "../Engine/Globals.h"


Fruit::FruitDetails Fruit::LookupTable[FRUIT_TYPE_MAX]
{
	{ "Cherry", sf::Color(0xF20C3AFF), 10.f, },
	{ "Strawberry", sf::Color(0xF51D00FF), 20.f, },
	{ "Grapes", sf::Color(0x750CF2FF), 25.f, },
	{ "Dekopon", sf::Color(0xF4C860FF), 30.f, },
	{ "Orange", sf::Color(0xF2690CFF), 40.f, },
	{ "Apple", sf::Color(0x97F20CFF), 45.f, },
	{ "Pear", sf::Color(0x0CF285FF), 50.f, },
	{ "Peach", sf::Color(0xF5C6E5FF), 65.f, },
	{ "Pineapple", sf::Color(0xFAF36BFF), 60.f, },
	{ "Melon", sf::Color(0xB8F5A4FF), 65.f, },
	{ "Watermelon", sf::Color(0x002801FF), 80.f, },
};

Fruit::Fruit(const eFruitType type, const sf::Vector2f& position) :
	GameObject(position),
	m_currentType(type)
{
	InitialiseFruitDetails();

	m_shape.setPosition(m_position);


	// Set the point to the centre
	m_shape.setOrigin({ m_shape.getGlobalBounds().size.x / 2.f, m_shape.getGlobalBounds().size.y / 2.f });

	m_drawables.emplace_back(&m_shape);
}


void Fruit::Update()
{
	Move();

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

const Fruit::FruitDetails& Fruit::GetCurrentFruitDetails() const
{
	return LookupTable[m_currentType];
}

const Fruit::FruitDetails& Fruit::GetFruitDetails(eFruitType type)
{
	return LookupTable[type];
}

void Fruit::InitialiseFruitDetails()
{
	m_mass = LookupTable[m_currentType].m_Radius * 10.f;

	m_shape.setRadius(LookupTable[m_currentType].m_Radius);

	m_shape.setFillColor(LookupTable[m_currentType].m_Colour);
}

void Fruit::Move()
{
	m_velocity -= m_acceleration;

	m_position -= m_velocity /** Timer::Get().DeltaTime()*/;
}


