#include "BitwiseOperations.h"
#include "Registers.h"

#include <algorithm>


template<typename T>
inline T GetRegisterData(RegisterInfo Info)
{
	return *(T*)(reinterpret_cast<uint64_t*>(&Registers) + Info.GetRegIdx());
}

inline uint64_t GetDestinationData(EAddressingMode Mode, uint8_t* Data)
{
	if (Mode == EAddressingMode::Register)
	{
		RegisterInfo RegInfo = *reinterpret_cast<RegisterInfo*>(Data);

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

void OpcodeAND(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	uint8_t* PtrBackup = Data;

	uint64_t DestData = GetDestinationData(SizeInfo.GetSrcAddressingMode(), Data);
	uint64_t SrcData = GetSourceData(OperantSpecifics.GetDestAddressingMode(), Data, SizeInfo);

	uint64_t Res = DestData & SrcData;

	if (Res == 0)
		FL.SetFlags(EFlags::Zero);

	SetDestOperand(OperantSpecifics, SizeInfo, Data, &Res);
}

void Opcode
(
	Operand OperantSpecifics, 
	OperandSizeInfo SizeInfo, 
	uint8_t* Data, 
	uint64_t(*Action)(uint64_t Dest, uint64_t Src), 
	void(*SetFlags)(uint64_t Dest, uint64_t Src, uint64_t Result, uint8_t HighestBit)
)
{
	uint8_t* PtrBackup = Data;

	uint64_t DestData = GetDestinationData(OperantSpecifics.GetDestAddressingMode(), Data);
	uint64_t SrcData = GetSourceData(SizeInfo.GetSrcAddressingMode(), Data, SizeInfo);

	uint64_t Result = Action(DestData, SrcData);

	SetFlags(DestData, SrcData, Result, SizeInfo.GetDestinationHighestBit());

	SetDestOperand(OperantSpecifics, SizeInfo, Data, &Result);
}

void ANDImpl(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{
	static auto AND = [](uint64_t DestinationData, uint64_t SourceData) -> uint64_t
	{
		return DestinationData & SourceData;
	};

	static auto SetFlags = [](uint64_t Dest, uint64_t Src, uint64_t Result, uint8_t HighestBit) -> void
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

	static auto SetFlags = [](uint64_t Dest, uint64_t Src, uint64_t Result, uint8_t HighestBit) -> void
	{
		if (Result == 0)
			FL.SetFlags(EFlags::Zero);

		if ((Dest & HighestBit) && (Src & HighestBit))
			FL.SetFlags(EFlags::Overflow);
	};

	Opcode(OperantSpecifics, SizeInfo, Data, OR, SetFlags);
}

void OpcodeXOR(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{

}

void OpcodeNOT(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{

}

void OpcodeNOR(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{

}

void OpcodeNAND(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data)
{

}