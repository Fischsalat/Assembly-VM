#include <bitset>
#include <iostream>

#include "BitwiseOperations.h"
#include "Registers.h"

inline uint64_t GetDestinationData(EAddressingMode Mode, uint8_t* Data, uint8_t& OutByteCount)
{
	if (Mode == EAddressingMode::Register)
	{
		RegisterInfo RegInfo = *reinterpret_cast<RegisterInfo*>(Data);

		OutByteCount = RegInfo.GetSubregByteCount();

		return *(reinterpret_cast<uint64_t*>(&Registers) + RegInfo.GetRegIdx());
	}
	else if (Mode == EAddressingMode::Memory)
	{
		// not implemented yet
	}
	else if (Mode == EAddressingMode::Relative)
	{
		// not implemented yet
	}

	return 0;
}

inline uint64_t GetSourceData(EAddressingMode Mode, uint8_t* Data, OperandSizeInfo SizeInfo)
{
	if (Mode == EAddressingMode::Register)
	{
		RegisterInfo RegInfo = *reinterpret_cast<RegisterInfo*>(Data + SizeInfo.GetDestinationSizeBytes());

		return *(reinterpret_cast<uint64_t*>(&Registers) + RegInfo.GetRegIdx());
	}
	else if (Mode == EAddressingMode::Immediate)
	{
		uint64_t OutValue = 0;
		memcpy(&OutValue, Data + SizeInfo.GetDestinationSizeBytes(), SizeInfo.GetSourceSizeBytes());

		return OutValue;
	}
	else if (Mode == EAddressingMode::Memory)
	{
		// not implemented yet
	}
	else if (Mode == EAddressingMode::Relative)
	{
		// not implemented yet
	}

	return 0;
}


inline void SetDestOperand(Operand Op, OperandSizeInfo SizeInfo, uint8_t* Data, void* WritebackData)
{
	static_assert(sizeof(RegisterInfo) == sizeof(uint8_t));

	if (Op.GetDestAddressingMode() == EAddressingMode::Register)
	{
		RegisterInfo RegInfo = *reinterpret_cast<RegisterInfo*>(Data);

		memcpy(&Registers + RegInfo.GetRegIdx(), WritebackData, RegInfo.GetSubregByteCount());
	}
}

//(ByteCount << 3) == BitCount
inline uint64_t MaskValue(uint64_t Value, uint8_t ByteCount)
{
	return Value & (0xFFFFFFFFFFFFFFFFull >> ((8 - ByteCount) << 3));
}

//(ByteCount << 3) == BitCount
inline uint64_t GetHighestBitSet(uint64_t ByteCount)
{
	return 1ull << ((ByteCount << 3) - 1);
}

inline void Opcode
(
	Operand OperantSpecifics,
	OperandSizeInfo SizeInfo,
	uint8_t* Data,
	uint64_t(*Action)(uint64_t Dest, uint64_t Src),
	void(*SetFlags)(uint64_t Dest, uint64_t Src, uint64_t Result, uint8_t ByteCount)
)
{
	uint8_t DestinationByteCount = 0;
	uint64_t DestData = GetDestinationData(OperantSpecifics.GetDestAddressingMode(), Data, DestinationByteCount);
	uint64_t SrcData = GetSourceData(SizeInfo.GetSrcAddressingMode(), Data, SizeInfo);

	uint64_t Result = Action(DestData, SrcData);

	SetFlags(DestData, SrcData, MaskValue(Result, DestinationByteCount), DestinationByteCount);

	SetDestOperand(OperantSpecifics, SizeInfo, Data, &Result);
}

inline void SingleOperandOpcode
(
	Operand OperantSpecifics,
	OperandSizeInfo SizeInfo,
	uint8_t* Data,
	uint64_t(*Action)(uint64_t Dest),
	void(*SetFlags)(uint64_t Dest, uint64_t Result, uint8_t ByteCount)
)
{
	uint8_t DestinationByteCount = 0;
	uint64_t DestData = GetDestinationData(OperantSpecifics.GetDestAddressingMode(), Data, DestinationByteCount);

	uint64_t Result = Action(DestData);

	SetFlags(DestData, MaskValue(Result, DestinationByteCount), DestinationByteCount);

	SetDestOperand(OperantSpecifics, SizeInfo, Data, &Result);
}

inline void FlagsOnlyOpcode
(
	Operand OperantSpecifics,
	OperandSizeInfo SizeInfo,
	uint8_t* Data,
	void(*SetFlags)(uint64_t Dest, uint64_t Src, uint8_t ByteCount)
)
{
	uint8_t DestinationByteCount = 0;
	uint64_t DestData = GetDestinationData(OperantSpecifics.GetDestAddressingMode(), Data, DestinationByteCount);
	uint64_t SrcData = GetSourceData(SizeInfo.GetSrcAddressingMode(), Data, SizeInfo);

	SetFlags(DestData, SrcData, DestinationByteCount);
}

