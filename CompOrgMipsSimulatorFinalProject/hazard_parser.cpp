#include "hazard_parser.h"
#include <string>
using namespace std;

//this method returns whether or not a hazard is found.
//make sure you only check at the right pipleline stage w.r.t. forwarding if necessary too
bool dataHazard(string currentLine, string prevLine) {
	
	//only need the destination of the previous line
	string dest_str = prevLine.substr(prevLine.find("$"), prevLine.find(",") - prevLine.find_first_of("$"));

	//need the used argument only registers for the current line
	string instruction = currentLine.substr(0, currentLine.find(" "));

	if (instruction == "add" || instruction == "or" || instruction == "and" || instruction == "slt") {

		string reg1 = currentLine.substr(currentLine.find(",") + 1, currentLine.find_last_of(",") - (currentLine.find(",") + 1));
		string reg2 = currentLine.substr(currentLine.find_last_of("$"));

		return dest_str == reg1 || dest_str == reg2;
	}
	else if (instruction == "addi" || instruction == "ori" || instruction == "slti" || instruction == "andi") {
		string reg1 = currentLine.substr(currentLine.find(",")+1, currentLine.find_last_of(",") - (currentLine.find(",") + 1));

		return dest_str.compare(reg1) == 0;
	}
	else if (instruction == "bne" || instruction == "beq") {
		string reg1 = currentLine.substr(currentLine.find("$"), currentLine.find(",") - currentLine.find("$"));
		string reg2 = currentLine.substr(currentLine.find_last_of("$"), currentLine.find_last_of(",") - currentLine.find_last_of("$"));

		return dest_str.compare(reg1) == 0 || dest_str.compare(reg2) == 0;
	}
	
	return false;
}