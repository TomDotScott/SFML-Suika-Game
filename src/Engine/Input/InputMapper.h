#ifndef INPUTMAPPER_H
#define INPUTMAPPER_H
#include <cstdint>
#include <unordered_map>
#include "InputValue.h"

class InputMapper
{
public:
	InputMapper();

	void Update();

	void Map(int inputId, eInputType type, int button);

	bool IsButtonDown(int inputID) const;
	bool IsButtonPressed(int inputID) const;
	bool IsButtonReleased(int inputID) const;

	// TODO: Define a set of defaults and read the rest of the values in from an XML file so we can support remapping
	struct GameAction
	{
		GameAction();
		InputValue PrimaryInput;
		InputValue SecondaryInput;
	};

private:
	std::unordered_map<int, GameAction> m_inputs;
};
#endif
