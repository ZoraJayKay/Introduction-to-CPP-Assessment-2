#pragma once

#include "raylib.h"
#include <string>
#include <vector>

using namespace std;

// NPC record container 
class DataFile
{
public:
	// Individual NPC record with particulars in member variables 
	struct Record {
		Image image;
		string name;
		int age;
	};

private:
	// The volume of records found in the binary file
	int recordCount;
	// The position of the records within the binary file, expressed as the distance (in binary characters) from the start of the file to each record
	std::vector<int> recordPositions;
	// The record being viewed at an arbitrary point in time
	Record currentRecord;


public:
	// Constructor
	DataFile();

	// Destructor
	~DataFile();

	// A function for returning an individual NPC record to be shown on screen by main()
	Record GetRecord(const char* file, int index);

	// A function to return the number of NPC records in the binary file
	int GetRecordCount() { return recordCount; };

	// Save function is commented out / disabled as not in use for this program
	//void Save(string filename);

	// A function for reading the whole binary file on startup so as to find the location in the file where each NPC can later be randomly accessed 
	void Load(string filename);

	// A function for randomly accessing an individual NPC record after startup
	void Load(string filename, int searchIndex);
};

