#pragma once
#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

class Data {
private:
	std::vector<std::vector<float>> data;

public:
	Data(std::string file);
	void printData();
	float dataAt(int i, int j);
	int nearestNeighbor(std::vector<int> featureSubSet, std::vector<float> newInstance);
};

#endif // !DATA_H
