#include "DataFile.h"
#include <fstream>
using namespace std;

DataFile::DataFile()
{
	recordCount = 0;
}

DataFile::~DataFile()
{
	Clear();
}

void DataFile::AddRecord(string imageFilename, string name, int age)
{
	Image i = LoadImage(imageFilename.c_str());

	Record* r = new Record;
	r->image = i;
	r->name = name;
	r->age = age;

	records.push_back(r);
	recordCount++;
}

DataFile::Record* DataFile::GetRecord(int index)
{
	return records[index];
}

void DataFile::Save(string filename)
{
	// ZORA: Open a file for streaming binary data, whose name is the parameter passed in
	ofstream outfile(filename, ios::binary);

	// ZORA: Need to always check that the file opened successfully before reading or writing
	if (outfile.is_open()) {

		// ZORA: Below is the starting code
		int recordCount = records.size();

		// ZORA: Below is the starting code
		// outfile.write((char*)&recordCount, sizeof(int));
		
		// ZORA: Update the size of the buffer to be written so it equals the size of an instance of a record
		outfile.write((char*)&recordCount, sizeof(DataFile::Record));

		for (int i = 0; i < recordCount; i++)
		{
			Color* imgdata = GetImageData(records[i]->image);

			int imageSize = sizeof(Color) * records[i]->image.width * records[i]->image.height;
			int nameSize = records[i]->name.length();
			int ageSize = sizeof(int);

			outfile.write((char*)&records[i]->image.width, sizeof(int));
			outfile.write((char*)&records[i]->image.height, sizeof(int));

			outfile.write((char*)&nameSize, sizeof(int));
			outfile.write((char*)&ageSize, sizeof(int));

			outfile.write((char*)imgdata, imageSize);
			outfile.write((char*)records[i]->name.c_str(), nameSize);
			outfile.write((char*)&records[i]->age, ageSize);
		}
	}
	else{
		// ZORA: What should I do if it fails to load?
	}
	
	outfile.close();
}

void DataFile::Load(string filename)
{
	Clear();	
	// ZORA: This is the starting code below
	// ifstream infile(filename, ios::binary);

	// ZORA: Declare a file for streaming binary data, whose name is the parameter passed in
	ifstream infile;

	// ZORA: Open the declared file
	infile.open(filename, ios::binary);

	// ZORA: Need to always check that the file opened successfully before reading or writing
	if (infile.is_open()) {

		// ZORA: This is the starting code
		recordCount = 0;
					
		// ORIGINAL CODE ---> infile.read((char*)&recordCount, sizeof(int));
		// ZORA: The original code reads the binary file containing NPC data and extracts a number of characters equal to the number of bytes in an integer (4) from the stream, storing them in the integer recordCount.
				
		// ZORA: Update the size of the buffer to be read so it equals the size of an instance of a record
		infile.read((char*)&recordCount, sizeof(DataFile::Record));

		for (int i = 0; i < recordCount; i++)
		{
			int nameSize = 0;
			int ageSize = 0;
			int width = 0, height = 0, format = 0, imageSize = 0;

			// ZORA: The original code reads the binary file containing NPC data and extracts a number of characters equal to the number of bytes in an integer (4) from the stream, storing them in the integers width and height.
			infile.read((char*)&width, sizeof(int));
			infile.read((char*)&height, sizeof(int));

			imageSize = sizeof(Color) * width * height;

			// ZORA: The original code reads the binary file containing NPC data and extracts a number of characters equal to the number of bytes in an integer (4) from the stream, storing them in the integers nameSize and ageSize.
			infile.read((char*)&nameSize, sizeof(int));
			infile.read((char*)&ageSize, sizeof(int));

			char* imgdata = new char[imageSize];
			infile.read(imgdata, imageSize);

			Image img = LoadImageEx((Color*)imgdata, width, height);

			// ZORA: Create a new char pointer to a char array with a number of elements equal to the number of bytes in an integer (4).
			char* name = new char[nameSize];
			int age = 0;

			// ZORA: The original code reads the binary file containing NPC data and extracts a number of characters equal to the number of bytes in an integer (4) from the stream, storing them in a pointer to a character array.
			infile.read((char*)name, nameSize);


			infile.read((char*)&age, ageSize);

			Record* r = new Record();
			r->image = img;
			r->name = string(name);
			r->age = age;
			records.push_back(r);

			// ZORA: Delete pointer to an array of chars
			delete[] imgdata;
			// ZORA ---> ADDED null pointer instruction
			imgdata = nullptr;

			// ZORA: Delete pointer to an array of chars
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

void DataFile::Clear()
{
	for (int i = 0; i < records.size(); i++)
	{
		delete records[i];

		// ZORA ---> ADDED null pointer instruction
		records[i] = nullptr;
	}
	records.clear();
	recordCount = 0;
}