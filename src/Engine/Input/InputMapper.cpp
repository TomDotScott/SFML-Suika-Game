#include "InputMapper.h"
#include "Keyboard.h"
#include "Mouse.h"

InputMapper::GameAction::GameAction() :
	PrimaryInput(eInputType::NONE, UNASSIGNED_BUTTON),
	SecondaryInput(eInputType::NONE, UNASSIGNED_BUTTON)
{
}

InputMapper::InputMapper() = default;

void InputMapper::Update()
{
	auto updateInputValue = [](InputValue& inputValue) {
		inputValue.m_previousValue = inputValue.m_value;

		switch (inputValue.GetType())
		{
		case eInputType::Keyboard:
			inputValue.m_value = Keyboard::Get().IsButtonDown(static_cast<sf::Keyboard::Key>(inputValue.GetButton())) ? 0xFF : 0x00;
			break;
		case eInputType::Mouse:
			inputValue.m_value = Mouse::Get().IsButtonDown(static_cast<sf::Mouse::Button>(inputValue.GetButton())) ? 0xFF : 0x00;
			break;
		case eInputType::Controller:
			// TODO: Maybe xInput? The sfml joystick class looks a bit shite
			break;
		case eInputType::NONE:
		default:;
			break;
		}
		};

	for (auto& [ID, inputValue] : m_inputs)
	{
		updateInputValue(inputValue.PrimaryInput);
		updateInputValue(inputValue.SecondaryInput);
	}
}

// TODO: Support for remapping of keys
void InputMapper::Map(const int inputId, const eInputType type, const int button)
{
	// If it already exists, we want to map to the secondary input
	if (m_inputs.find(inputId) != m_inputs.end())
	{
		m_inputs.at(inputId).SecondaryInput = InputValue(type, button);
	}
	else
	{
		// Otherwise, we want to initialise it and assign the primary
		m_inputs[inputId] = GameAction();

		m_inputs.at(inputId).PrimaryInput = InputValue(type, button);
	}
}

bool InputMapper::IsButtonDown(const int inputID) const
{
	const InputValue& primary = m_inputs.at(inputID).PrimaryInput;
	const bool primaryDown = primary.GetType() != eInputType::NONE && primary.IsDown();

	const InputValue& secondary = m_inputs.at(inputID).SecondaryInput;
	const bool secondaryDown = secondary.GetType() != eInputType::NONE && secondary.IsDown();

	return primaryDown || secondaryDown;
}

bool InputMapper::IsButtonPressed(const int inputID) const
{
	const InputValue& primary = m_inputs.at(inputID).PrimaryInput;
	const bool primaryPressed = primary.GetType() != eInputType::NONE && primary.IsPressed();

	const InputValue& secondary = m_inputs.at(inputID).SecondaryInput;
	const bool secondaryPressed = secondary.GetType() != eInputType::NONE && secondary.IsPressed();

	return primaryPressed || secondaryPressed;
}

bool InputMapper::IsButtonReleased(const int inputID) const
{
	const InputValue& primary = m_inputs.at(inputID).PrimaryInput;
	const bool primaryReleased = primary.GetType() != eInputType::NONE && primary.IsReleased();

	const InputValue& secondary = m_inputs.at(inputID).SecondaryInput;
	const bool secondaryReleased = secondary.GetType() != eInputType::NONE && secondary.IsReleased();

	return primaryReleased || secondaryReleased;
}
