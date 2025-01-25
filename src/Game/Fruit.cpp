#include "Fruit.h"

#include "../Engine/Timer.h"
#include "../Engine/Globals.h"

Fruit::Fruit() :
	GameObject(),
	m_currentType(FruitManager::FRUIT_TYPE_Cherry),
	m_mass(0.f)
{
	Init(m_currentType, sf::Vector2f(GRAPHIC_SETTINGS.GetScreenDetails().m_ScreenCentre));
}

void Fruit::Update()
{
	Move();

	m_acceleration = VECTOR2F_ZERO;
}

float Fruit::GetRadius() const
{
	return TRANSFORMED_SCALAR(FRUIT_MANAGER->GetFruitDetails(m_currentType).m_Radius);
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

	Init(type, position);
}

void Fruit::Upgrade()
{
	m_currentType = static_cast<FruitManager::eType>(std::min(static_cast<int>(m_currentType) + 1, static_cast<int>(FruitManager::FRUIT_TYPE_Watermelon)));
	Init(m_currentType, m_position);
}

void Fruit::Init(const FruitManager::eType type, const sf::Vector2f position)
{
	m_currentType = type;

	const float radius = GetRadius();

	m_mass = radius * TRANSFORMED_SCALAR(5.f);

	m_position = position;
	m_acceleration = VECTOR2F_ZERO;
	m_velocity = VECTOR2F_ZERO;
}

void Fruit::Move()
{
	m_velocity -= m_acceleration;

	m_position -= m_velocity * Timer::Get().DeltaTime();
}


