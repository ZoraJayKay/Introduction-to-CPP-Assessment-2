#include "DataFile.h"
#include <fstream>
#include <iostream>

using namespace std;

DataFile::DataFile()
{
	currentRecord = new Record;
}

DataFile::~DataFile()
{
	delete currentRecord;
	currentRecord = nullptr;
}

//void DataFile::AddRecord(string imageFilename, string name, int age)
//{
//	Image i = LoadImage(imageFilename.c_str());
//
//	Record* r = new Record;
//	r->image = i;
//	r->name = name;
//	r->age = age;
//
//	records.push_back(r);
//	recordCount++;
//}

DataFile::Record* DataFile::GetRecord(const char* file, int index)
{
	// ZORA: Load the record at the index from the nominated file into the current record
	Load(file, recordPositions[index]);
	return currentRecord;
}

// +++++++++++++++++++++++ ZORA: I didn't update the save function as it didn't seem to be in the scope of the assessment
//void DataFile::Save(string filename)
//{
//	// ZORA: Open a file (whose name is the parameter passed in) for streaming binary data (WRITE)
//	ofstream outfile(filename, ios::out | ios::binary);
//
//	// ZORA: Need to always check that the file opened successfully before reading or writing
//	if (outfile.is_open()) {
//		// ZORA: Below is the starting code
//		// Create a counter equal to the size of the number of records to be accessed
//		int recordCount = recordPositions.size();
//		
//		// ZORA: Update the number of records in the file
//		// ******** NEW *********
//		outfile.write(reinterpret_cast<char*>(& recordCount), sizeof(int));
//		// ******** NEW *********
//
//		// For as many times as there are records...
//		for (int i = 0; i < recordCount; i++)
//		{
//			// Create a pointer to a RayLib Color object, which holds the memory location of the Image of this Record
//			Color* imgdata = GetImageData(records[i]->image);
//
//			// Create an integer to hold the number of characters that represents the binary colour data of this Record's NPC image in the file.
//			int imageSize = 
//				sizeof(Color) * 
//				records[i]->image.width * 
//				records[i]->image.height;
//
//			// Create an integer for the number of characters in the NPC's name
//			int nameSize = records[i]->name.length();
//			// Create an integer for the memory allocation required for an integer ()
//			int ageSize = sizeof(int);
//
//			// Write the image width and height to the file
//			outfile.write(reinterpret_cast<char*>(& records[i]->image.width), sizeof(int));
//			outfile.write(reinterpret_cast<char*>(& records[i]->image.height), sizeof(int));
//
//			// Write the length (number of characters) of the name, and the age to the file
//			// ZORA: Below is the starting code
//			// outfile.write((char*)&nameSize, sizeof(int));
//			// outfile.write((char*)&ageSize, sizeof(int));
//			outfile.write(reinterpret_cast<char*>(& nameSize), sizeof(int));
//			outfile.write(reinterpret_cast<char*>(& ageSize), sizeof(int));
//
//			// Write the 
//			outfile.write((char*)imgdata, imageSize);
//			outfile.write((char*)records[i]->name.c_str(), nameSize);
//			outfile.write((char*)&records[i]->age, ageSize);
//		}
//	}
//	else{
//		// ZORA: What should I do if it fails to load?
//	}
//	
//	outfile.close();
//}

void DataFile::Load(string filename)
{
	// ZORA: Declare a file for streaming binary data
	ifstream infile;

	// ZORA: Open the declared file (whose name is the parameter passed in) for streaming binary data (READ)
	infile.open(filename, ios::in | ios::binary);

	// ZORA: Need to always check that the file opened successfully before reading or writing
	if (infile.is_open()) {
		recordCount = 0;
				
		// ZORA: Read the first 4 bytes, which contains how many records are in the file
		infile.read(reinterpret_cast<char*>(&recordCount), sizeof(int));

		// For as many times as there are records...
		// *** --- NOTE TO SELF: THIS IS THE SEQUENTIAL READ OF THE FILE --- ***
		for (int i = 0; i < recordCount; i++)
		{
			// ZORA: Before reading anything, record the current position in the file as an integer so we know where the record starts
			recordPositions.push_back(infile.tellg());

			// A temporary new integer for storing the NPC name.
			int nameSize = 0;
			// A temporary new integer for storing the NPC age.
			int ageSize = 0;
			// A temporary new integer for storing the NPC RayLib Image's width, height, format and total memory requirement
			int width = 0, height = 0, format = 0, imageSize = 0;

			// ZORA: Read the binary file containing NPC data and extract a number of binary characters equal to the number of bytes in an integer (4) from the stream, storing them in the integers 'width' and 'height'.
			infile.read(reinterpret_cast<char*>(&width), sizeof(int));
			infile.read(reinterpret_cast<char*>(&height), sizeof(int));

			// Set the number of characters to read from the file when we retrieve the binary colour data of this Record's NPC image in the file.
			imageSize = 
				sizeof(Color) * 
				width * 
				height;

			// Are we working on the basis of 4 bytes because RayLib colour information is in RGBA (1 byte each)...?
			// ZORA: Read the binary file containing NPC data and extract a number of characters equal to the number of bytes in an integer (4) from the stream, storing them in the integers nameSize and ageSize.
			infile.read(reinterpret_cast<char*>(&nameSize), sizeof(int));
			infile.read(reinterpret_cast<char*>(&ageSize), sizeof(int));

			// ZORA: Create a pointer to a dynamic array of characters which will hold the colour data of the NPC's Image. The dynamic array's size is equal to the number of characters in the colour data of this Record's NPC Image, expressed as an integer.
			char* imgdata = new char[imageSize];
			// Read all of the characters from this Record's NPC Image and copy them to the dynamic array.
			infile.read(imgdata, imageSize);

			// Create a new RayLib Image and populate it with the colour data of the NPC Image from the binary file.
			//Image img = LoadImageEx((Color*)imgdata, width, height);

			// ZORA: Create a pointer to a dynamic array of characters which will hold the name data of the NPC. The dynamic array's size is equal to the number of characters in the name of this Record, expressed as an integer.
			// THIS NEEDS TO HAVE A NULL POINTER ON THE END BECAUSE OTHERWISE IT WILL CONTINUE TO READ UNTIL ONE IS FOUND
			char* name = new char[nameSize + 1];
			// Set the final address to null so the reader detects the end has been reached
			name[nameSize] = NULL;

			int age = 0;

			// ZORA: The original code reads the binary file containing NPC data and extracts a number of characters equal to the number of bytes in an integer (4) from the stream, storing them in a pointer to a character array.
			infile.read((char*)name, nameSize);
			infile.read((char*)&age, ageSize);

			// ZORA: Record the current position in the file (as at the time of reaching the end of the record) as an integer
			//recordPositions.push_back(infile.tellg());

			// ZORA: Delete pointer to the dynamic array of characters that holds NPC colour data
			delete[] imgdata;
			// ZORA ---> ADDED null pointer instruction
			imgdata = nullptr;

			// ZORA: Delete pointer to the dynamic array of characters that holds NPC name data
			delete[] name;
			// ZORA ---> ADDED null pointer instruction
			name = nullptr;
		}
	}

	else {
		// ZORA: What should I do if it fails to load?
	}	

	infile.close();
}


