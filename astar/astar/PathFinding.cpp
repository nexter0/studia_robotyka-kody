#include "PathFinding.h"
#include <sstream>
#include <cmath>
#include <iostream>
#include <algorithm>

PathFinding::PathFinding(std::string gridStr)
{
    this->_gridStr = gridStr;

    SetGridSize(this->_gridStr);
    InitializeGrid(this->_gridStr);
}

int PathFinding::SetGridSize(std::string gridStr)
{
    std::vector<std::string> lines;
    std::istringstream iss(gridStr);
    std::string line;

    while (std::getline(iss, line))
    {
        lines.push_back(line);
    }

    int numRows = lines.size();

    if (numRows == 0)
        return -1;

    int numCols = lines[0].size();

    for (int i = 1; i < numRows; i++) 
    {
        if (lines[i].size() != numCols) 
            return -1;
    }

    if (numRows == numCols - (numCols / 2))
    {
        this->_gridSize = numRows;
        return numRows;
    }
    else
        return -1;
}

int PathFinding::SetGridSize(int gridSize)
{
    this->_gridSize = gridSize;
    return this->_gridSize;
}

void PathFinding::SetStartCell(int x, int y)
{
    if (this->_grid.empty())
        return;
    
    Cell* startCell = new Cell(x, y);

    this->_grid[y][x] = startCell;
    this->_startCell = startCell;
}

void PathFinding::SetEndCell(int x, int y)
{
    if (this->_grid.empty())
        return;

    Cell* endCell = new Cell(x, y);

    this->_grid[y][x] = endCell;
    this->_endCell = endCell;
}



void PathFinding::InitializeGrid(std::string gridStr)
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

    this->_grid = grid;
}


double PathFinding::EuclideanDistance(int x1, int y1, int x2, int y2)
{
    double dx = x1 - x2;
    double dy = y1 - y2;
    return std::sqrt(dx * dx + dy * dy);
}

void PathFinding::SetCellFields(Cell* cell, int x, int y)
{
    if (!cell->isObstacle)
    {
        Cell* parent = this->_grid[y][x];


        if (!containsCell(_openList, cell))
        {
            cell->parent = parent;
            cell->g = cell->parent->g + 1;
            cell->h = EuclideanDistance(cell->x, cell->y, this->_endCell->x, this->_endCell->y);
            cell->f = cell->CalculateValue();
            this->_openList.push_back(cell);
        }
        else
        {
            int tempG = parent->g + 1;
            if (cell->CalculateValue() < cell->f)
            {
                cell->parent = parent;
                cell->g = tempG;
                cell->f = cell->CalculateValue();
            }
        }

    }
}

Cell* PathFinding::GetMinValueCell()
{
    return *std::min_element(this->_openList.begin(), this->_openList.end(), compareCellPointers);
}

bool PathFinding::containsCell(const std::vector<Cell*> cellVector, const Cell* targetCell) {
    return std::find(cellVector.begin(), cellVector.end(), targetCell) != cellVector.end();
}

bool PathFinding::compareCellPointers(const Cell* a, const Cell* b)
{
        return a->f < b->f;
}


bool PathFinding::FindPath() 
{
    this->_closedList.push_back(this->_startCell);
    int cX = this->_startCell->x;
    int cY = this->_startCell->y;

    int goalX = this->_endCell->x;
    int goalY = this->_endCell->y;

    while ((cX != goalX) || (cY != goalY))
    {
        // w dół
        if (cY + 1 < this->_gridSize)
        {
            Cell* neighbourCell = this->_grid[cY + 1][cX];
            if (!containsCell(this->_closedList, neighbourCell))
                SetCellFields(neighbourCell, cX, cY);

        }

        // w lewo
        if (cX - 1 >= 0)
        {
            Cell* neighbourCell = this->_grid[cY][cX - 1];
            if (!containsCell(this->_closedList, neighbourCell))
                SetCellFields(neighbourCell, cX, cY);
        }

        // w górę
        if (cY - 1 >= 0)
        {
            Cell* neighbourCell = this->_grid[cY - 1][cX];
            if (!containsCell(this->_closedList, neighbourCell))
                SetCellFields(neighbourCell, cX, cY);
        }

        // w prawo
        if (cX + 1 < this->_gridSize)
        {
            Cell* neighbourCell = this->_grid[cY][cX + 1];
            if (!containsCell(this->_closedList, neighbourCell))
                SetCellFields(neighbourCell, cX, cY);

        }

        Cell* minCell = GetMinValueCell();
        cX = minCell->x;
        cY = minCell->y;
        this->_closedList.push_back(minCell);
        std::erase(this->_openList, minCell);
    }

    std::cout << "OK: " << cX << " " << cY << std::endl;
    PrintPath(this->_gridStr);
    return true;
}

void PathFinding::PrintPath(std::string gridStr)
{
    Cell* cell = this->_closedList[_closedList.size() - 1];

    std::stringstream ss(gridStr);
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(ss, line)) {
        lines.push_back(line);
    }

    while (cell != this->_startCell)
    {
        lines[cell->y][2*cell->x] = '3';
        gridStr.clear();
        for (const std::string& updatedLine : lines) {
            gridStr += updatedLine + '\n';
        }

        cell = cell->parent;
    }
    lines[cell->y][2 * cell->x] = 'S';
    gridStr.clear();
    for (const std::string& updatedLine : lines) {
        gridStr += updatedLine + '\n';
    }

    //std::cout << gridStr << std::endl;
    for (char c : gridStr)
    {
        if (c == '3')
        {
            std::cout << "\u001b[33m";
            std::cout << c;
            std::cout << "\033[0m";
        }
        else
            std::cout << c;
    }
        
    std::cout << std::endl << "\033[32m";
    std::cout << "PATH FOUND SUCCESSFULLY!" << std::endl;
    std::cout << "\033[0m";

}


