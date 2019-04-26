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

	bool forwarding = *argv[1] == 'F';

	ifstream inputstream(argv[2]);
	vector<string> instructions;
	string temp, temp2;
	int labelcount = 0;
	int lineCount = 0;
	while(inputstream >> temp){
		if(isLabel(temp)){
			labelLine(temp, lineCount); //label set as next line
			instructions.push_back(temp);
			labelcount++;
		} else {
			inputstream >> temp2;
			instructions.push_back(temp + ' ' + temp2);
			lineCount++; //only increment for real lines
		}
	}

	vector<vector<int> > pipeline;	  //[instruction][cycle] = stage
	vector<string> pipeinstructions;  //[index]=instruction
 
	int controlLimit = -1;

	//8 begins jump stall
	string pipestages[12] = {"IF", "ID", "EX", "MEM", "WB", ".", "*", "*", "*", "*", "*", "*" };
	//pipeling contains a matrix of the stage cycle. The stage cycle is an index to pipestages to represent the chars.

	unsigned int total = instructions.size() - labelcount + 4;  //step where final instruction executes, updates as the program hazards
	unsigned int cycle = 0;										//this is which (out of 16) cycle we're on now
	unsigned int instructionIterator = 0;								//this is which line we are on
	while(cycle != total && cycle < 16){
		print_line();
		print_cycle();

		bool stallHazard = false;

		unsigned int pipelineSize = pipeline.size();

		//STEP ALL STAGE FORWARD ONE
		//THIS is where all the EXECUTION AND WRITE BACK SHOULD HAPPEN
		//		-add, or, and, slt should happen in here!
		//		-bne, beq should happen here!
		//		-hazarding should be initially checked here, but processed in the later for loop
		for (unsigned int i = 0; i < pipelineSize && (controlLimit == -1 || (int)i < controlLimit) && i < 16; i++) {
			//cout << pipeline[i][cycle - 1] << '\n';

			//calculate the amount of hazard offset needed
			unsigned int hazard_offset = 0;

			if (pipeline[i][cycle - 1] + 1 == 2 && pipeinstructions[i] != "nop") {//only on ID check for hazards

				int realLines = 0;

				//hazard detection (only when forwarding is false)
				for (int j = i - 1; j >= 0 && !forwarding; j--) {

					if (pipeinstructions[j] != "nop") {
						realLines++;

						unsigned int difference = 3 - (i - (i-realLines));

					    bool hazardFound = dataHazard(pipeinstructions[i], pipeinstructions[j]);

						if (hazardFound && difference > hazard_offset && realLines <= 2 && pipeline[j][cycle] != 5) {

							stallHazard = true;

							//cout << "asdfhasdgjas\n\n\n";

							hazard_offset = difference;
							if (pipeinstructions[i-1] != "nop") { //need nops
								if (hazard_offset == 2) {
									pipeinstructions.insert(pipeinstructions.begin() + i, "nop");
									pipeinstructions.insert(pipeinstructions.begin() + i, "nop");

									pipeline.insert(pipeline.begin() + i, vector<int>());
									pipeline.insert(pipeline.begin() + i, vector<int>());

									for (unsigned int k = 0; k < 16; k++) {
										if (k < cycle) {
											pipeline[i].push_back(5);
											pipeline[i + 1].push_back(5);
										}
										else {
											pipeline[i].push_back(5);
											pipeline[i + 1].push_back(5);
										}
									}
									pipeline[i][cycle - 2] = 0;
									pipeline[i][cycle - 1] = 1;
									pipeline[i][cycle] = 7;
									pipeline[i + 1][cycle - 2] = 0;
									pipeline[i + 1][cycle - 1] = 1;
									pipeline[i + 1][cycle] = 7;
								}
								else if (hazard_offset == 1) {
									pipeinstructions.insert(pipeinstructions.begin() + i, "nop");
									pipeline.insert(pipeline.begin() + i, vector<int>());
									for (unsigned int k = 0; k < 16; k++) {
										if (k < cycle) {
											pipeline[i].push_back(5);
										}
										else {
											pipeline[i].push_back(5);
										}
									}
									pipeline[i][cycle - 2] = 0;
									pipeline[i][cycle - 1] = 1;
									pipeline[i][cycle] = 7;

								}
								i += hazard_offset; //increment i so it actually points to the instruction still.
								pipelineSize += hazard_offset;
								total += hazard_offset;
							}
						}
						
					}
				}
			}

			if (hazard_offset == 0) { //DO NOT PARSE
				if (pipeinstructions[i] != "nop") {
					//Perform STEPPING here
					if (pipeline[i][cycle - 1] == 3) {
						bool jump = parse(pipeinstructions[i], sRegs, tRegs);
						if (jump) { //add all lines after label to pipe

							for (unsigned int j = i + 1; j < pipeinstructions.size(); j++) {
								pipeline[j][cycle] = 8 + (pipeline[j][cycle-1]); //set beginning of stall-land
							}

							int labelline = labelParse(pipeinstructions[i]);

							int curSize = pipeinstructions.size();
							if (controlLimit != -1)
								curSize = controlLimit;

							for (unsigned int j = labelline; j < instructions.size(); j++) {
								if (!isLabel(instructions[j])) {
									pipeinstructions.push_back(instructions[j]); //add all lines after label.
									total += 1;
									

									pipeline.push_back(vector<int>());
									for (int k = 0; k < 16; k++)
										pipeline[pipeline.size() - 1].push_back(5);
									//pipeline[pipeline.size() - 1][cycle + j - labelline - 1] = 0;

								}
							}
							pipeline[curSize][cycle] = 0;
							controlLimit = curSize++;
						}
					}
					if (pipeline[i][cycle] >= 8) {}//do nothing
					else
					if (pipeline[i][cycle - 1] < 4 && !stallHazard)
						pipeline[i][cycle] = pipeline[i][cycle - 1] + 1;
					else if (pipeline[i][cycle - 1] < 4 && stallHazard)
						pipeline[i][cycle] = pipeline[i][cycle - 1];
					else if (pipeline[i][cycle - 1] >= 8 && pipeline[i][cycle - 1]  < 11 && !stallHazard) {
						pipeline[i][cycle] = pipeline[i][cycle - 1] + 1;
					}

					int doublestall = i >= 2 && pipeline[i - 2][cycle] == 7;
					int checkcycle = 4 -  doublestall;

					if (i >= 1 && pipeline[i - 1][cycle] == 7 && pipeline[i][cycle] >= checkcycle && !stallHazard) {
						pipeline[i - 1][cycle] = 5;
					}
					if (i >= 2 && pipeline[i - 2][cycle] == 7 && pipeline[i][cycle] >= checkcycle && !stallHazard) {
						pipeline[i - 2][cycle] = 5;
					}
				}
				else {
					if (pipeline[i][cycle - 1] == 7)
						pipeline[i][cycle] = 7;
				}
			}
			else {
				if (pipeline[i][cycle - 1] + 1 == 1)
					pipeline[i][cycle] = 1;
				else
					pipeline[i][cycle] = pipeline[i][cycle - 1]; //stall
			}
		}

		//Update the line counts relative to the adjusted pipeline since hazards may have occured
		//unsigned int lineCount = 0;
		//while (lineCount < pipeline.size()) {
		//	if (isLabel(pipeinstructions[lineCount]))
		//		labelLine(temp, lineCount);
		//	lineCount++;
		//}

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

		if (controlLimit != -1 && controlLimit < (int)pipeline.size()) {
			pipeline[controlLimit][cycle] = 0;
		}

		//Print full new pipeline
		for(unsigned int i = 0; i < pipeline.size() && (controlLimit == -1 || (int)i <= controlLimit) && i < 16; i++){

			//print the current instruction
			cout << pipeinstructions[i];

			//add spacing
			for (unsigned int k = 0; k < 20 - pipeinstructions[i].length(); k++)
				cout << ' ';

			if (pipeinstructions[i] == "nop") {
				//manage printing nops around here: need IF and ID and * in single print
				if (pipeline[i][cycle - 1] == 6) { //this updating only happens ONCE!
					//here print IF and ID and *
					//pipeline[i][cycle - 3] = 0; //IF
					//pipeline[i][cycle - 2] = 1; //ID
					//pipeline[i][cycle - 1] = 7; //end star so it won't happen again
				}
			}

			for(unsigned int j = 0; j < 15; j++){
				std::cout.width(4);
				std::cout.fill(' ');

				//print the current step in the pipeline
				cout << left << pipestages[pipeline[i][j]];
			}
			cout << pipestages[pipeline[i][15]];
			cout << endl;
		}

		cout << endl;

		if (controlLimit != -1 && controlLimit < (int)pipeline.size()) {
			controlLimit++;
		}

		cycle++;
		instructionIterator++;
		print_regs(sRegs, tRegs);
	}


	cout << "----------------------------------------------------------------------------------\nEND OF SIMULATION\n";
}
