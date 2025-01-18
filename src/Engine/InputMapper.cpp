#include "InputMapper.h"

#include "Keyboard.h"


InputValue::InputValue(const eInputType type, const int button) :
	m_type(type),
	m_button(button),
	m_value(0),
	m_previousValue(0)
{
}

uint8_t InputValue::GetValue() const
{
	return m_value;
}

bool InputValue::IsPressed() const
{
	return m_value && !m_previousValue;
}

bool InputValue::IsDown() const
{
	return m_value;
}

bool InputValue::IsReleased() const
{
	return m_previousValue && !m_value;
}

InputMapper::InputMapper() = default;

void InputMapper::Update()
{
	for (auto& [ID, inputValue] : m_inputs)
	{
		switch (inputValue.m_type)
		{
		case InputValue::eInputType::Keyboard:
			inputValue.m_previousValue = inputValue.m_value;
			inputValue.m_value = Keyboard::Get().IsButtonDown(static_cast<sf::Keyboard::Key>(inputValue.m_button)) ? 0xFF : 0x00;
			break;
		case InputValue::eInputType::Mouse:
			// TODO: Implement a wrapper for sf::Mouse like Keyboard.h
			break;
		case InputValue::eInputType::Controller:
			// TODO: Maybe xInput? The sfml joystick class looks a bit shite
			break;
		default:;
		}
	}
}

void InputMapper::Map(const int inputId, const InputValue::eInputType type, const int button)
{
	if (m_inputs.find(inputId) != m_inputs.end())
	{
		printf("TRYING TO MAP A VALUE THAT ALREADY EXISTS!\n");
		return;
	}

	m_inputs[inputId] = InputValue(type, button);
}

InputValue InputMapper::GetInputValue(const int inputId) const
{
	return m_inputs.at(inputId);
}
