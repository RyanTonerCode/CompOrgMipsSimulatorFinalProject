#ifndef parser_h
#define parser_h
#include <string>
#include <cstring>
#include <cstdlib>
using namespace std;

bool isLabel(string line);

int labelParse(string line);

void labelLine(string line, int lineNum);

int getLabelLine(string label);

bool parse(string line, int saveReg[8], int tempReg[10]);

#endif 