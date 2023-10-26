#pragma once

struct Cell {

	int x;
	int y;
	double f;
    int g;
    double h;
	bool isObstacle;
	Cell* parent;

    Cell(int aX, int aY);

	double calculateFValue();

};

