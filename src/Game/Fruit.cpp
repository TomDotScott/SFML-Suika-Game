#include "Fruit.h"

#include "../Engine/Timer.h"
#include "../Engine/Globals.h"

Fruit::Fruit() :
	GameObject(),
	m_currentType(FruitManager::FRUIT_TYPE_Cherry),
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

const FruitManager::Details& Fruit::GetCurrentFruitDetails() const
{
	return FRUIT_MANAGER->GetFruitDetails(m_currentType);
}

// TODO: Work out some std::function magic to make this not be an overload
// see if there's a way we can call a protected member m_onActivatedFunc from
// OnActivate and pass through the params from the variadic ObjectPool class
void Fruit::OnActivate(const FruitManager::eType type, const sf::Vector2f position)
{
	GameObject::OnActivate();

	InitialiseFruitDetails(type, position);
}

void Fruit::Upgrade()
{
	m_currentType = static_cast<FruitManager::eType>(std::min(static_cast<int>(m_currentType) + 1, static_cast<int>(FruitManager::FRUIT_TYPE_Watermelon)));
	InitialiseFruitDetails(m_currentType, m_position);
}

void Fruit::InitialiseFruitDetails(const FruitManager::eType type, const sf::Vector2f position)
{
	const FruitManager::Details& fruitDetails = FRUIT_MANAGER->GetFruitDetails(type);

	const float radius = TRANSFORMED_SCALAR(fruitDetails.m_Radius);

	m_mass = radius * TRANSFORMED_SCALAR(5.f);

	m_shape.setRadius(radius);

	m_shape.setFillColor(fruitDetails.m_Colour);

	m_shape.setOrigin({ m_shape.getGlobalBounds().size.x / 2.f, m_shape.getGlobalBounds().size.y / 2.f });

	m_currentType = type;

	m_position = position;

	m_acceleration = VECTOR2F_ZERO;
	m_velocity = VECTOR2F_ZERO;

	m_shape.setPosition(m_position);
}

void Fruit::Move()
{
	m_velocity -= m_acceleration;

	m_position -= m_velocity * Timer::Get().DeltaTime();
}


