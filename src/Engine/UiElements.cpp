#include "UiElements.h"

#include <fstream>
#include <SFML/Graphics/Font.hpp>

#include "Globals.h"
#include "TextureManager.h"

namespace
{
	bool OnlyWhitespace(const char* chr)
	{
		while (*chr != '\0')
		{
			// If not a whitespace or newline
			if (*chr != ' ' && *chr != '\t' && *chr != '\n' && *chr != '\r')
			{
				return false;
			}
			chr++;
		}
		return true;
	}

	// I don't like how this is needed for sf::Text to compile
	sf::Font defaultFont("fonts/FiraCode-Regular.ttf");
}

bool UiElement::Load(hoxml_context_t*& context, const char* xml, const size_t xmlLength)
{
	// We have come from UiManager load so let's assume that we are on a begin element at the moment
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

void UiSprite::SetPosition(const sf::Vector2f& position)
{
	GameObject::SetPosition(position);

	m_sprite->setPosition(m_position);
}

bool UiSprite::ParseEndElement(hoxml_context_t*& context)
{
	if (!context)
	{
		return false;
	}

	if (context->tag && strcmp("Sprite", context->tag) == 0)
	{
		if (!m_sprite)
		{
			printf(" Closing tag found and no sprite was assigned to %s!\n", GetName().c_str());
			return false;
		}
		m_sprite->setPosition(m_position);

		AddDrawable(m_sprite);

		// When we have finished assigning the sprite, we can return true!
		return true;
	}

	if (context->content == nullptr || OnlyWhitespace(context->content))
	{
		return false;
	}

	if (strcmp("name", context->tag) == 0)
	{
		SetName(context->content);
		return false;
	}

	if (strcmp("layer", context->tag) == 0)
	{
		if (strcmp("foreground", context->content) == 0)
		{
			SetLayer(eLayer::FOREGROUND);
		}
		else if (strcmp("midground", context->content) == 0)
		{
			SetLayer(eLayer::MIDGROUND);
		}
		else if (strcmp("background", context->content) == 0)
		{
			SetLayer(eLayer::BACKGROUND);
		}
		else
		{
			printf(" Unknown layer: %s!\n", context->content);
		}

		return false;
	}

	if (strcmp("texture", context->tag) == 0)
	{
		const std::filesystem::path filePath = std::filesystem::u8path(context->content);

		// Grab the texture name
		if (!std::filesystem::exists(filePath))
		{
			printf(" Texture: %s does not exist!\n", context->content);
			return false;
		}

		const std::string textureName = filePath.filename().string();

		// Load the texture into the TextureManager
		if (!TEXTUREMANAGER.LoadTexture(textureName, filePath))
		{
			printf(" Sprite: Failed to load texture: %s", context->content);
			return false;
		}

		const sf::Texture* texture = TEXTUREMANAGER.GetTexture(textureName);

		const sf::Vector2f scaleFactor = {
			static_cast<float>(texture->getSize().x) / TRANSFORMED_SCALAR(texture->getSize().x),
			static_cast<float>(texture->getSize().y) / TRANSFORMED_SCALAR(texture->getSize().y)
		};

		m_sprite = new sf::Sprite(*texture);
		m_sprite->setScale({ 1.f / scaleFactor.x, 1.f / scaleFactor.y });

		return false;
	}

	return UiElement::ParseEndElement(context);
}

bool UiSprite::ParseAttribute(hoxml_context_t*& context)
{
	if (strcmp("position", context->tag) == 0)
	{
		if (strcmp("x", context->attribute) == 0)
		{
			m_position.x = TRANSFORMED_SCALAR(std::stof(context->value));
			return true;
		}

		if (strcmp("y", context->attribute) == 0)
		{
			m_position.y = TRANSFORMED_SCALAR(std::stof(context->value));
			return true;
		}

		printf(" Sprite: Unknown parameter in <position/>\n");
		return false;
	}

	printf(" Sprite: Unknown attribute %s\n", context->attribute);

	return UiElement::ParseAttribute(context);
}

UiText::UiText() :
	UiElement(),
	m_text(defaultFont)
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

	if (strcmp("name", context->tag) == 0)
	{
		SetName(context->content);
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

bool UiText::ParseAttribute(hoxml_context_t*& context)
{
	if (strcmp("position", context->tag) == 0)
	{
		if (strcmp("x", context->attribute) == 0)
		{
			m_position.x = TRANSFORMED_SCALAR(std::stof(context->value));
			return true;
		}

		if (strcmp("y", context->attribute) == 0)
		{
			m_position.y = TRANSFORMED_SCALAR(std::stof(context->value));
			return true;
		}

		printf(" UiText: Unknown parameter in <position/>\n");
		return false;
	}

	printf(" UiText: Unknown attribute %s\n", context->attribute);

	return UiElement::ParseAttribute(context);
}

UiText* UiManager::GetUiText(const std::string& name) const
{
	if (m_uiElements.find(name) == m_uiElements.end())
	{
		return nullptr;
	}

	return static_cast<UiText*>(m_uiElements.at(name));
}

#if !BUILD_MASTER
void UiManager::DrawDebugText(sf::RenderWindow& window) const
{
	for (const auto& text : GetUiText("DEBUG_TEXT")->GetDrawablesList())
	{
		window.draw(*text);
	}
}
#endif

UiManager& UiManager::Get()
{
	static UiManager* uiManager = nullptr;
	if (uiManager == nullptr)
	{
		uiManager = new UiManager();
	}

	return *uiManager;
}

bool UiManager::Load(const std::filesystem::path& path)
{
	std::string line, text;
	std::ifstream in(path);
	while (std::getline(in, line))
	{
		text += line + "\n";
	}

	if (text.empty())
	{
		return false;
	}

	const char* content = text.c_str();

	const size_t content_length = strlen(content);

	hoxml_context_t* hoxml_context = new hoxml_context_t();
	const auto buffer = static_cast<char*>(malloc(content_length * 2));

	hoxml_init(hoxml_context, buffer, content_length * 2);

	// Loop until the "end of document" code is returned
	hoxml_code_t code = hoxml_parse(hoxml_context, content, content_length);
	while (code != HOXML_END_OF_DOCUMENT)
	{
		if (code == HOXML_ELEMENT_BEGIN)
		{
#if BUILD_DEBUG
			printf(" Opened <%s>\n", hoxml_context->tag);
#endif

			if (strcmp("Font", hoxml_context->tag) == 0)
			{
				LoadFont(hoxml_context, content, content_length);
			}
			else
			{
				LoadElement(hoxml_context, content, content_length);
			}
		}
		if (code == HOXML_ERROR_TAG_MISMATCH)
		{
			printf(" UiManager: Start tag did not match end tag on line %u\n", hoxml_context->line);
			return false;
		}

		code = hoxml_parse(hoxml_context, content, content_length);
	}

	free(buffer);
	return true;
}

bool UiManager::LoadElement(hoxml_context_t*& context, const char* xml, const size_t xmlLength)
{
	UiElement* currentElement = nullptr;

	if (strcmp("Sprite", context->tag) == 0)
	{
		currentElement = new UiSprite();
	}
	else if (strcmp("Text", context->tag) == 0)
	{
		currentElement = new UiText();
	}
	else
	{
		// We only care about our specific tags...
		return false;
	}

	if (!currentElement)
	{
		return false;
	}

	if (!currentElement->Load(context, xml, xmlLength))
	{
		// TODO: Proper cleanup
		delete currentElement;

		printf(" Error reading xml!");
		return false;
	}

	if (m_uiElements.find(currentElement->GetName()) != m_uiElements.end())
	{
		delete currentElement;

		printf(" UiManager: UI Element with name %s already exists!\n", currentElement->GetName().c_str());
		return false;
	}

	m_uiElements[currentElement->GetName()] = currentElement;

	switch (currentElement->GetLayer()) {
	case UiElement::eLayer::NONE:
		// TODO: Proper cleanup
		delete currentElement;

		printf(" UiManager: No layer set for UiElement %s\n", currentElement->GetName().c_str());
		return false;

	case UiElement::eLayer::BACKGROUND:
		m_backgroundElements.insert(currentElement->GetName());
		break;
	case UiElement::eLayer::MIDGROUND:
		m_midgroundElements.insert(currentElement->GetName());
		break;
	case UiElement::eLayer::FOREGROUND:
		m_foregroundElements.insert(currentElement->GetName());
		break;
	}

	return true;
}

bool UiManager::LoadFont(hoxml_context_t*& context, const char* xml, const size_t xmlLength)
{
	hoxml_code_t code = HOXML_ELEMENT_BEGIN;

	std::string name;
	std::filesystem::path path;

	do
	{
		if (code == HOXML_ELEMENT_END)
		{
			if (strcmp("name", context->tag) == 0)
			{
				name = context->content;
			}
			else if (strcmp("path", context->tag) == 0)
			{
				path = context->content;
			}

			if (strcmp("Font", context->tag) != 0)
			{
				code = hoxml_parse(context, xml, xmlLength);
				continue;
			}

			if (name.empty())
			{
				printf(" UiManager::LoadFont: Trying to load a font with no name tag on line %u.\n", context->line);
				return false;
			}
			if (path.empty())
			{
				printf(" UiManager::LoadFont: Trying to load font %s with no path tag on line %u.\n", name.c_str(), context->line);
				return false;
			}

			if (m_fonts.find(name) == m_fonts.end())
			{
#if BUILD_DEBUG
				printf(" UiManager::LoadFont: Loading font \"%s\", path=\"%ls\"\n", name.c_str(), path.c_str());
#endif
				sf::Font* font = new sf::Font(path);

				m_fonts[name] = font;
			}

			// Reached the end!
			return true;
		}

		code = hoxml_parse(context, xml, xmlLength);
	} while (code != HOXML_END_OF_DOCUMENT);

	return false;
}

void UiManager::RenderForeground(sf::RenderWindow& window) const
{
	for (const auto& elementName : m_foregroundElements)
	{
		for (const auto* drawable : m_uiElements.at(elementName)->GetDrawablesList())
		{
			window.draw(*drawable);
		}
	}
}

void UiManager::RenderMidground(sf::RenderWindow& window) const
{
	for (const auto& elementName : m_midgroundElements)
	{
		for (const auto* drawable : m_uiElements.at(elementName)->GetDrawablesList())
		{
			window.draw(*drawable);
		}
	}
}

void UiManager::RenderBackground(sf::RenderWindow& window) const
{
	for (const auto& elementName : m_backgroundElements)
	{
		for (const auto* drawable : m_uiElements.at(elementName)->GetDrawablesList())
		{
			window.draw(*drawable);
		}
	}
}

const sf::Font* UiManager::GetFont(const std::string& name) const
{
	if (m_fonts.find(name) == m_fonts.end())
	{
		return nullptr;
	}
	return m_fonts.at(name);
}
