#pragma once
#include "Dispatcher.h"
#include "BitwiseOperations.h"

std::array<void(*)(Operand, OperandSizeInfo, uint8_t*), Dispatcher::MaxNumMnemonics> Dispatcher::Mappings;


void Dispatcher::Init()
{
	Mappings[(uint8_t)Mnemonic::AND] = OpcodeAND;
	Mappings[(uint8_t)Mnemonic::OR] = OpcodeOR;
	Mappings[(uint8_t)Mnemonic::XOR] = OpcodeXOR;
	Mappings[(uint8_t)Mnemonic::NOT] = OpcodeNOT;
	Mappings[(uint8_t)Mnemonic::NOR] = OpcodeNOR;
	Mappings[(uint8_t)Mnemonic::NAND] = OpcodeNAND;

	Mappings[(uint8_t)Mnemonic::INC] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::DEC] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::ADD] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::SUB] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::MUL] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::DIV] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::TEST] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::CMP] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::JMP] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::JZ] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::JNZ] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::JE] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::JNE] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::JG] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::JGE] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::JL] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::JLE] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::JO] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::JNO] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::LEA] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::MOV] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::CMOVZ] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::CMOVNZ] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::CMOVE] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::CMOVNE] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::CMOVG] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::CMOVGE] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::CMOVL] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::CMOVLE] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::CMOVO] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::CMOVNO] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::PUSH] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
	Mappings[(uint8_t)Mnemonic::POP] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::RET] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::NOP] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::CALL] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };

	Mappings[(uint8_t)Mnemonic::SYS] = [](Operand, OperandSizeInfo, uint8_t*) -> void { return; };
}

extern void ANDImpl(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data);

void Dispatcher::Dispatch(ByteStream& Stream)
{
	static_assert(sizeof(Operand) == sizeof(uint8_t));
	static_assert(sizeof(OperandSizeInfo) == sizeof(uint8_t));

	uint8_t Opcode = 0x0;
	Operand OperandSpecifics;
	OperandSizeInfo SizeInfo;

	Stream.Read(&Opcode, sizeof(uint8_t));
	Stream.Read(&OperandSpecifics, sizeof(uint8_t));
	Stream.ReadSamePos(&SizeInfo, sizeof(uint8_t));
	Stream.IncrementPos(OperandSpecifics.HasSizeInfoByte());

	uint8_t Buffer[0x20] = { 0x0 };
	Stream.Read(Buffer, OperandSpecifics.GetNumBytes());

	Mappings[Opcode](OperandSpecifics, SizeInfo, Buffer);
}