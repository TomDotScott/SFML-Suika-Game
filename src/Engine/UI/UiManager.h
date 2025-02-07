#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include <filesystem>
#include <set>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "UiText.h"
#include "../../Libs/hoxml.h"

class UiManager
{
public:
	static UiManager& Get();

	bool Load(const std::filesystem::path& path);

	// TODO: There has to be a better way to do this? Maybe a renderer class with a vector of sf::drawables in order?
	void RenderForeground(sf::RenderWindow& window) const;
	void RenderMidground(sf::RenderWindow& window) const;
	void RenderBackground(sf::RenderWindow& window) const;

	const sf::Font* GetFont(const std::string& name) const;

	UiText* GetUiText(const std::string& name) const;

#if !BUILD_MASTER
	void DrawDebugText(sf::RenderWindow& window) const;
#endif

private:
	UiManager() = default;

	std::unordered_map<std::string, sf::Font*> m_fonts;

	std::unordered_map <std::string, UiElement*> m_uiElements;
	std::set<std::string> m_backgroundElements;
	std::set<std::string> m_midgroundElements;
	std::set<std::string> m_foregroundElements;

	bool LoadElement(hoxml_context_t*& context, const char* xml, size_t xmlLength);
	bool LoadFont(hoxml_context_t*& context, const char* xml, size_t xmlLength);
};

#define UIMANAGER UiManager::Get()

#endif