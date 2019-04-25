#ifndef parser
#define parser

#include <string>
#include <cstring>
#include <cstdlib>

void labelParse(std::string line, int lineNum);

void labelLine(std::string line, int lineNum);

int getLabelLine(std::string label);

bool parse(std::string line, int saveReg[8], int tempReg[10]);

#endif 