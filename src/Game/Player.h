#ifndef PLAYER_H
#define PLAYER_H
#include "../Engine/Gameobject.h"
#include "../Engine/InputMapper.h"

class Player final : public GameObject
{
public:
	Player();

	void Update() override;

	void AddPoints(unsigned amount);

	unsigned GetPoints() const;

	bool WantsMoveLeft() const;
	bool WantsMoveRight() const;
	bool WantsDrop() const;

private:
	enum eInputs : uint8_t
	{
		LEFT = 0,
		RIGHT,
		DROP
	};

	InputMapper m_mapper;
	unsigned m_points;
	sf::Vector2f m_speed;
};


#endif
