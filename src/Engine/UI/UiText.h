#ifndef UI_TEXT_H
#define UI_TEXT_H
#include <SFML/Graphics/Text.hpp>

#include "UiElement.h"

class UiText final : public UiElement
{
public:
	UiText();

	template<typename... Args>
	void SetText(const char* fmt, Args... args)
	{
		char buffer[512]{ 0 };

		if (sprintf_s(buffer, fmt, args...) == -1)
		{
			printf(" UiText: Error writing text arguments\n");
			return;
		}

		m_string = &buffer[0];
		m_text.setString(m_string);
	}

	void SetTextSize(unsigned size);

	void SetPosition(const sf::Vector2f& position) override;

private:
	std::string m_string;
	sf::Text m_text;

	bool ParseEndElement(hoxml_context_t*& context) override;
};

#endif
