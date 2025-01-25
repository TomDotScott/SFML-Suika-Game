#include "../Engine/Globals.h"
#include "FruitManager.h"

#include <fstream>
#include <string>
#define HOXML_IMPLEMENTATION
#include "../Libs/hoxml.h"

FruitManager* FruitManager::Get()
{
	static FruitManager* manager = nullptr;

	if (manager == nullptr)
	{
		manager = new FruitManager();

		if (!manager->Init())
		{
			delete manager;
			manager = nullptr;
		}
	}

	return manager;
}

bool FruitManager::Init()
{
	std::string line, text;
	std::ifstream in("fruitinfo.xml");
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

	hoxml_context_t hoxml_context[1];
	auto buffer = static_cast<char*>(malloc(content_length * 2));

	hoxml_init(hoxml_context, buffer, content_length * 2);

	const static Details EMPTY = { INVALID, "NULL", sf::Color::Magenta, -1.f, 0 };

	Details currentDetails = EMPTY;

	// Loop until the "end of document" code is returned
	hoxml_code_t code = hoxml_parse(hoxml_context, content, content_length);
	while (code != HOXML_END_OF_DOCUMENT)
	{
		switch (code)
		{
		case HOXML_ELEMENT_BEGIN:

			printf(" Opened <%s>\n", hoxml_context->tag);
			break;
		case HOXML_ELEMENT_END:
			if (hoxml_context->tag != nullptr)
			{
				if (strcmp("Fruit", hoxml_context->tag) == 0)
				{
					m_fruitDetails[currentDetails.m_Type] = currentDetails;

					currentDetails = EMPTY;
				}

				printf(" Closed %s\n", hoxml_context->tag);
			}

			if (hoxml_context->content != nullptr)
			{
				/* Check the content to see if it only contains whitespace */
				bool isEmpty = true;
				const char* character = hoxml_context->content;

				while (*character != '\0')
				{
					// If not a whitespace or newline
					if (*character != ' ' && *character != '\t' && *character != '\n' && *character != '\r')
					{
						isEmpty = false;
						break;
					}
					character++;
				}

				// If the content string contains more than just whitespace, we want to parse it
				if (!isEmpty)
				{
					if (strcmp("enum_value", hoxml_context->tag) == 0)
					{
						currentDetails.m_Type = static_cast<eType>(atoi(hoxml_context->content));
					}
					else if (strcmp("name", hoxml_context->tag) == 0)
					{
						currentDetails.m_Name = hoxml_context->content;
					}
					else if (strcmp("texture", hoxml_context->tag) == 0)
					{
						// TODO
					}
					else if (strcmp("colour", hoxml_context->tag) == 0)
					{
						char* p;
						unsigned long n = strtoul(hoxml_context->content, &p, 16);
						if (*p != 0)
						{
							printf(" ERROR: BAD COLOUR INPUT %s\n", hoxml_context->content);
							return false;
						}

						currentDetails.m_Colour = static_cast<sf::Color>(n);
					}
					else if (strcmp("radius", hoxml_context->tag) == 0)
					{
						currentDetails.m_Radius = std::stof(hoxml_context->content);
					}
					else if (strcmp("points", hoxml_context->tag) == 0)
					{
						currentDetails.m_Points = static_cast<unsigned>(atoi(hoxml_context->content));
					}

					printf(" Closed <%s> with content \"%s\"\n", hoxml_context->tag, hoxml_context->content);
				}
			}
			break;
		case HOXML_ATTRIBUTE:
			printf(" Attribute \"%s\" of <%s> has value: %s\n", hoxml_context->attribute, hoxml_context->tag,
				hoxml_context->value);
			break;
		}

		code = hoxml_parse(hoxml_context, content, content_length);
	}

	free(buffer);
	return true;
}

const FruitManager::Details& FruitManager::GetFruitDetails(const eType type) const
{
	return m_fruitDetails.at(type);
}

FruitManager::eType FruitManager::GenerateRandomType()
{
	return static_cast<eType>(static_cast<int>(RNG.Next() * static_cast<double>(FRUIT_TYPE_Apple)));
}
