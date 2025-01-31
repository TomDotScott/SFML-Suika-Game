#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H
#include <filesystem>
#include <set>
#include <SFML/Graphics/Sprite.hpp>

#include "Gameobject.h"
#include "../Libs/hoxml.h"

class UiElement : public GameObject
{
	// TODO: This is a code smell, but I need to figure out what the best way to do this is and this works for now
	friend class UiManager;
public:
	enum class eLayer : std::int8_t
	{
		NONE = -1,
		BACKGROUND,
		MIDGROUND,
		FOREGROUND
	};

	UiElement() = default;
	virtual bool Load(hoxml_context_t*& context, const char* xml, size_t xml_length) = 0;

	eLayer GetLayer() const;
	void SetLayer(eLayer layer);

	std::string GetName() const;

protected:
	void SetName(const std::string& name);
	void AddDrawable(const sf::Drawable* drawable);

private:
	std::string m_name;
	eLayer m_layer;
	std::vector<const sf::Drawable*> m_drawables;
};

class UiSprite : public UiElement
{
public:
	UiSprite() = default;

	bool Load(hoxml_context_t*& context, const char* xml, size_t xml_length) override;

private:
	sf::Sprite* m_sprite;
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