#include "parser.h"
#include <string>
#include "execution.h"
using namespace std;

//this parser is for writeback
//returns true for branching
bool parse(string line, int* saveReg[8], int* tempReg[10]) {

	string instruction = line.substr(0, line.find(" "));

		
	if (instruction == "add" || instruction == "or" || instruction == "and" || instruction == "slt") {

		string dest_str = line.substr(line.find("$")+1, line.find(","));

		int number = atoi((const char*)dest_str[1]);

		int* dest, *reg1, *reg2;

		if (dest_str[0] == 't')
			dest = &(*tempReg[number]);
		else
			dest = &(*saveReg[number]);

		string reg1_str = line.substr(line.find(",")).substr(line.find("$") + 1, line.find(","));

		number = atoi((const char*)reg1_str[1]);

		if (reg1_str[0] == 't')
			reg1 = &(*tempReg[number]);
		else
			reg1 = &(*saveReg[number]);

		string reg2_str = line.substr(line.find(","));
		reg2_str = line.substr(line.find("$") + 1, line.find(","));

		number = atoi((const char*)reg2_str[1]);

		if (reg2_str[0] == 't')
			reg2 = &(*tempReg[number]);
		else
			reg2 = &(*saveReg[number]);

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

		int number = atoi((const char*)dest_str[1]);

		int* dest, * reg1;
		int immediate = 0;

		if (dest_str[0] == 't')
			dest = &(*tempReg[number]);
		else
			dest = &(*saveReg[number]);

		string reg1_str = line.substr(line.find(",")).substr(line.find("$") + 1, line.find(","));

		number = atoi((const char*)reg1_str[1]);

		if (reg1_str[0] == 't')
			reg1 = &(*tempReg[number]);
		else
			reg1 = &(*saveReg[number]);

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

		int number = atoi((const char*)left_str[1]);

		int* left, * right;
		int immediate = 0;

		if (left_str[0] == 't')
			left = &(*tempReg[number]);
		else
			left = &(*saveReg[number]);

		string right_str = line.substr(line.find(",")).substr(line.find("$") + 1, line.find(","));

		number = atoi((const char*)right_str[1]);

		if (right_str[0] == 't')
			right = &(*tempReg[number]);
		else
			right = &(*saveReg[number]);

		if (instruction == "beq")
			if (beq_(left, right))
				return true;
        else if (instruction == "bne")
            if (bne_(left, right))
                return true;

	}

	return false;
}