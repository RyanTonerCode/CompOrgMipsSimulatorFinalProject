#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "execution.h"

using namespace std;

void print_line() {
	cout << "----------------------------------------------------------------------------------" << '\n';
}
void print_cycle(){
	cout << "CPU Cycles ===>     1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16" << endl;
}
void print_regs(int sRegs[8], int tRegs[10]){

	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 4; j++)
			cout << "$s" << i * 4 + j << " = " << sRegs[i * 4 + j] << "\t\t";
		cout << endl;
	}
	for (int i = 0; i < 3; i++) {
		
		for (int j = 0; j < 4 && i <= 1; j++) 
			std::cout << "$t" << i * 4 + j << " = " << tRegs[i * 4 + j] << "\t\t";
		for (int j = 0; j < 2 && i == 2; j++)
			std::cout << "$t" << i * 4 + j << " = " << tRegs[i * 4 + j] << "\t\t";
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


	tRegs[0] = 1;
	tRegs[1] = 1;
	
	and_(&sRegs[0], &tRegs[0], &tRegs[1]);

	cout << sRegs[0];

	
	if (!strcmp(argv[1], "F"))
	cout << "Start of Simulation (forwarding)" << endl;
	else
		cout << "Start of Simulations (no forwarding)" << endl;

	bool forwarding = *argv[1] == 'F';
	cout << forwarding << endl;

	ifstream inputstream(argv[2]);
	vector<string> instructions;
	string temp, temp2;
	int labelcount = 0;
	while(inputstream >> temp){
		if(temp[temp.length] == ':'){
			instructions.push_back(temp);
			labelcount++;
		} else {
			inputstream >> temp2;
			instructions.push_back(temp + ' ' + temp2);
		}
	}
	
	vector<vector<int> > pipeline;
	vector<vector<string>> pipeinstructions;
	// for(unsigned int i = 0; i < instructions.size(); i++){
	// 	if(instructions[i][instructions[i].length] != ':'){
	// 		pipeline.push_back(vector<string>());
	// 		pipeline.back().push_back(instructions[i]);
	// 		for(unsigned int j = 0; j < 16; j++)
	// 			pipeline.back().push_back(".");
	// 	}
	// }
 
	string pipestages[7] = {"IF", "ID", "EX", "MEM", "WB", ".", "*" };
	unsigned int total = instructions.size() - labelcount + 4; //step where final instruction executes
	unsigned int cycle = 0;
	unsigned int stackpointer = 0;
	while(cycle != total){
		print_line();
		print_cycle();

		pipeinstructions.push_back(vector<string>());
		if(instructions[stackpointer][instructions[stackpointer].length] == ':')
			stackpointer++;
		//what is i here? pipeinstructions[i] = instructions[stackpointer];
		for(int i = 0; i < 16; i++)
			pipeline[i].push_back(5);

		for(int i = 0; i < cycle; i++){
			if(pipeline[cycle][17] == 'r'){
				//TODO if() DO CONTROL HAZARD HERE
			}
		}

		for(unsigned int i = 0; i <= pipeline.size(); i++){
			cout << pipeline[i][0];
			for(unsigned int j = 0; j < 16; j++){
				std::cout.width(4);
				cout << left << pipeline[i][j];
			}
			cout << endl;
		}
		cycle++;
		stackpointer++;
		print_regs(sRegs, tRegs);
	}



}
