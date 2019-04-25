#include "parser.h"
#include <string>
#include "execution.h"
#include <map>
using namespace std;

std::map<string, int> labelMap;

//associate a label with a line number, for beq's and bne's
void labelParse(string line, int lineNum) {
	string instruction = line.substr(0, line.find(" "));

	if (instruction == "bne" || instruction == "beq") {
		string label = line.substr(line.find_last_of(",") + 1, line.find(":"));
		//question: will substring retain the newline character?
		//that'd throw off the label, since it should have a colon and no newline
		labelMap[label] = lineNum;
	}
}

void labelLine(string line, int lineNum) {
	if (line.at(line.length() - 2) == ':') {
		labelMap[line.substr(0, line.length()-2)] = lineNum;
	}
}

//please dont pass me invalid data
//note: the labelmap right now also has the lines of bne's and beq's with labels in them
int getLabelLine(string label) {
	return labelMap[label];
}

//this parser is for writeback
//think of this as execution parser
//returns true for branching
bool parse(string line, int saveReg[8], int tempReg[10]) {

	int zero = 0;

	string instruction = line.substr(0, line.find(" "));

		
	if (instruction == "add" || instruction == "or" || instruction == "and" || instruction == "slt") {

		string dest_str = line.substr(line.find("$")+1, line.find(","));

		int number = dest_str[1] - '0';

		int* dest, *reg1, *reg2;

		if (dest_str[0] == 't')
			dest = &(tempReg[number]);
		else
			dest = &(saveReg[number]);

		string reg1_str = line.substr(line.find(",")).substr(line.find("$") + 1, line.find(","));

		number = reg1_str[1] - '0';

		if (reg1_str[0] == 't')
			reg1 = &(tempReg[number]);
		else if (reg1_str[0] == 's')
			reg1 = &(saveReg[number]);
		else
			reg1 = &zero;

		string reg2_str = line.substr(line.find(","));
		reg2_str = line.substr(line.find("$") + 1, line.find(","));

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

		string reg1_str = line.substr(line.find(",")).substr(line.find("$") + 1, line.find(","));

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
		string left_str = line.substr(line.find("$") + 1, line.find(","));

		int number = left_str[1] - '0';

		int* left, * right;
		//int immediate = 0; -wall says this is unused

		if (left_str[0] == 't')
			left = &(tempReg[number]);
		else if (left_str[0] == 's')
			left = &(saveReg[number]);
		else
			left = &zero;

		string right_str = line.substr(line.find(",")).substr(line.find("$") + 1, line.find(","));

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