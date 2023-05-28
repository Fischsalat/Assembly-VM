#pragma once
#include <inttypes.h>
#include "Mnemonics.h"


// OperandSizeInfo may be unused by some instructions

void OpcodeAND(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data);
void OpcodeOR(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data);
void OpcodeXOR(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data);
void OpcodeNOT(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data);
void OpcodeNOR(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data);
void OpcodeNAND(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data);

