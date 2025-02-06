#include "Player.h"

#include "../Engine/Globals.h"
#include "../Engine/Input/Keyboard.h"
#include "../Engine/Timer.h"


Player::Player() :
	GameObject(),
	m_points(0),
	m_speed()
{
	// Half the screen in a second
	m_speed = sf::Vector2f(REFERENCE_SCREEN_SIZE.x / 2.f, 0.f);

	// TODO: Make remap UI and Make these readable from a file
}

void Player::Update()
{
	// Update inputs
	m_mapper.Update();


	if (WantsMoveLeft())
	{
		m_position -= TRANSFORMED_VECTOR(m_speed) * Timer::Get().DeltaTime();
	}
	else if (WantsMoveRight())
	{
		m_position += TRANSFORMED_VECTOR(m_speed) * Timer::Get().DeltaTime();
	}
}

unsigned Player::GetPoints() const
{
	return m_points;
}

bool Player::WantsMoveLeft() const
{
}

bool Player::WantsMoveRight() const
{
}

bool Player::WantsDrop() const
{
}

void Player::AddPoints(const unsigned amount)
{
	m_points += amount;
}
