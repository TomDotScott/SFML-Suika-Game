#ifndef PLAYER_H
#define PLAYER_H
#include "../Engine/InputMapper.h"

class Player
{
public:
	Player();
	void Update();

private:
	enum eInputs : uint8_t
	{
		LEFT = 0,
		RIGHT,
		DROP
	};


	InputMapper m_mapper;
};


#endif
