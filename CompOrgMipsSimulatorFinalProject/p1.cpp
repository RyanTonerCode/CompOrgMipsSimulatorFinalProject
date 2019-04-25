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

using namespace std;

void print_line() {
	cout << "----------------------------------------------------------------------------------" << '\n';
}
void print_cycle(){
	cout << "CPU Cycles ===>     1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16" << endl;
}
void print_regs(int sRegs[8], int tRegs[10]){

	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 4; j++){
			cout << "$s" << i * 4 + j << " = " << sRegs[i * 4 + j];
			for (unsigned int k = 0; k < (13 - (std::to_string(sRegs[i * 4 + j])).length()); k++)
				std::cout << ' ';
		}
		cout << endl;
	}
	for (int i = 0; i < 3; i++) {
		
		for (int j = 0; j < 4 && i <= 1; j++) {
			std::cout << "$t" << i * 4 + j << " = " << tRegs[i * 4 + j];
			for (unsigned int k = 0; k < (13 - (std::to_string(tRegs[i * 4 + j])).length()); k++)
				std::cout << ' ';
		}
		for (int j = 0; j < 2 && i == 2; j++){
			std::cout << "$t" << i * 4 + j << " = " << tRegs[i * 4 + j];
			for (unsigned int k = 0; k < (13 - (std::to_string(tRegs[i * 4 + j])).length()); k++)
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

	//tRegs[0] = 1;
	//tRegs[1] = 1;
	//and_(&sRegs[0], &tRegs[0], &tRegs[1]);
	//cout << sRegs[0];

	if (!strcmp(argv[1], "F"))
	cout << "Start of Simulation (forwarding)" << endl;
	else
		cout << "Start of Simulations (no forwarding)" << endl;

	//bool forwarding = *argv[1] == 'F';
	//cout << forwarding << endl;

	ifstream inputstream(argv[2]);
	vector<string> instructions;
	string temp, temp2;
	int labelcount = 0;
	while(inputstream >> temp){
		if(temp.at(temp.length() - 2) == ':'){
			instructions.push_back(temp);
			labelcount++;
		} else {
			inputstream >> temp2;
			instructions.push_back(temp + ' ' + temp2);
		}
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
	unsigned int stackpointer = 0;								//this is which line we are on
	while(cycle != total){
		print_line();
		print_cycle();

		//STEP ALL STAGE FORWARD ONE
		//THIS is where all the EXECUTION AND WRITE BACK SHOULD HAPPEN
		//		-add, or, and, slt should happen in here!
		//		-bne, beq should happen here!
		//		-hazarding should be initially checked here, but processed in the later for loop
		for (unsigned int i = 0; i < pipeline.size(); i++) {
			//cout << pipeline[i][cycle - 1] << '\n';
			if (pipeline[i][cycle - 1] == 3) parse(pipeinstructions[i], sRegs, tRegs);
			if (pipeline[i][cycle - 1] < 4) pipeline[i][cycle] = pipeline[i][cycle-1] + 1;
		}

		//cout << "stepped\n";

		//ADD NEW PIPE FOR NEW INSTRUCTION READ-IN
		if (stackpointer < instructions.size() && instructions[stackpointer].at(instructions[stackpointer].length() - 2) == ':')
			while(stackpointer < instructions.size() && instructions[stackpointer].at(instructions[stackpointer].length() - 2) == ':') stackpointer++;

		//cout << "instruction found\n";
		if (stackpointer < instructions.size()) {
			pipeinstructions.push_back(instructions[stackpointer]);
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
			for(unsigned int j = 0; j < 16; j++){
				std::cout.width(5);
				cout << left << pipestages[pipeline[i][j]];
			}
			cout << endl;
		}

		cycle++;
		stackpointer++;
		print_regs(sRegs, tRegs);
	}


	cout << "----------------------------------------------------------------------------------\nEND OF SIMULATION\n";
}
