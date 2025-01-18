#include "Player.h"
#include "../Engine/Keyboard.h"


Player::Player()
{
	// TODO: Make remap UI and Make these readable from a file
	// TODO: Account for actions to have multiple inputs (LEFT + A) or multiple input options if a controller is connected
	m_mapper.Map(LEFT, InputValue::eInputType::Keyboard, static_cast<int>(sf::Keyboard::Key::A));
	m_mapper.Map(RIGHT, InputValue::eInputType::Keyboard, static_cast<int>(sf::Keyboard::Key::D));
	m_mapper.Map(DROP, InputValue::eInputType::Keyboard, static_cast<int>(sf::Keyboard::Key::Space));
}

void Player::Update()
{
	// Update inputs
	m_mapper.Update();

	// Act on them
	if (m_mapper.GetInputValue(LEFT).IsReleased())
	{
		printf("LEFT!\n");
	}

	if (m_mapper.GetInputValue(RIGHT).IsReleased())
	{
		printf("RIGHT!\n");
	}

	if (m_mapper.GetInputValue(DROP).IsReleased())
	{
		printf("DROP!\n");
	}
}