void DataFile::Load(string filename, int searchIndex)
{
	// ZORA: Declare a file for streaming binary data
	ifstream infile;

	// ZORA: Open the declared file (whose name is the parameter passed in) for streaming binary data (READ)
	infile.open(filename, ios::in | ios::binary);

	// ZORA: Need to always check that the file opened successfully before reading or writing
	if (infile.is_open()) {
		// ZORA: Navigate through the file until we've reached the position of this record
		infile.seekg(searchIndex);

		// A temporary new integer for storing the NPC name.
		int nameSize = 0;
		// A temporary new integer for storing the NPC age.
		int ageSize = 0;
		// A temporary new integer for storing the NPC RayLib Image's width, height, format and total memory requirement
		int width = 0, height = 0, format = 0, imageSize = 0;

		// ZORA: Read the binary file containing NPC data and extract a number of binary characters equal to the number of bytes in an integer (4) from the stream,storing them in the integers 'width' and 'height'.
		infile.read(reinterpret_cast<char*>(&width), sizeof(int));
		infile.read(reinterpret_cast<char*>(&height), sizeof(int));

		// Set the number of characters to read from the file when we retrieve the binary colour data of this Record's NPC image in the file.
		imageSize =
			sizeof(Color) *
			width *
			height;

		// Are we working on the basis of 4 bytes because RayLib colour information is in RGBA (1 byte each)...?
		// ZORA: Read the binary file containing NPC data and extract a number of characters equal to the number of bytes in an integer (4) from the stream, storingthem in the integers nameSize and ageSize.
		infile.read(reinterpret_cast<char*>(&nameSize), sizeof(int));
		infile.read(reinterpret_cast<char*>(&ageSize), sizeof(int));

		// ZORA: Create a pointer to a dynamic array of characters which will hold the colour data of the NPC's Image. The dynamic array's size is equal to the numbe of characters in the colour data of this Record's NPC Image, expressed as an integer.
		char* imgdata = new char[imageSize];
		// Read all of the characters from this Record's NPC Image and copy them to the dynamic array.
		infile.read(imgdata, imageSize);

		// Create a new RayLib Image and populate it with the colour data of the NPC Image from the binary file.
		Image img = LoadImageEx((Color*)imgdata, width, height);

		// ZORA: Create a pointer to a dynamic array of characters which will hold the name data of the NPC. The dynamic array's size is equal to the number ofcharacters in the name of this Record, expressed as an integer.
		// THIS NEEDS TO HAVE A NULL POINTER ON THE END BECAUSE OTHERWISE IT WILL CONTINUE TO READ UNTIL ONE IS FOUND
		char* name = new char[nameSize + 1];
		// Set the final address to null so the reader detects the end has been reached
		name[nameSize] = NULL;

		int age = 0;

		// ZORA: The original code reads the binary file containing NPC data and extracts a number of characters equal to the number of bytes in an integer (4) fromthe stream, storing them in a pointer to a character array.
		infile.read((char*)name, nameSize);
		infile.read((char*)&age, ageSize);

		// ZORA: Set the particulars of the 'current record' as being equal to those found in this record
		currentRecord->image = img;
		currentRecord->name = string(name);
		currentRecord->age = age;

		// ZORA: Delete pointer to the dynamic array of characters that holds NPC colour data
		delete[] imgdata;
		// ZORA ---> ADDED null pointer instruction
		imgdata = nullptr;

		// ZORA: Delete pointer to the dynamic array of characters that holds NPC name data
		delete[] name;
		// ZORA ---> ADDED null pointer instruction
		name = nullptr;
	}

	else {
		// ZORA: What should I do if it fails to load?
	}

	infile.close();
}