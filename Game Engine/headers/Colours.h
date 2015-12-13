/*
==================================================================================

==================================================================================
*/

#ifndef _COLOUR_H
#define _COLOUR_H

#include "glm\glm.hpp"

class Colours
{
private:
	glm::vec3 m_ColourList[7];

public:
	enum colours
	{
		RED, YELLOW, GREEN, BLUE, PURPLE, WHITE, GREY
	};

	Colours();
	glm::vec3 getColour(colours colour);
	void setColourList();
};
#endif
