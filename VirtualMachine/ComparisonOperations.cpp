#pragma once
#include "ComparisonOperations.h"

#include "Opcode.h"



void OpcodeTEST(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto SetFlags = [](uint64_t Dest, uint64_t Src, uint8_t ByteCount) -> void
	{
		uint64_t Result = Dest & Src;

		Result = MaskValue(Result, ByteCount);

		if (Result == 0)
			FL.SetFlags(EFlags::Zero);
	};

	FlagsOnlyOpcode(OperantSpecifics, SizeInfo, Data, SetFlags);
}

void OpcodeCMP(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto SetFlags = [](uint64_t Dest, uint64_t Src, uint8_t ByteCount) -> void
	{
		Dest = MaskValue(Dest, ByteCount);
		Src = MaskValue(Src, ByteCount);

		int64_t SignedResult = static_cast<int64_t>(Dest - Src);

		FL.SetFlags(SignedResult == 0 ? EFlags::Zero : SignedResult > 0 ? EFlags::Greater : EFlags::Lower);
	};

	FlagsOnlyOpcode(OperantSpecifics, SizeInfo, Data, SetFlags);
}