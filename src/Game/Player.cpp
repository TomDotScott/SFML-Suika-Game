#include "Player.h"

#include "../Engine/Globals.h"
#include "../Engine/Input/Keyboard.h"
#include "../Engine/Input/Mouse.h"
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
	m_mapper.Map(DROP, eInputType::Mouse, static_cast<int>(sf::Mouse::Button::Left));
}

void Player::Update()
{
	// Update inputs
	m_mapper.Update();

	// If we've moved the mouse, let's give that priority over the position
	if (Mouse::Get().GetDelta().lengthSquared() > 0)
	{
		// We can set it directly here as the Game class corrects us later on, and we are using the mouse position relative to the window
		m_position.x = static_cast<float>(Mouse::Get().GetPosition().x);
	}

	if (m_mapper.IsButtonDown(LEFT))
	{
		m_position -= TRANSFORMED_VECTOR(m_speed) * Timer::Get().DeltaTime();
	}
	else if (m_mapper.IsButtonDown(RIGHT))
	{
		m_position += TRANSFORMED_VECTOR(m_speed) * Timer::Get().DeltaTime();
	}
}

unsigned Player::GetPoints() const
{
	return m_points;
}

bool Player::WantsDrop() const
{
	return m_mapper.IsButtonReleased(DROP);
}

void Player::AddPoints(const unsigned amount)
{
	m_points += amount;
}
