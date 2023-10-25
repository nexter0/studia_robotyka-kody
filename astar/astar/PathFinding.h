#pragma once
#include <vector>
#include <string>
#include "cell.h"



class PathFinding {
private:
	int _gridSize;
	std::string _gridStr;
	
	std::vector<std::vector<Cell*>> _grid;

	Cell* _startCell;
	Cell* _endCell;
	
	std::vector<Cell*> _openList;
	std::vector<Cell*> _closedList;


	double EuclideanDistance(int x1, int y1, int x2, int y2);
	void SetCellFields(Cell* cell, int x, int y);
	Cell* GetMinValueCell();
	static bool compareCellPointers(const Cell* a, const Cell* b);
	static bool containsCell(const std::vector<Cell*> cellVector, const Cell* targetCell);

public:

	PathFinding(std::string gridStr);

	int SetGridSize(std::string gridStr);
	int SetGridSize(int gridSize);
	void SetStartCell(int x, int y);
	void SetEndCell(int x, int y);
	void InitializeGrid(std::string gridStr);
	bool FindPath();
	void PrintPath(std::string gridStr);

};