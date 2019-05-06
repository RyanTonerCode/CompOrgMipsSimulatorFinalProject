#ifndef execution_h
#define execution_h

#include <string>
using namespace std;

void li(int* destRegister, int immediate);

void la(int* destRegister, int address);

void syscall(int vRegs[2], int aRegs[4]);

void add_(int* destRegister, int* leftRegister, int* rightRegister);

void addi_(int* destRegister, int* leftRegister, int immediate);

void and_(int* destRegister, int* leftRegister, int* rightRegister);

void andi_(int* destRegister, int* leftRegister, int immediate);

void or_(int* destRegister, int* leftRegister, int* rightRegister);

void ori_(int* destRegister, int* leftRegister, int immediate);

void slt_(int* destRegister, int* leftRegister, int* rightRegister);

void slti_(int* destRegister, int* leftRegister, int immediate);

bool beq_(int* leftRegister, int* rightRegister);

bool bne_(int* leftRegister, int* rightRegister);

#endif 