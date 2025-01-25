#ifndef FRUITMANAGER_H
#define FRUITMANAGER_H
#include <map>
#include <string>
#include <SFML/Graphics/Color.hpp>

class FruitManager
{
public:
	enum eType : std::int8_t
	{
		INVALID = -1,
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

	struct Details
	{
		eType m_Type;
		std::string m_Name;
		std::string m_TextureName;
		sf::Color m_Colour;
		float m_Radius;
		unsigned m_Points;
	};

	static FruitManager* Get();
	bool Init();

	const Details& GetFruitDetails(eType type) const;

	static eType GenerateRandomType();


private:
	FruitManager() = default;

	std::map<eType, Details> m_fruitDetails;
};

#define FRUIT_MANAGER FruitManager::Get()

#endif
