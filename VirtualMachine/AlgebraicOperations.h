#pragma once
#include <inttypes.h>
#include "Mnemonics.h"

// OperandSizeInfo may be unused by some instructions

void OpcodeINC(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data);
void OpcodeDEC(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data);
void OpcodeADD(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data);
void OpcodeSUB(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data);
void OpcodeMUL(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data);
void OpcodeDIV(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data);