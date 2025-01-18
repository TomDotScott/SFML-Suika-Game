#ifndef INPUTMAPPER_H
#define INPUTMAPPER_H
#include <cstdint>
#include <unordered_map>

class InputValue
{
	friend class InputMapper;

public:
	enum class eInputType : uint8_t
	{
		Keyboard = 0,
		Mouse,
		// TODO...
		Controller
	};

	// TODO: Think of a better way to do this... This constructor is only used for the unordered_map
	InputValue() = default;
	InputValue(eInputType type, int button);
	uint8_t GetValue() const;

	// True the first frame the button is pressed
	bool IsPressed() const;

	// True whilst the button is pressed
	bool IsDown() const;

	// True the frame the button is released
	bool IsReleased() const;

private:
	eInputType m_type;
	int m_button;

	uint8_t m_value;
	uint8_t m_previousValue;
};

class InputMapper
{
public:
	InputMapper();

	void Update();

	void Map(int inputId, InputValue::eInputType type, int button);

	InputValue GetInputValue(int inputId) const;

private:
	std::unordered_map<int, InputValue> m_inputs;
};
#endif
