#include "PathFinding.h"
#include <sstream>
#include <cmath>
#include <iostream>
#include <algorithm>

PathFinding::PathFinding(std::string gridStr)
{
    this->gridStr = gridStr;
    this->startCell = nullptr;
    this->endCell = nullptr;
    this->isPathCalculated = false;

    setGridSize(this->gridStr);
    initializeGrid(this->gridStr);
}

void PathFinding::setGridSize(std::string gridStr)
{
    std::vector<std::string> lines;
    std::istringstream iss(gridStr);
    std::string line;

    while (std::getline(iss, line))
    {
        lines.push_back(line);
    }

    int numRows = lines.size();

    if (numRows < 2)
        printInvalidGridShapeError();

    int numCols = lines[0].size();

    for (int i = 1; i < numRows; i++) 
    {
        if (lines[i].size() != numCols)
            printInvalidGridShapeError();
    }

    if (numRows == numCols - (numCols / 2))
    {
        this->gridSize = numRows;
    }
    else
        printInvalidGridShapeError();
}

void PathFinding::setStartCell(int x, int y)
{
    if (this->grid.empty())
        printGridEmptyError();

    int y_cor = this->gridSize - 1 - y;

    Cell* startCell = new Cell(x, y_cor);

    this->grid[y_cor][x] = startCell;
    this->startCell = startCell;
}

void PathFinding::setEndCell(int x, int y)
{
    if (this->grid.empty())
        printGridEmptyError();

    int y_cor = this->gridSize - 1 - y;

    Cell* endCell = new Cell(x, y_cor);

    this->grid[y_cor][x] = endCell;
    this->endCell = endCell;
}



void PathFinding::initializeGrid(std::string gridStr)
{
    std::vector<std::vector<Cell*>> grid;
    std::istringstream inputStream(gridStr);
    std::string line;

    int row = 0;
    while (std::getline(inputStream, line)) {
        std::vector<Cell*> rowCells;
        std::istringstream lineStream(line);
        int col = 0;
        int cellValue;

        while (lineStream >> cellValue) {
            Cell* cell = new Cell(col, row);
            cell->isObstacle = (cellValue == 5);
            rowCells.push_back(cell);
            col++;
        }

        grid.push_back(rowCells);
        row++;
    }

    this->grid = grid;
}


double PathFinding::euclideanDistance(int x1, int y1, int x2, int y2)
{
    double dx = x1 - x2;
    double dy = y1 - y2;
    return std::sqrt(dx * dx + dy * dy);
}

void PathFinding::setCellFields(Cell* cell, int x, int y)
{
    if (!cell->isObstacle)
    {
        Cell* parent = this->grid[y][x];


        if (!containsCell(openList, cell))
        {
            cell->parent = parent;
            cell->g = cell->parent->g + 1;
            cell->h = euclideanDistance(cell->x, cell->y, this->endCell->x, this->endCell->y);
            cell->f = cell->calculateFValue();
            this->openList.push_back(cell);
        }
        else
        {
            int tempG = parent->g + 1;
            if (cell->calculateFValue() < cell->f)
            {
                cell->parent = parent;
                cell->g = tempG;
                cell->f = cell->calculateFValue();
            }
        }

    }
}

void PathFinding::printInvalidGridShapeError()
{
    std::cout << "\033[31m";
    std::cerr << "ERROR! Invalid grid shape. Make sure the grid is at least a 2 x 2 square." << std::endl;
    std::cout << "\033[0m";
    exit(1);
}

void PathFinding::printGridEmptyError()
{
    std::cout << "\033[31m";
    std::cerr << "ERROR! Cell matrix is empty. Make sure the constructor has been called properly." << std::endl;
    std::cout << "\033[0m";
    exit(1);
}

void PathFinding::printPathNotCalculatedError()
{
    std::cout << "\033[31m";
    std::cerr << "ERROR! Cannot print the path. Path was not calculated successfully." << std::endl;
    std::cout << "\033[0m";
    exit(1);
}

void PathFinding::printSuccessMessage()
{
    std::cout << "\033[32m";
    std::cout << "SUCCESS! The shortest path from (" << this->startCell->x << ", " << this->gridSize - 1 - this->startCell->y << ") to (" << this->endCell->x << ", " << this->gridSize - 1 - this->endCell->y << ") was found." << std::endl;
    std::cout << "\033[0m";
}

void PathFinding::printFailureMessage()
{
        std::cout << "\033[31m";
        std::cerr << "Cannot find the path from (" << this->startCell->x << ", " << this->gridSize - 1 - this->startCell->y << ") to (" << this->endCell->x << ", " << this->gridSize - 1 - this->endCell->y << ")." << std::endl;
        std::cout << "\033[0m";
}

Cell* PathFinding::getMinCell()
{
    return *std::min_element(this->openList.begin(), this->openList.end(), compareCellPointers);
}

bool PathFinding::containsCell(const std::vector<Cell*> cellVector, const Cell* targetCell) {
    return std::find(cellVector.begin(), cellVector.end(), targetCell) != cellVector.end();
}

bool PathFinding::compareCellPointers(const Cell* a, const Cell* b)
{
        return a->f < b->f;
}


bool PathFinding::findPath() 
{
    this->closedList.push_back(this->startCell);
    int cX = this->startCell->x;
    int cY = this->startCell->y;

    int goalX = this->endCell->x;
    int goalY = this->endCell->y;

    while ((cX != goalX) || (cY != goalY))
    {
        if (cY + 1 < this->gridSize)
        {
            Cell* neighbourCell = this->grid[cY + 1][cX];
            if (!containsCell(this->closedList, neighbourCell))
                setCellFields(neighbourCell, cX, cY);

        }

        if (cX - 1 >= 0)
        {
            Cell* neighbourCell = this->grid[cY][cX - 1];
            if (!containsCell(this->closedList, neighbourCell))
                setCellFields(neighbourCell, cX, cY);
        }

        if (cY - 1 >= 0)
        {
            Cell* neighbourCell = this->grid[cY - 1][cX];
            if (!containsCell(this->closedList, neighbourCell))
                setCellFields(neighbourCell, cX, cY);
        }

        if (cX + 1 < this->gridSize)
        {
            Cell* neighbourCell = this->grid[cY][cX + 1];
            if (!containsCell(this->closedList, neighbourCell))
                setCellFields(neighbourCell, cX, cY);

        }

        if (this->openList.empty())
        {
            return false;
        }

        Cell* minCell = getMinCell();
        cX = minCell->x;
        cY = minCell->y;
        this->closedList.push_back(minCell);
        std::erase(this->openList, minCell);

    }

    printSuccessMessage();
    this->isPathCalculated = true;
    return true;
}

void PathFinding::printPath()
{
    if (!this->isPathCalculated)
    {
        printPathNotCalculatedError();
        return;
    }

    Cell* cell = this->closedList[closedList.size() - 1];

    std::vector<std::string> lines;
    std::stringstream ss(this->gridStr);
    std::string line;

    while (std::getline(ss, line)) {
        lines.push_back(line);
    }

    while (cell != this->startCell)
    {
        lines[cell->y][2 * cell->x] = '3';
        cell = cell->parent;
    }
    lines[cell->y][2 * cell->x] = 'S';


    std::string updatedGridStr;

    for (const std::string& updatedLine : lines) {
        updatedGridStr += updatedLine + '\n';
    }

    for (char c : updatedGridStr)
    {
        if (c == '3')
        {
            std::cout << "\u001b[33m" << c << "\033[0m";
        }
        else
        {
            std::cout << c;
        }
    }

    this->gridStr = updatedGridStr;
}
