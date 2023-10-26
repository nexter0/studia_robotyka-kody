#include "cell.h"

Cell::Cell(int aX, int aY) : x(aX), y(aY), h(0), g(0), f(0), isObstacle(false), parent(nullptr) {}

double Cell::calculateFValue()
{
	return g + h;
}
