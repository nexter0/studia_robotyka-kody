#include "cell.h"

Cell::Cell() : x(0), y(0), h(0), g(0), f(g + h), isObstacle(false), parent(nullptr) {}

Cell::Cell(int aX, int aY) : x(aX), y(aY), h(0), g(0), f(g + h), isObstacle(false), parent(nullptr) {}

Cell::Cell(int aX, int aY, bool aIsObstacle, int aG, int aH, Cell* aParent) : x(aX), y(aY), h(0), g(0), f(g + h), isObstacle(aIsObstacle), parent(aParent) {}

double Cell::CalculateValue()
{
	return g + h;
}
