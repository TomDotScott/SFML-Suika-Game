#include "UiText.h"

#include "UiManager.h"
#include "../Globals.h"

UiText::UiText() :
	UiElement(),
	m_text(DEFAULT_FONT)
{
	// All text drawn on top
	// TODO: Make this more sophisticated... I need to be able to support arbitrary placement of elements in the XML otherwise we are at the mercy of std::hash!
	SetLayer(eLayer::FOREGROUND);
}

void UiText::SetTextSize(const unsigned size)
{
	m_text.setCharacterSize(size);
}

void UiText::SetPosition(const sf::Vector2f& position)
{
	GameObject::SetPosition(position);
	m_text.setPosition(position);
}

bool UiText::ParseEndElement(hoxml_context_t*& context)
{
	if (!context)
	{
		return false;
	}

	if (context->tag && strcmp("Text", context->tag) == 0)
	{
		if (!m_string.empty())
		{
			printf(" Closing tag found and no text was assigned to %s!\n", GetName().c_str());
			return false;
		}

		m_text.setPosition(m_position);

		AddDrawable(&m_text);

		// When we have finished assigning the sprite, we can return true!
		return true;
	}

	if (context->content == nullptr || OnlyWhitespace(context->content))
	{
		return false;
	}

	if (strcmp("string", context->tag) == 0)
	{
		m_text.setString(context->content);
		return false;
	}

	if (strcmp("colour", context->tag) == 0)
	{
		char* p;
		const unsigned long n = strtoul(context->content, &p, 16);
		if (*p != 0)
		{
			printf(" ERROR: BAD COLOUR INPUT %s\n", context->content);
			return false;
		}

		m_text.setFillColor(static_cast<sf::Color>(n));

		return false;
	}

	if (strcmp("outline", context->tag) == 0)
	{
		// TODO...
	}

	if (strcmp("size", context->tag) == 0)
	{
		m_text.setCharacterSize(TRANSFORMED_SCALAR(std::stol(context->content)));
		return false;
	}

	if (strcmp("font", context->tag) == 0)
	{
		// TODO: FontManager?
		const sf::Font* font = UIMANAGER.GetFont(context->content);
		if (!font)
		{
			printf(" UiText: Unknown font %s on line %u\n", context->content, context->line);
			return false;
		}

		m_text.setFont(*font);
		return false;
	}

	return UiElement::ParseEndElement(context);
}
