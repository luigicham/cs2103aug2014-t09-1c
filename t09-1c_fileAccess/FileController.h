#pragma once
#ifndef FILE_CONTROLLER
#define FILE_CONTROLLER

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#define BAD_ACCESS \
	"Error. File cannot be accessed."

#endif

using namespace std;

class FileController
{
private:
	FileController();
	~FileController();

public:
	static bool checkFile(string fileName);
	static vector<string> parseFileToMemoryVector(string fileName);
	static bool cloneMemoryVectorToFile(string fileName, vector<string> memVector);
	static string declareFileAccessError();
};

