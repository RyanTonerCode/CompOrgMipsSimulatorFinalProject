#include "data_parser.h"
#include <string>
#include <map>
#include <iostream>
#include <iomanip>

using namespace std;

//maps label to string
map<string, string> stringData;

//maps address to string
map<int, string> addressedStringData;

//Parses out a line of code in the data segment
void parseDataLine(string currentLine) {

	//get the label
	string label = currentLine.substr(0, currentLine.find(":"));

	//figure out the data type, starting from the period like .asciiz
	int dataPeriod = currentLine.find(".") + 1;

	string afterPeriod = currentLine.substr(dataPeriod);

	string datatype = afterPeriod.substr(0, afterPeriod.find(" "));

	//check to see if data type is string
	if (datatype == "asciiz") {

		int string_start = afterPeriod.find_first_of("\"") + 1;
		//get the string
		stringData[label] = afterPeriod.substr(string_start, afterPeriod.find_last_not_of("\"") - string_start + 1);
		addressedStringData[getStringAddress(label)] = stringData[label];
	}
}

int getStringAddress(string label) {
	return (int)(&stringData[label][0]);
}

string getStringFromAddress(int address) {
	int abc = 5;
	return addressedStringData[address];
}