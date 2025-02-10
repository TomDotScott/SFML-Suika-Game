#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H
#include <filesystem>
#include "../Gameobject.h"
#include "../../Libs/hoxml.h"

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

	UiElement();

	// Exposing Load to callers and inheritors, but this will be the basis for all the UI Elements load from XML using the hoxml
	// library. There are protected Parse functions that will be privated/protected in the children of the UiElement class.
	virtual bool Load(hoxml_context_t*& context, const char* xml, size_t xmlLength);

	eLayer GetLayer() const;
	void SetLayer(eLayer layer);

	std::string GetName() const;

	// TODO: It's getting to the point where a proper renderer class would be nice
	const std::vector<const sf::Drawable*>& GetDrawablesList() const;

protected:
	void AddDrawable(const sf::Drawable* drawable);

	virtual bool ParseBeginElement(hoxml_context_t*& context);

	// We return true when the element we're loading has hit a closing tag with its name
	// so, for <sprite>, it returns true on </sprite>
	virtual bool ParseEndElement(hoxml_context_t*& context);

	virtual bool ParseAttribute(hoxml_context_t*& context);

	static bool ParseVectorAttribute(hoxml_context_t*& context, sf::Vector2f& vector);

private:
	std::string m_name;
	eLayer m_layer;
	std::vector<const sf::Drawable*> m_drawables;
};
#endif
