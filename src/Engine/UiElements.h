#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H
#include <filesystem>
#include <set>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

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

	void SetPosition(const sf::Vector2f& position) override;

private:
	sf::Sprite* m_sprite;

	bool ParseEndElement(hoxml_context_t*& context) override;
	bool ParseAttribute(hoxml_context_t*& context) override;
};

class UiText final : public UiElement
{
public:
	UiText();

	template<typename... Args>
	void SetText(const char* fmt, Args... args)
	{
		char buffer[512]{ 0 };

		if (sprintf_s(buffer, fmt, args...) == -1)
		{
			printf(" UiText: Error writing text arguments\n");
			return;
		}

		m_string = &buffer[0];
		m_text.setString(m_string);
	}

	void SetTextSize(unsigned size);

	void SetPosition(const sf::Vector2f& position) override;

private:
	std::string m_string;
	sf::Text m_text;

	bool ParseEndElement(hoxml_context_t*& context) override;
	bool ParseAttribute(hoxml_context_t*& context) override;
};

//class UiPanel : public UiElement
//{
//public:
//	bool Load(hoxml_context_t*& context, const char* xml, size_t xml_length) override;
//
//private:
//	UiText m_text;
//	UiSprite m_sprite;
//};

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