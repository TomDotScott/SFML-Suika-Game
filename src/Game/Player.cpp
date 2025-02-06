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
	m_mapper.Map(LEFT, eInputType::Keyboard, static_cast<int>(sf::Keyboard::Key::A));
	m_mapper.Map(LEFT, eInputType::Keyboard, static_cast<int>(sf::Keyboard::Key::Left));

	m_mapper.Map(RIGHT, eInputType::Keyboard, static_cast<int>(sf::Keyboard::Key::D));
	m_mapper.Map(RIGHT, eInputType::Keyboard, static_cast<int>(sf::Keyboard::Key::Right));

	m_mapper.Map(DROP, eInputType::Keyboard, static_cast<int>(sf::Keyboard::Key::Space));
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
	return m_mapper.IsButtonDown(LEFT);
}

bool Player::WantsMoveRight() const
{
	return m_mapper.IsButtonDown(RIGHT);
}

bool Player::WantsDrop() const
{
	return m_mapper.IsButtonReleased(DROP);
}

void Player::AddPoints(const unsigned amount)
{
	m_points += amount;
}
