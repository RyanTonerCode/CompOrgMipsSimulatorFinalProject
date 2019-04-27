#ifndef printer_h
#define printer_h
#include <string>
#include <vector>
using namespace std;

void print_start(bool forwarding);

void print_end();

void print_line();

void print_cycle();

void print_regs(int sRegs[8], int tRegs[10]);

void print_pipeline(vector<string> pipeinstructions, vector<vector<int>> pipeline, int controlLimit);

#endif