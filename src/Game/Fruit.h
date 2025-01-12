#ifndef FRUIT_H
#define FRUIT_H

#include <SFML/Graphics/CircleShape.hpp>

#include "../Engine/Gameobject.h"

class Fruit : public GameObject
{
public:
	enum eFruitType
	{
		FRUIT_TYPE_Cherry,
		FRUIT_TYPE_Strawberry,
		FRUIT_TYPE_Grapes,
		FRUIT_TYPE_Dekopon,
		FRUIT_TYPE_Orange,
		FRUIT_TYPE_Apple,
		FRUIT_TYPE_Pear,
		FRUIT_TYPE_Peach,
		FRUIT_TYPE_Pineapple,
		FRUIT_TYPE_Melon,
		FRUIT_TYPE_Watermelon,
		FRUIT_TYPE_MAX
	};

	struct FruitDetails
	{
		std::string m_Name;
		sf::Color m_Colour;
		float m_Radius;
		int m_Points;
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

	const FruitDetails& GetCurrentFruitDetails() const;


private:
	// TODO: Make this a texture
	sf::CircleShape m_shape;
	eFruitType m_currentType;

	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	float m_mass;

	static FruitDetails LookupTable[FRUIT_TYPE_MAX];

	void InitialiseFruitDetails();
	void Move();
};


#endif
