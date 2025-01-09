#ifndef GLOBALS_H
#define GLOBALS_H
#include <random>
#include <SFML/System/Vector2.hpp>

class GraphicSettings
{
public:
	struct ScreenDetails
	{
		sf::Vector2u m_ScreenSize;
		sf::Vector2u m_ScreenCentre;
	};

	void SetScreenSize(const sf::Vector2u& screenSize) { m_screenDetails = { screenSize, screenSize / 2u }; }

	const ScreenDetails& GetScreenDetails() const { return m_screenDetails; }
	ScreenDetails GetScreenDetails() { return m_screenDetails; }

private:
	ScreenDetails m_screenDetails;
};

class RandomRangeGenerator
{
public:
	RandomRangeGenerator(double min, double max);

	double Next() { return m_dist(m_randomEngine); }

private:
	std::uniform_real_distribution<double> m_dist;
	std::random_device m_randomDevice;
	std::default_random_engine m_randomEngine;
};

extern GraphicSettings GRAPHIC_SETTINGS;
extern RandomRangeGenerator RNG;

#endif
