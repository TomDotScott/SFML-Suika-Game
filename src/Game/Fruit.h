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

	~Fruit() override;

	void Update() override;

private:
	// TODO: Make this a texture
	sf::CircleShape m_shape;

	sf::Vector2f m_direction;

	static sf::Color GetColour(eFruitType type);
};


#endif
