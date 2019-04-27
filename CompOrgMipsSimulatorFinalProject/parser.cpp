#include "parser.h"
#include <string>
#include "execution.h"
#include <map>
using namespace std;

//Maps the label to the proceding line of code.
map<string, int> labelMap;

//returns whether the line defines a label
bool isLabel(string line) {
	return line.at(line.length() - 1) == ':';
}

//get line number from a label, for beq's and bne's
int labelParse(string line) {
	string instruction = line.substr(0, line.find(" "));

	if (instruction == "bne" || instruction == "beq") {
		string label = line.substr(line.find_last_of(",") + 1);
		return getLabelLine(label);
	}
	return -1;
}

//sets the map for a label line with a colon
void labelLine(string line, int lineNum) {
	if (isLabel(line))
		labelMap[line.substr(0, line.length() - 1)] = lineNum;
}

//returns the line associated with the given label
int getLabelLine(string label) {
	return labelMap.at(label);
}

//This parser is for the writeback stage
//Think of this as "execution" parser
//Returns true for branching
bool parse(string line, int saveReg[8], int tempReg[10]) {

	//pseudo zero-register
	int zero = 0;

	string instruction = line.substr(0, line.find(" "));
		
	if (instruction == "add" || instruction == "or" || instruction == "and" || instruction == "slt") {

		string dest_str = line.substr(line.find("$")+1, line.find(",") - (line.find("$") + 1));

		int number = dest_str[1] - '0';

		int* dest, *reg1, *reg2;

		if (dest_str[0] == 't')
			dest = &(tempReg[number]);
		else
			dest = &(saveReg[number]);

		string reg1_str = line.substr(line.find(",") + 2, line.find_last_of(",") - (line.find(",") + 1));

		number = reg1_str[1] - '0';

		if (reg1_str[0] == 't')
			reg1 = &(tempReg[number]);
		else if (reg1_str[0] == 's')
			reg1 = &(saveReg[number]);
		else
			reg1 = &zero;

		string reg2_str = line.substr(line.find_last_of("$")+1);

		number = reg2_str[1] - '0';

		if (reg2_str[0] == 't')
			reg2 = &(tempReg[number]);
		else if (reg2_str[0] == 's')
			reg2 = &(saveReg[number]);
		else
			reg2 = &zero;

		if(instruction == "add")
			add_(dest, reg1, reg2);
		else if (instruction == "and")
			and_(dest, reg1, reg2);
		else if (instruction == "or")
			or_(dest, reg1, reg2);
		else if (instruction == "slt")
			slt_(dest, reg1, reg2);
	}
	else if (instruction == "addi" || instruction == "ori" || instruction == "slti" || instruction == "andi") {
		
		string dest_str = line.substr(line.find("$") + 1, line.find(","));

		int number = dest_str[1] - '0';

		int* dest, * reg1;
		int immediate = 0;

		if (dest_str[0] == 't')
			dest = &(tempReg[number]);
		else
			dest = &(saveReg[number]);

		string reg1_str = line.substr(line.find(",") + 2, line.find_last_of(",") - (line.find(",") + 1));

		number = reg1_str[1] - '0';

		if (reg1_str[0] == 't')
			reg1 = &(tempReg[number]);
		else if (reg1_str[0] == 's')
			reg1 = &(saveReg[number]);
		else
			reg1 = &zero;

		string imm_str = line.substr(line.find_last_of(",") + 1);

		immediate = atoi((const char*)imm_str.c_str());

		if (instruction == "addi")
			addi_(dest, reg1, immediate);
		else if (instruction == "andi")
			andi_(dest, reg1, immediate);
		else if (instruction == "ori")
			ori_(dest, reg1, immediate);
		else if (instruction == "slti")
			slti_(dest, reg1, immediate);
	}
	else if (instruction == "bne" || instruction == "beq") {
		string left_str = line.substr(line.find("$") + 1, line.find(",") - (line.find("$") + 1));

		int number = left_str[1] - '0';

		int* left, *right;

		if (left_str[0] == 't')
			left = &(tempReg[number]);
		else if (left_str[0] == 's')
			left = &(saveReg[number]);
		else
			left = &zero;

		string right_str = line.substr(line.find_last_of("$")+1, line.find_last_of(",") - (line.find_last_of("$")+1));

		number = right_str[1] - '0';

		if (right_str[0] == 't')
			right = &(tempReg[number]);
		else if (right_str[0] == 's')
			right = &(saveReg[number]);
		else
			right = &zero;

		if (instruction == "beq") {
			if (beq_(left, right))
				return true;
		} else if (instruction == "bne")
            if (bne_(left, right))
                return true;

	}

	return false;
}