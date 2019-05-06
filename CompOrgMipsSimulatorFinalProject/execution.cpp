#include "execution.h"
#include <string>
#include <iostream>
#include <regex>
#include "data_parser.h"
#include "printer.h"
using namespace std;

void li(int* destRegister, int immediate) {
	*destRegister = immediate;
}

void la(int* destRegister, int address) {
	*destRegister = address;
}

void syscall(int vRegs[2], int aRegs[4]) {
	if (vRegs[0] == 4) { //print string
		int address = aRegs[0]; //get the address of the string in memory
		string strFromAddress = getStringFromAddress(address);
		if (strFromAddress == "\\n") {
			sysout("\n");
		}
		else
			sysout(strFromAddress); //deference the pointer to print the actual string by calling the operating system
	}
}

void add_(int* destRegister, int* leftRegister, int* rightRegister) {
	*destRegister = *leftRegister + *rightRegister;
}

void addi_(int* destRegister, int* leftRegister, int immediate) {
	*destRegister = *leftRegister + immediate;
}

void and_(int* destRegister, int* leftRegister, int* rightRegister) {
	*destRegister = *leftRegister & *rightRegister;
}

void andi_(int* destRegister, int* leftRegister, int immediate) {
	*destRegister = *leftRegister & immediate;
}

void or_(int* destRegister, int* leftRegister, int* rightRegister) {
	*destRegister = *leftRegister | *rightRegister;
}

void ori_(int* destRegister, int* leftRegister, int immediate) {
	*destRegister = *leftRegister | immediate;
}

void slt_(int* destRegister, int* leftRegister, int* rightRegister) {
	*destRegister = *leftRegister < *rightRegister;
}

void slti_(int* destRegister, int* leftRegister, int immediate) {
	*destRegister = *leftRegister < immediate;
}

bool beq_(int* leftRegister, int* rightRegister) {
	return *leftRegister == *rightRegister;
}


bool bne_(int* leftRegister, int* rightRegister) {
	return *leftRegister != *rightRegister;
}
