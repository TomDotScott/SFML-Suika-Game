#ifndef FRUIT_H
#define FRUIT_H

#include <SFML/Graphics/CircleShape.hpp>

#include "../Engine/Gameobject.h"

class Fruit : public GameObject
{
public:
	enum class eFruitType
	{
		Cherry,
		Strawberry,
		Grapes,
		Dekopon,
		Orange,
		Apple,
		Pear,
		Peach,
		Pineapple,
		Melon,
		Watermelon
	};

	Fruit(eFruitType type, const sf::Vector2f& position);

	~Fruit() override = default;

	void Update() override;

	float GetRadius() const;

	void ApplyForce(const sf::Vector2f& force);

	sf::Vector2f GetVelocity() const;
	void SetVelocity(sf::Vector2f velocity);

	float GetMass() const;
	sf::Vector2f GetAcceleration() const;

	eFruitType GetType() const;
	std::string GetTypeName() const;

private:
	// TODO: Make this a texture
	sf::CircleShape m_shape;
	eFruitType m_type;

	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	float m_mass;

	void Move();
	static sf::Color GetColour(eFruitType type);
};


#endif
