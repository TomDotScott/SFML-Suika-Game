#include "Globals.h"

RandomRangeGenerator::RandomRangeGenerator(const double min, const double max) :
	m_dist(min, max), m_randomEngine(m_randomDevice())
{
}
