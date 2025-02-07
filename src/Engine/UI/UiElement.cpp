#include "UiElement.h"

bool UiElement::Load(hoxml_context_t*& context, const char* xml, const size_t xmlLength)
{
	// We have come from UiManager::Load so let's assume that we are on a HOXML_ELEMENT_BEGIN at the moment
	hoxml_code_t code = HOXML_ELEMENT_BEGIN;
	do
	{
		if (code == HOXML_ELEMENT_BEGIN)
		{
			ParseBeginElement(context);
		}
		else if (code == HOXML_ELEMENT_END)
		{
			if (ParseEndElement(context))
			{
				return true;
			}
		}
		else if (code == HOXML_ATTRIBUTE)
		{
			ParseAttribute(context);
		}

		code = hoxml_parse(context, xml, xmlLength);
	} while (code != HOXML_END_OF_DOCUMENT);

	return true;
}

UiElement::eLayer UiElement::GetLayer() const
{
	return m_layer;
}

void UiElement::SetLayer(const eLayer layer)
{
	m_layer = layer;
}

std::string UiElement::GetName() const
{
	return m_name;
}

const std::vector<const sf::Drawable*>& UiElement::GetDrawablesList() const
{
	return m_drawables;
}

void UiElement::SetName(const std::string& name)
{
	m_name = name;
}

void UiElement::AddDrawable(const sf::Drawable* drawable)
{
	m_drawables.emplace_back(drawable);
}

bool UiElement::ParseBeginElement(hoxml_context_t*& context)
{
	if (!context)
	{
		return false;
	}

#if BUILD_DEBUG
	printf(" UiElement: Opened <%s>\n", context->tag);
#endif
	return false;
}

bool UiElement::ParseEndElement(hoxml_context_t*& context)
{
	if (!context)
	{
		return false;
	}

#if BUILD_DEBUG
	printf(" UiElement: Closed %s\n", context->tag);
#endif
	return false;
}

bool UiElement::ParseAttribute(hoxml_context_t*& context)
{
	if (!context || !context->attribute || !context->tag || !context->value)
	{
		return false;
	}

#if BUILD_DEBUG
	printf(" UiElement: Attribute \"%s\" of <%s> has value: %s\n", context->attribute, context->tag, context->value);
#endif
	return false;
}
