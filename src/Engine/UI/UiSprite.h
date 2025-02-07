#ifndef UI_SPRITE_H
#define UI_SPRITE_H
#include <SFML/Graphics/Sprite.hpp>

#include "UiElement.h"

class UiSprite : public UiElement
{
public:
	UiSprite() = default;

	void SetPosition(const sf::Vector2f& position) override;

private:
	sf::Sprite* m_sprite;

	bool ParseEndElement(hoxml_context_t*& context) override;
	bool ParseAttribute(hoxml_context_t*& context) override;
};

#endif