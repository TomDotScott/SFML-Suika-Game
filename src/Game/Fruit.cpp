#include "Fruit.h"

#include "../Engine/Timer.h"
#include "../Engine/Globals.h"


Fruit::FruitDetails Fruit::LookupTable[FRUIT_TYPE_MAX]
{
	{ FRUIT_TYPE_Cherry, "Cherry", sf::Color(0xF20C3AFF), 10.f, },
	{ FRUIT_TYPE_Strawberry, "Strawberry", sf::Color(0xF51D00FF), 20.f, },
	{ FRUIT_TYPE_Grapes, "Grapes", sf::Color(0x750CF2FF), 25.f, },
	{ FRUIT_TYPE_Dekopon, "Dekopon", sf::Color(0xF4C860FF), 30.f, },
	{ FRUIT_TYPE_Orange, "Orange", sf::Color(0xF2690CFF), 40.f, },
	{ FRUIT_TYPE_Apple, "Apple", sf::Color(0x97F20CFF), 45.f, },
	{ FRUIT_TYPE_Pear, "Pear", sf::Color(0x0CF285FF), 50.f, },
	{ FRUIT_TYPE_Peach, "Peach", sf::Color(0xF5C6E5FF), 65.f, },
	{ FRUIT_TYPE_Pineapple, "Pineapple", sf::Color(0xFAF36BFF), 60.f, },
	{ FRUIT_TYPE_Melon, "Melon", sf::Color(0xB8F5A4FF), 65.f, },
	{ FRUIT_TYPE_Watermelon, "Watermelon", sf::Color(0x002801FF), 80.f, },
};

Fruit::Fruit() :
	GameObject(),
	m_currentType(FRUIT_TYPE_Cherry),
	m_mass(0.f)
{
	InitialiseFruitDetails(m_currentType, sf::Vector2f(GRAPHIC_SETTINGS.GetScreenDetails().m_ScreenCentre));

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

// TODO: Work out some std::function magic to make this not be an overload
// see if there's a way we can call a protected member m_onActivatedFunc from
// OnActivate and pass through the params from the variadic ObjectPool class
void Fruit::OnActivate(const eFruitType type, const sf::Vector2f position)
{
	GameObject::OnActivate();

	InitialiseFruitDetails(type, position);
}

void Fruit::Upgrade()
{
	m_currentType = static_cast<eFruitType>(std::min(static_cast<int>(m_currentType) + 1, static_cast<int>(FRUIT_TYPE_Watermelon)));
	InitialiseFruitDetails(m_currentType, m_position);
}

void Fruit::InitialiseFruitDetails(const eFruitType type, const sf::Vector2f position)
{
	m_mass = LookupTable[type].m_Radius * 10.f;

	m_shape.setRadius(LookupTable[type].m_Radius);

	m_shape.setFillColor(LookupTable[type].m_Colour);

	m_shape.setOrigin({ m_shape.getGlobalBounds().size.x / 2.f, m_shape.getGlobalBounds().size.y / 2.f });

	m_currentType = type;

	m_position = position;
}

void Fruit::Move()
{
	m_velocity -= m_acceleration;

	m_position -= m_velocity /** Timer::Get().DeltaTime()*/;
}


