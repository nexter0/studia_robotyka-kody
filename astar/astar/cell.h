#pragma once

struct Cell {

	int x;
	int y;
	double f;
    int g;
    double h;
	bool isObstacle;
	Cell* parent;

	Cell();
    Cell(int aX, int aY);
	Cell(int aX, int aY, bool aIsObstacle, int aG, int aH, Cell* aParent);

	double CalculateValue();

};

