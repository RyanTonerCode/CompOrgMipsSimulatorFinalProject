#ifndef data_parser_h
#define data_parser_h
#include <string>
using namespace std;

int getStringAddress(string label);

string getStringFromAddress(int address);

void parseDataLine(string currentLine);

#endif