#include "UiElements.h"

#include <fstream>

#include "Globals.h"
#include "TextureManager.h"

static bool OnlyWhitespace(const char* chr)
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

void UiElement::SetName(const std::string& name)
{
	m_name = name;
}

void UiElement::AddDrawable(const sf::Drawable* drawable)
{
	m_drawables.emplace_back(drawable);
}

bool UiSprite::Load(hoxml_context_t*& context, const char* xml, const size_t xml_length)
{
	// We have come from UiManager load so let's assume that we are on a begin element at the moment
	// TODO: If I want to make this reusable, probably make this more robust somehow...
	hoxml_code_t code = HOXML_ELEMENT_BEGIN;
	do
	{
		if (code == HOXML_ELEMENT_BEGIN)
		{
#if BUILD_DEBUG
			printf(" Sprite: Opened <%s>\n", context->tag);
#endif
		}
		else if (code == HOXML_ELEMENT_END)
		{
#if BUILD_DEBUG
			printf(" Closed %s\n", context->tag);
#endif
			if (context->tag != nullptr && strcmp("Sprite", context->tag) == 0)
			{
				if (!m_sprite)
				{
					printf(" Closing tag found and no sprite was assigned to %s!\n", GetName().c_str());
					return false;
				}
				m_sprite->setPosition(m_position);

				AddDrawable(m_sprite);

				return true;
			}

			if (context->content != nullptr && !OnlyWhitespace(context->content))
			{
				if (strcmp("name", context->tag) == 0)
				{
					SetName(context->content);
				}
				else if (strcmp("layer", context->tag) == 0)
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
						return false;
					}
				}
				else if (strcmp("texture", context->tag) == 0)
				{
					std::filesystem::path filePath = std::filesystem::u8path(context->content);

					// Grab the texture name
					if (!std::filesystem::exists(filePath))
					{
						printf(" Texture: %s does not exist!\n", context->content);
						return false;
					}

					std::string textureName = filePath.filename().string();

					// Load the texture into the TextureManager
					if (!TEXTUREMANAGER.LoadTexture(textureName, filePath))
					{
						printf("Failed to load texture: %s", context->content);
						return false;
					}

					const sf::Texture* texture = TEXTUREMANAGER.GetTexture(textureName);

					const sf::Vector2f scaleFactor = {
						static_cast<float>(texture->getSize().x) / TRANSFORMED_SCALAR(texture->getSize().x),
						static_cast<float>(texture->getSize().y) / TRANSFORMED_SCALAR(texture->getSize().y)
					};

					m_sprite = new sf::Sprite(*texture);
					m_sprite->setScale({ 1.f / scaleFactor.x, 1.f / scaleFactor.y });
				}
				printf(" Closed <%s> with content \"%s\"\n", context->tag, context->content);
			}
		}
		else if (code == HOXML_ATTRIBUTE)
		{
			if (strcmp("position", context->tag) == 0)
			{
				if (strcmp("x", context->attribute) == 0)
				{
					m_position.x = TRANSFORMED_SCALAR(std::stof(context->value));
				}
				else if (strcmp("y", context->attribute) == 0)
				{
					m_position.y = TRANSFORMED_SCALAR(std::stof(context->value));
				}
			}

#if BUILD_DEBUG
			printf(" Attribute \"%s\" of <%s> has value: %s\n", context->attribute, context->tag, context->value);
#endif
		}

		code = hoxml_parse(context, xml, xml_length);
	} while (code != HOXML_END_OF_DOCUMENT);

	printf(" Sprite: %s block hit the end of the document before fully generating! Check the xml and try again\n", GetName().c_str());
	return false;
}

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

	size_t content_length = strlen(content);

	hoxml_context_t* hoxml_context = new hoxml_context_t();
	auto buffer = static_cast<char*>(malloc(content_length * 2));

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
			UiElement* currentElement;

			if (strcmp("Sprite", hoxml_context->tag) == 0)
			{
				currentElement = new UiSprite();
			}
			else if (strcmp("Text", hoxml_context->tag) == 0)
			{
				currentElement = new UiText();
			}
			else if (strcmp("Panel", hoxml_context->tag) == 0)
			{
				currentElement = new UiPanel();
			}
			else
			{
				// We only care about our specific tags...
				code = hoxml_parse(hoxml_context, content, content_length);
				continue;
			}

			if (!currentElement->Load(hoxml_context, content, content_length))
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
		}
		code = hoxml_parse(hoxml_context, content, content_length);
	}

	free(buffer);
	return true;
}

void UiManager::RenderForeground(sf::RenderWindow& window) const
{
	for (const auto& elementName : m_foregroundElements)
	{
		for (const auto* drawable : m_uiElements.at(elementName)->m_drawables)
		{
			window.draw(*drawable);
		}
	}
}

void UiManager::RenderMidground(sf::RenderWindow& window) const
{
	for (const auto& elementName : m_midgroundElements)
	{
		for (const auto* drawable : m_uiElements.at(elementName)->m_drawables)
		{
			window.draw(*drawable);
		}
	}
}

void UiManager::RenderBackground(sf::RenderWindow& window) const
{
	for (const auto& elementName : m_backgroundElements)
	{
		for (const auto* drawable : m_uiElements.at(elementName)->m_drawables)
		{
			window.draw(*drawable);
		}
	}
}
