#include <algorithm>

#include "BitwiseOperations.h"
#include "Opcode.h"


void OpcodeAND(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto AND = [](uint64_t DestinationData, uint64_t SourceData) -> uint64_t
	{
		return DestinationData & SourceData;
	};

	static auto SetFlags = [](uint64_t Dest, uint64_t Src, uint64_t Result, uint8_t ByteCount) -> void
	{
		if (Result == 0)
			FL.SetFlags(EFlags::Zero);
	};

	Opcode(OperantSpecifics, SizeInfo, Data, AND, SetFlags);
}

void OpcodeOR(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto OR = [](uint64_t DestinationData, uint64_t SourceData) -> uint64_t
	{
		return DestinationData | SourceData;
	};

	static auto SetFlags = [](uint64_t Dest, uint64_t Src, uint64_t Result, uint8_t ByteCount) -> void
	{
		uint64_t HighestBit = GetHighestBitSet(ByteCount);

		if (Result == 0)
			FL.SetFlags(EFlags::Zero);

		if ((Dest & HighestBit) && (Src & HighestBit))
			FL.SetFlags(EFlags::Overflow);
	};

	Opcode(OperantSpecifics, SizeInfo, Data, OR, SetFlags);
}

void OpcodeXOR(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto XOR = [](uint64_t DestinationData, uint64_t SourceData) -> uint64_t
	{
		return DestinationData ^ SourceData;
	};

	static auto SetFlags = [](uint64_t Dest, uint64_t Src, uint64_t Result, uint8_t ByteCount) -> void
	{
		if (Result == 0)
			FL.SetFlags(EFlags::Zero);
	};

	Opcode(OperantSpecifics, SizeInfo, Data, XOR, SetFlags);
}

void OpcodeNOT(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto NOT = [](uint64_t DestinationData) -> uint64_t
	{
		return ~DestinationData;
	};

	static auto SetFlags = [](uint64_t Dest, uint64_t Result, uint8_t ByteCount) -> void
	{
		if (Result == 0)
			FL.SetFlags(EFlags::Zero);
	};

	SingleOperandOpcode(OperantSpecifics, SizeInfo, Data, NOT, SetFlags);
}

void OpcodeNOR(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto NOR = [](uint64_t DestinationData, uint64_t SourceData) -> uint64_t
	{
		return ~(DestinationData | SourceData);
	};

	static auto SetFlags = [](uint64_t Dest, uint64_t Src, uint64_t Result, uint8_t ByteCount) -> void
	{
		Result = MaskValue(Result, ByteCount);

		if (Result == 0)
			FL.SetFlags(EFlags::Zero);
	};

	Opcode(OperantSpecifics, SizeInfo, Data, NOR, SetFlags);
}

void OpcodeNAND(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto NAND = [](uint64_t DestinationData, uint64_t SourceData) -> uint64_t
	{
		return ~(DestinationData & SourceData);
	};

	static auto SetFlags = [](uint64_t Dest, uint64_t Src, uint64_t Result, uint8_t ByteCount) -> void
	{
		if (Result == 0)
			FL.SetFlags(EFlags::Zero);
	};

	Opcode(OperantSpecifics, SizeInfo, Data, NAND, SetFlags);
}