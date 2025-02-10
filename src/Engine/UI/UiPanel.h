#ifndef UI_PANEL_H
#define UI_PANEL_H

#include "UiElement.h"
#include "UiSprite.h"
#include "UiText.h"

class UiPanel : public UiElement
{
public:
	UiPanel();

	void SetPosition(const sf::Vector2f& position) override;

	UiText* GetUiText(const std::string& name);

private:
	UiSprite* m_sprite;

	struct OffsetText
	{
		UiText* m_text;
		sf::Vector2f m_offset;
	};

	std::vector<OffsetText> m_text;

	bool ParseBeginElement(hoxml_context_t*& context) override;
	bool ParseEndElement(hoxml_context_t*& context) override;
};

#endif