#ifndef FRUIT_H
#define FRUIT_H

#include <SFML/Graphics/CircleShape.hpp>

#include "FruitManager.h"
#include "../Engine/Gameobject.h"
#include "../Engine/Globals.h"

class Fruit final : public GameObject, Updatable
{
public:
	Fruit();

	~Fruit() override = default;

	void Update() override;

	float GetRadius() const;

	void ApplyForce(const sf::Vector2f& force);

	sf::Vector2f GetVelocity() const;
	void SetVelocity(sf::Vector2f velocity);

	float GetMass() const;
	sf::Vector2f GetAcceleration() const;

	const FruitManager::Details& GetCurrentFruitDetails() const;

	void OnActivate(FruitManager::eType type, sf::Vector2f position = VECTOR2F_ZERO);
	void Upgrade();

private:
	FruitManager::eType m_currentType;

	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	float m_mass;

	void Init(FruitManager::eType type, sf::Vector2f position);
	void Move();
};


#endif
