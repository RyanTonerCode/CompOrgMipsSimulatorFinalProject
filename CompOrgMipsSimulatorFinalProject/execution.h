#ifndef execution
#define execution
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