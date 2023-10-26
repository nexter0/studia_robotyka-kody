#pragma once
#include <vector>
#include <string>
#include "cell.h"



class PathFinding {
private:
	int gridSize;
	bool isPathCalculated;
	std::string gridStr;
	
	std::vector<std::vector<Cell*>> grid;

	Cell* startCell;
	Cell* endCell;
	
	std::vector<Cell*> openList;
	std::vector<Cell*> closedList;

	double euclideanDistance(int x1, int y1, int x2, int y2);
	Cell* getMinCell();
	void setCellFields(Cell* cell, int x, int y);
	void printInvalidGridShapeError();
	void printGridEmptyError();
	void printPathNotCalculatedError();



	static bool compareCellPointers(const Cell* a, const Cell* b);
	static bool containsCell(const std::vector<Cell*> cellVector, const Cell* targetCell);

public:
	PathFinding(std::string gridStr);

	bool findPath();
	void initializeGrid(std::string gridStr);
	void setGridSize(std::string gridStr);
	void setStartCell(int x, int y);
	void setEndCell(int x, int y);
	void printPath();
	void printSuccessMessage();
	void printFailureMessage();

};