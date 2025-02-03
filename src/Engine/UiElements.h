#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H
#include <filesystem>
#include <set>
#include <SFML/Graphics/Sprite.hpp>

#include "Gameobject.h"
#include "../Libs/hoxml.h"

class UiElement : public GameObject
{
public:
	enum class eLayer : std::int8_t
	{
		NONE = -1,
		BACKGROUND,
		MIDGROUND,
		FOREGROUND
	};

	UiElement() = default;

	// Exposing Load to callers and inheritors, but this will be the basis for all the UI Elements load from XML using the hoxml
	// library. There are protected Parse functions that will be privated/protected in the children of the UiElement class.
	virtual bool Load(hoxml_context_t*& context, const char* xml, size_t xmlLength);

	eLayer GetLayer() const;
	void SetLayer(eLayer layer);

	std::string GetName() const;

	// TODO: It's getting to the point where a proper renderer class would be nice
	const std::vector<const sf::Drawable*>& GetDrawablesList() const;

protected:
	void SetName(const std::string& name);
	void AddDrawable(const sf::Drawable* drawable);

	virtual bool ParseBeginElement(hoxml_context_t*& context);

	// We return true when the element we're loading has it a closing tag with its name
	// so, for <sprite>, it returns true on </sprite>
	virtual bool ParseEndElement(hoxml_context_t*& context);

	virtual bool ParseAttribute(hoxml_context_t*& context);

private:
	std::string m_name;
	eLayer m_layer;
	std::vector<const sf::Drawable*> m_drawables;
};

class UiSprite : public UiElement
{
public:
	UiSprite() = default;

private:
	sf::Sprite* m_sprite;

	bool ParseEndElement(hoxml_context_t*& context) override;
	bool ParseAttribute(hoxml_context_t*& context) override;
};


class UiManager
{
public:
	static UiManager& Get();

	bool Load(const std::filesystem::path& path);

	// TODO: There has to be a better way to do this? Maybe a renderer class with a vector of sf::drawables in order?
	void RenderForeground(sf::RenderWindow& window) const;
	void RenderMidground(sf::RenderWindow& window) const;
	void RenderBackground(sf::RenderWindow& window) const;

private:
	UiManager() = default;

	std::unordered_map <std::string, UiElement*> m_uiElements;
	std::set<std::string> m_backgroundElements;
	std::set<std::string> m_midgroundElements;
	std::set<std::string> m_foregroundElements;
};

#define UIMANAGER UiManager::Get()

#endif