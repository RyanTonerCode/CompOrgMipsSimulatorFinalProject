#include "printer.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

void print_start(bool forwarding) {
	if (forwarding)
		cout << "START OF SIMULATION (forwarding)" << endl;
	else
		cout << "START OF SIMULATION (no forwarding)" << endl;
}

void print_end() {
	print_line();
	cout << "END OF SIMULATION" << "\n";
}

//prints the dashes
void print_line() {
	cout << "----------------------------------------------------------------------------------" << '\n';
}

//prints cycle tabulation
void print_cycle() {
	cout << "CPU Cycles ===>     1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16" << endl;
}

//prints all the registers with their data
void print_regs(int sRegs[8], int tRegs[10]) {

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			cout << "$s" << i * 4 + j << " = " << sRegs[i * 4 + j];
			for (unsigned int k = 0; k < (14 - (to_string(sRegs[i * 4 + j])).length()); k++)
				cout << ' ';
		}
		cout << "$s" << i * 4 + 3 << " = " << sRegs[i * 4 + 3];
		cout << endl;
	}
	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 4 && i <= 1; j++) {
			cout << "$t" << i * 4 + j << " = " << tRegs[i * 4 + j];
			for (unsigned int k = 0; j != 3 && k < (14 - (to_string(tRegs[i * 4 + j])).length()); k++)
				cout << ' ';
		}
		for (int j = 0; j < 2 && i == 2; j++) {
			cout << "$t" << i * 4 + j << " = " << tRegs[i * 4 + j];
			for (unsigned int k = 0; j != 1 && k < (14 - (to_string(tRegs[i * 4 + j])).length()); k++)
				cout << ' ';
		}
		cout << endl;
	}
}

//pipeling contains a matrix of the stage cycle. The stage cycle is an index to pipestages to represent the chars.
//0-4 are valid pipeline stages
//5 is nothing
//6 represents stalling
//7 begins jump stall, and next cases tell how many stalls need to go.
string pipestages[11] = { "IF", "ID", "EX", "MEM", "WB", ".",  "*", "*", "*", "*", "*" };

void print_pipeline(vector<string> pipeinstructions, vector<vector<int>> pipeline, int controlLimit) {
	//Print full new pipeline
	for (unsigned int i = 0; i < pipeline.size() && (controlLimit == -1 || (int)i <= controlLimit) && i < 16; i++) {

		//print the current instruction
		cout << pipeinstructions[i];

		//add spacing
		for (unsigned int k = 0; k < 20 - pipeinstructions[i].length(); k++)
			cout << ' ';

		for (unsigned int j = 0; j < 15; j++) {
			cout.width(4);
			cout.fill(' ');

			//print the current step in the pipeline
			cout << left << pipestages[pipeline[i][j]];
		}
		cout << pipestages[pipeline[i][15]] << endl;
	}

	cout << endl;
}