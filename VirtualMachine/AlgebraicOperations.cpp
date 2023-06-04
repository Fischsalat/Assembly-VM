#pragma once
#include "AlgebraicOperations.h"
#include "Opcode.h"


void OpcodeINC(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto INC = [](uint64_t DestinationData) -> uint64_t
	{
		return (DestinationData + 1);
	};

	static auto SetFlags = [](uint64_t Dest, uint64_t Result, uint8_t ByteCount) -> void
	{
		if (Result == 0)
			FL.SetFlags(EFlags::Zero | EFlags::Overflow);

	};

	SingleOperandOpcode(OperantSpecifics, SizeInfo, Data, INC, SetFlags);
}

void OpcodeDEC(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto DEC = [](uint64_t DestinationData) -> uint64_t
	{
		return (DestinationData - 1);
	};

	static auto SetFlags = [](uint64_t Dest, uint64_t Result, uint8_t ByteCount) -> void
	{
		if (Result == 0)
			FL.SetFlags(EFlags::Zero);

	};

	SingleOperandOpcode(OperantSpecifics, SizeInfo, Data, DEC, SetFlags);
}


void OpcodeADD(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto ADD = [](uint64_t DestinationData, uint64_t SourceData) -> uint64_t
	{
		return DestinationData + SourceData;
	};

	static auto SetFlags = [](uint64_t Dest, uint64_t Src, uint64_t Result, uint8_t ByteCount) -> void
	{
		if (Result == 0)
			FL.SetFlags(EFlags::Zero);

		if (Result < Dest || Result < Src)
			FL.SetFlags(EFlags::Overflow);

	};

	Opcode(OperantSpecifics, SizeInfo, Data, ADD, SetFlags);
}

void OpcodeSUB(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto SUB = [](uint64_t DestinationData, uint64_t SourceData) -> uint64_t
	{
		return DestinationData - SourceData;
	};

	static auto SetFlags = [](uint64_t Dest, uint64_t Src, uint64_t Result, uint8_t ByteCount) -> void
	{
		if (Result == 0)
			FL.SetFlags(EFlags::Zero);
	};

	Opcode(OperantSpecifics, SizeInfo, Data, SUB, SetFlags);
}

void OpcodeMUL(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto MUL = [](uint64_t DestinationData, uint64_t SourceData) -> uint64_t
	{
		return DestinationData * SourceData;
	};

	static auto SetFlags = [](uint64_t Dest, uint64_t Src, uint64_t Result, uint8_t ByteCount) -> void
	{
		if (Dest != 0 && (Result / Dest) != Src)
			FL.SetFlags(EFlags::Overflow);

		if (Result == 0)
			FL.SetFlags(EFlags::Zero);
	};

	Opcode(OperantSpecifics, SizeInfo, Data, MUL, SetFlags);
}

void OpcodeDIV(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto DIV = [](uint64_t DestinationData, uint64_t SourceData) -> uint64_t
	{
		if (SourceData == 0)
			SourceData = 1;

		return DestinationData / SourceData;
	};

	static auto SetFlags = [](uint64_t Dest, uint64_t Src, uint64_t Result, uint8_t ByteCount) -> void
	{
		if (Result == 0)
			FL.SetFlags(EFlags::Zero);
	};

	Opcode(OperantSpecifics, SizeInfo, Data, DIV, SetFlags);
}

void OpcodeMOD(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto MOD = [](uint64_t DestinationData, uint64_t SourceData) -> uint64_t
	{
		return DestinationData % SourceData;
	};

	static auto SetFlags = [](uint64_t Dest, uint64_t Src, uint64_t Result, uint8_t ByteCount) -> void
	{
		if (Result == 0)
			FL.SetFlags(EFlags::Zero);
	};

	Opcode(OperantSpecifics, SizeInfo, Data, MOD, SetFlags);
}