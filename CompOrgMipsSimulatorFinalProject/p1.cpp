#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <math.h>

#include "execution.h"
#include "parser.h"
#include "hazard_parser.h"
#include "pipeline.h"

using namespace std;

void print_line() {
	cout << "----------------------------------------------------------------------------------" << '\n';
}
void print_cycle(){
	cout << "CPU Cycles ===>     1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16" << endl;
}
void print_regs(int sRegs[8], int tRegs[10]){

	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 3; j++){
			cout << "$s" << i * 4 + j << " = " << sRegs[i * 4 + j];
			for (unsigned int k = 0; k < (14 - (std::to_string(sRegs[i * 4 + j])).length()); k++)
				std::cout << ' ';
		}
		cout << "$s" << i * 4 + 3 << " = " << sRegs[i * 4 + 3];
		cout << endl;
	}
	for (int i = 0; i < 3; i++) {
		
		for (int j = 0; j < 4 && i <= 1; j++) {
			std::cout << "$t" << i * 4 + j << " = " << tRegs[i * 4 + j];
			for (unsigned int k = 0; j != 3 && k < (14 - (std::to_string(tRegs[i * 4 + j])).length()); k++)
				std::cout << ' ';
		}
		for (int j = 0; j < 2 && i == 2; j++){
			std::cout << "$t" << i * 4 + j << " = " << tRegs[i * 4 + j];
			for (unsigned int k = 0; j != 1 && k < (14 - (std::to_string(tRegs[i * 4 + j])).length()); k++)
				std::cout << ' ';
		}
		cout << endl;
	}
}
int main(int argc, char* argv[])
{
	int tRegs[10];
	for(int i = 0; i < 10; i++)
		tRegs[i] = 0;
	int sRegs[8];
	for(int i = 0; i < 8; i++)
		sRegs[i] = 0; 

	if (!strcmp(argv[1], "F"))
		cout << "START OF SIMULATION (forwarding)" << endl;
	else
		cout << "START OF SIMULATION (no forwarding)" << endl;

	//currently unused variable, commenting it out for -Wall
	//bool forwarding = *argv[1] == 'F';
	//cout << forwarding << endl;

	ifstream inputstream(argv[2]);
	vector<string> instructions;
	string temp, temp2;
	int labelcount = 0;
	int lineCount = 0;
	while(inputstream >> temp){
		if(isLabel(temp)){
			//indicates a label
			labelLine(temp, lineCount);
			instructions.push_back(temp);
			labelcount++;
		} else {
			inputstream >> temp2;
			instructions.push_back(temp + ' ' + temp2);
		}
		lineCount++;
	}

	//for (unsigned int i = 0; i < instructions.size(); i++)
		//cout << instructions[i] << '\n';

	//cout << "made it to pipeline's start\n";

	vector<vector<int> > pipeline;					//A vector-vector of the cycles' stage
	vector<string> pipeinstructions;		//A vector of the pipe's instruction

	// for(unsigned int i = 0; i < instructions.size(); i++){
	// 	if(instructions[i][instructions[i].length] != ':'){
	// 		pipeline.push_back(vector<string>());
	// 		pipeline.back().push_back(instructions[i]);
	// 		for(unsigned int j = 0; j < 16; j++)
	// 			pipeline.back().push_back(".");
	// 	}
	// }
 
	string pipestages[7] = {"IF", "ID", "EX", "MEM", "WB", ".", "*" };
	//pipeling contains a matrix of the stage cycle. The stage cycle is an index to pipestages to represent the chars.

	unsigned int total = instructions.size() - labelcount + 4;  //step where final instruction executes, updates as the program hazards
	unsigned int cycle = 0;										//this is which (out of 16) cycle we're on now
	unsigned int instructionIterator = 0;								//this is which line we are on
	while(cycle != total && cycle < 17){
		print_line();
		print_cycle();

		//STEP ALL STAGE FORWARD ONE
		//THIS is where all the EXECUTION AND WRITE BACK SHOULD HAPPEN
		//		-add, or, and, slt should happen in here!
		//		-bne, beq should happen here!
		//		-hazarding should be initially checked here, but processed in the later for loop
		for (unsigned int i = 0; i < pipeline.size(); i++) {
			//cout << pipeline[i][cycle - 1] << '\n';

			//calculate the amount of hazard offset needed
			int hazard_offset = 0;
			//hazard detection
			for (int j = i - 1; j >= 0; j--) {
				//cout << "uh oh";
				int difference = i - j;
				if (dataHazard(pipeinstructions[i], pipeinstructions[j]) && difference > hazard_offset) {
					hazard_offset = difference;
					if (hazard_offset >= 2)
						pipeinstructions.insert(pipeinstructions.begin() + i - 1, "NOP");
					if(hazard_offset >= 1)
						pipeinstructions.insert(pipeinstructions.begin() + i - 1, "NOP");
				}
				//cout << "\nwoohoo\n";
			}

			//Perform STEPPING here
			if (pipeline[i][cycle - 1] == 3) parse(pipeinstructions[i], sRegs, tRegs);
			if (pipeline[i][cycle - 1] < 4) pipeline[i][cycle] = pipeline[i][cycle-1] + 1;
		}

		//cout << "stepped\n";

		//ADD NEW PIPE FOR NEW INSTRUCTION READ-IN
		while(instructionIterator < instructions.size() && isLabel(instructions[instructionIterator])) 
			instructionIterator++;

		//cout << "instruction found\n";
		if (instructionIterator < instructions.size()) {
			pipeinstructions.push_back(instructions[instructionIterator]);
			pipeline.push_back(vector<int>());

			//cout << "push backed a pipeline\n";

			//what is i here? pipeinstructions[i] = instructions[stackpointer];
			for (int i = 0; i < 16; i++)
				pipeline[pipeline.size() - 1].push_back(5);
			pipeline[pipeline.size() - 1][cycle] = 0;
		}

		for(unsigned int i = 0; i < pipeline.size(); i++){
			if(pipeline[i][cycle] == 6){
				//TODO if() DO CONTROL HAZARD HERE
				//note: "6" may not indicate just a control hazard, it can represent a data hazard for non-forwarding
			}
		}

		//cout << "past hazard\n";

		//Print full new pipeline
		for(unsigned int i = 0; i < pipeline.size(); i++){
			cout << pipeinstructions[i];
			for (unsigned int k = 0; k < 20 - pipeinstructions[i].length(); k++)
				cout << ' ';
			for(unsigned int j = 0; j < 15; j++){
				std::cout.width(4);
				std::cout.fill(' ');
				cout << left << pipestages[pipeline[i][j]];
			}
			cout << pipestages[pipeline[i][15]];
			cout << endl;
		}

		cout << endl;

		cycle++;
		instructionIterator++;
		print_regs(sRegs, tRegs);
	}


	cout << "----------------------------------------------------------------------------------\nEND OF SIMULATION\n";
}
