// pccp-L03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "stdafx.h"
#include <string>  
#include <iostream> 
#include <fstream>
#include <vector>
#include "stdlib.h" 
#include <ctime>
#include <fstream>
#include <random>
#include <sstream>      // std::istringstream
#include <iterator>     // std::istream_iterator
#include <amp.h> 
#include <amp_graphics.h>

using namespace std;
using namespace concurrency;


// for splitting string by commas
class WordDelimitedByCommas : public std::string
{};
// overload of operator to read until next comma
std::istream& operator >> (std::istream& is, WordDelimitedByCommas& output) {
	std::getline(is, output, ',');
	return is;
}
//return random char
char genRandom() {
	static const char alphanum[] =
		"0123456789"
		"!@#$%^&*"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	int stringLength = sizeof(alphanum) - 1;
	return alphanum[rand() % stringLength];
}

void printHelp() {
	cout << "mmDistPt.exe -i fileName -[h] [-v] -[w] [-p] [-g n,dx,dy,dz]" << endl;
	cout << "options:" << endl;
	cout << "    -i set the filename with the points. If -f option is set a new dataset is generated with this name" << endl;
	cout << "    -h \tprints help" << endl;
	cout << "    -v \tactivate additional outputs" << endl;
	cout << "    -w \twait before ending the programm for key press" << endl;
	cout << "    -p \tshow processing time" << endl;
	cout << "    -g  n,dx,dy,dz geneartes a new random dataset with n points and max. dx,dy,dz coordinates" << endl;
}

void generateRandomPointsAndWriteToFile(int numPoints, int dx, int dy, int dz, string _filename) {

	ofstream myfile;
	myfile.open(_filename, ios::out);
	/* initialize random seed: */
	srand(time(NULL));

	//generates random coordinates from 0 to dx,dy and dz respectively
	for (int i = 0; i < numPoints; i++) {
		float rnd_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/dx);
		float rnd_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/dy);
		float rnd_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/dz);

		//use random string as point identifier
		std::string Str;
		for (int z = 0; z < 10; z++)
		{
			Str += genRandom();
		}

		//writes random points with point identifier to file
		myfile << Str << ", " << rnd_x << "," << rnd_y << "," << rnd_z <<std::endl;
		Str = ' ';
	}
	myfile.close();
}


void openFileAndReadPoints(string _filename, vector<graphics::float_3>* _datapoints, vector<string>* _datapointsName) {
	//read points from file
	ifstream infile(_filename);
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::vector<std::string> results((std::istream_iterator<WordDelimitedByCommas>(iss)),
		std::istream_iterator<WordDelimitedByCommas>());
		graphics::float_3 indata(stof(results.at(1)), stof(results.at(2)), stof(results.at(3)));
		(*_datapoints).push_back(indata);
		(*_datapointsName).push_back(results[0]);

	}
}



int main(int argc, char* argv[])
{
	float maxdist = 0;
	float mindist;
	vector<string> name_mindist;
	vector<string> name_maxdist;
	vector<graphics::float_3> datapoints;
	vector<string> datapointNames;
	bool printProcesTime = false;
	bool extendedOutput = false;
	bool waitBeforeTerminate = false;
	string filePath;
	int n;
	float dx, dy, dz;
	clock_t processTime;
	std::istringstream iss;


	if (argc < 2) {
		printHelp();
		return 0;
	}

	//handle input arguments
	for (int i = 1; i < argc; i++) {
		try {
			string argument = argv[i];

			if (argument == "-i") {
				filePath = argv[i + 1];
				openFileAndReadPoints(filePath, &datapoints, &datapointNames);
				i++;
			}

			else if (argument == "-g") {
				string temp = argv[i + 1];
				cout << temp << endl;
				//TODO split string by , and convert to n;dx,dy,dz
				std::istringstream iss(temp);
				std::vector<std::string> results((std::istream_iterator<WordDelimitedByCommas>(iss)),
					std::istream_iterator<WordDelimitedByCommas>());
			
				int _n = std::stoi(results[0]);
				int _dx = std::stoi(results[1]);
				int _dy = std::stoi(results[2]);
				int _dz = std::stoi(results[3]);
				string filename = "pccp_03.txt";
				generateRandomPointsAndWriteToFile(_n,_dx,_dy,_dz, filename);
				i++;
		
			}
			else if (argument == "-h") {
				printHelp();
				break;
			}
			else if (argument == "-p") {
				printProcesTime = true;
			}
			else if (argument == "-v") {
				extendedOutput = true;
			}
			else if (argument == "-w") {
				waitBeforeTerminate = true;
			}
			else {
				cout << "unknown argument: " << argv[i] << endl;
			}
		}
		catch (exception e) {
			fprintf(stderr, "Error: in interpreting arguments %s \n", argv[i]);
		}

	}

	//start measuring process time if wanted
	if (printProcesTime) {
		processTime = clock();
	}

	//TODO place code here
	//accelerator myAcc(accelerator::default_accelerator);
	//accelerator_view accView = myAcc.get_default_view();

	//array_view<graphics::float_3> av();
	//parallel_for_each(av.extent, [=](index<1> idx) restrict(amp)
	//{
	//	av[idx] += 1;
	//});

	for (int i = 0; i < datapoints.size(); i++) {
		for (int k = i + 1; k < datapoints.size(); k++) {
			float d[] = { abs(datapoints.at(i).x - datapoints.at(k).x), abs(datapoints.at(i).y - datapoints.at(k).y), abs(datapoints.at(i).z - datapoints.at(k).z) };
			if (d[0] < d[1]) swap(d[0], d[1]);
			if (d[0] < d[2]) swap(d[0], d[2]);
			float distance = d[0] * sqrt(1.0 + d[1] / d[0] + d[2] / d[0]);
			if (distance < mindist || (i==0 && k==1) ) {
				mindist = distance;
				name_mindist.clear();
				name_mindist.push_back(datapointNames.at(i));
				name_mindist.push_back(datapointNames.at(k));
			}
			else if(distance == mindist) {
				name_mindist.push_back(datapointNames.at(i));
				name_mindist.push_back(datapointNames.at(k));
			}
			else if (distance > maxdist) {
				maxdist = distance;
				name_maxdist.clear();
				name_maxdist.push_back(datapointNames.at(i));
				name_maxdist.push_back(datapointNames.at(k));
			}
			else if (distance == maxdist) {
				name_maxdist.push_back(datapointNames.at(i));
				name_maxdist.push_back(datapointNames.at(k));
			}
		}
	}
	cout << "Mindist zwischen: " << endl;
	for (string temp: name_mindist){
		cout << temp << endl;
		}

	cout << endl << "Maxdist zwischen: " << endl;
	for (string temp : name_maxdist) {
		cout << temp <<endl;
	}

	//print process time
	if (printProcesTime) {
		processTime = clock() - processTime;
		double time_taken = ((double)processTime) / CLOCKS_PER_SEC; // in seconds
		printf("\nThe process took %f seconds to execute \n\n", time_taken);
	}

	if (extendedOutput) {
		cout << "extended output" << endl;
	}

	//wait for input before exit
	if (waitBeforeTerminate) {
		cout << "Press any key to exit...";
		_getch();
	}


	return 0;
}

