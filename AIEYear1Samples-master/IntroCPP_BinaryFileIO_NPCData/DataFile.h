#pragma once

#include "raylib.h"
#include <string>
#include <vector>

using namespace std;

class DataFile
{
public:
	struct Record {
		Image image;
		string name;
		int age;
	};

private:
	int recordCount;
	std::vector<int> recordPositions;	// ZORA: Each integer measures the distance (in binary characters) from the start of the file to each record
	Record* currentRecord;


public:
	DataFile();
	~DataFile();

	void AddRecord(string imageFilename, string name, int age);
	Record* GetRecord(const char* file, int index);

	int GetRecordCount() { return recordCount; };

	void Save(string filename);
	void Load(string filename);
	void Load(string filename, int searchIndex);
};

