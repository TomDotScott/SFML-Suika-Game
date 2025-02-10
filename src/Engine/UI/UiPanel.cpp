#include "UiPanel.h"

#include "UiManager.h"
#include "../Globals.h"

UiPanel::UiPanel() :
	m_sprite(nullptr)
{
	SetLayer(eLayer::MIDGROUND);
}

void UiPanel::SetPosition(const sf::Vector2f& position)
{
	UiElement::SetPosition(position);

	m_sprite->SetPosition(m_position);

	for (const auto& [text, offset] : m_text)
	{
		text->SetPosition(m_position + offset);
	}
}

UiText* UiPanel::GetUiText(const std::string& name)
{
	for (auto& [uiText, offsetPosition] : m_text)
	{
		if (uiText->GetName() == name)
		{
			return uiText;
		}
	}
	return nullptr;
}

bool UiPanel::ParseBeginElement(hoxml_context_t*& context)
{
	auto [xmlText, xmlLength] = UIMANAGER.GetLastXmlDetails();

	if (strcmp("Sprite", context->tag) == 0)
	{
		m_sprite = new UiSprite();
		if (!m_sprite->Load(context, xmlText, xmlLength))
		{
			printf(" UiPanel: Error loading sprite on line %u\n", context->line);
			return true;
		}
	}
	else if (strcmp("Text", context->tag) == 0)
	{
		OffsetText& newText = m_text.emplace_back();

		newText.m_text = new UiText();

		if (!newText.m_text->Load(context, xmlText, xmlLength))
		{
			printf(" UiPanel: Error loading Text on line %u\n", context->line);
			return true;
		}

		newText.m_offset = newText.m_text->GetPosition();
	}

	return UiElement::ParseBeginElement(context);
}

bool UiPanel::ParseEndElement(hoxml_context_t*& context)
{
	if (strcmp("Panel", context->tag) == 0)
	{
		SetPosition(m_position);

		for (const sf::Drawable* drawable : m_sprite->GetDrawablesList())
		{
			AddDrawable(drawable);
		}

		for (const auto& [text, offset] : m_text)
		{
			for (const sf::Drawable* drawable : text->GetDrawablesList())
			{
				AddDrawable(drawable);
			}
		}

		return true;
	}

	return UiElement::ParseEndElement(context);
}
