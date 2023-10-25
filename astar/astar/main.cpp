#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "cell.h"
#include "PathFinding.h"


const int GRID_ROWS = 20;
const int GRID_COLS = 20;

using namespace std;

string GetGridStr(string path) {
	ifstream file(path);

	if (!file.is_open()) {
		std::cerr << "Error: Can't open the grid.txt file." << std::endl;
		return "";
	}

	string gridStr;
	string line;

	while (getline(file, line)) {
		gridStr += line + "\n";
	}

	file.close();

	return gridStr;
}

int main() {
	string gridStr = GetGridStr("grid.txt");
	cout << gridStr;

	PathFinding pf = PathFinding(gridStr);
	pf.SetStartCell(0, 19);
	pf.SetEndCell(19, 0);
	pf.FindPath();



	return 0;
}