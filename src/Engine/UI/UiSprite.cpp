#include "UiSprite.h"
#include "../Globals.h"
#include "../TextureManager.h"

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

		const std::string textureName = filePath.stem().string();

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
