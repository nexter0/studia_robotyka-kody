#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "cell.h"
#include "PathFinding.h"

std::string getGridStr(std::string path) {
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cerr << "Error: Can't open the grid.txt file." << std::endl;
		return "";
	}

	std::string gridStr;
	std::string line;

	while (std::getline(file, line)) {
		gridStr += line + "\n";
	}

	file.close();

	return gridStr;
}

int main() {
	std::string gridStr = getGridStr("grid.txt");
	std::cout << gridStr;

	PathFinding pf = PathFinding(gridStr);
	pf.setStartCell(0, 0);
	pf.setEndCell(19, 19);
	if (pf.findPath())
	{
		pf.printPath();
	}
	else
		pf.printFailureMessage();

	return 0;
}