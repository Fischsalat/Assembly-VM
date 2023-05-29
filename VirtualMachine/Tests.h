#pragma once
#include <cassert>
#include <vector>

#include "Mnemonics.h"
#include "Dispatcher.h"
#include "Registers.h"

extern uint64_t GetDestinationData(EAddressingMode Mode, uint8_t* Data);
extern uint64_t GetSourceData(EAddressingMode Mode, uint8_t* Data, OperandSizeInfo SizeInfo);

class Tests
{
private:
	static uint8_t OperandSizeInfoBuilder()
	{

	}

public:
	static void TestGetDestinationData()
	{
		//Setup test registers
		Registers.ZeroAll();
		Registers.Q3 = 0x69;

		// EAddressingMode::Immediate
		uint8_t ImmediateBytes[] = { 0x0, 0x69, 0x0, 0x0, 0x0 };

		// EAddressingMode::Register
		uint8_t RegisterBytes[] = { 0b0, 0b011000011 }; // bIsFloat = false, RegByteCount = 7(+1), RegisterIdx = 3 [Q3] 

		uint8_t* Immediate = reinterpret_cast<uint8_t*>(ImmediateBytes);
		uint8_t* Register = reinterpret_cast<uint8_t*>(RegisterBytes);

		assert(GetDestinationData(EAddressingMode::Immediate, Immediate) == 0, "GetDestinationData can not handle Immediate destinations. Invalid!");
		assert(GetDestinationData(EAddressingMode::Register, Register + 1) == 0x69, "Register was NOT 0x69");

		OperandSizeInfo Info;
		Info.SrcAddressingMode = 0b00;
		Info.SrcSize = 0b011;
		Info.DestSize = 0b000;

		assert(GetSourceData(EAddressingMode::Immediate, Immediate, Info) == 0x69, "Immediate was NOT 0x69");
		assert(GetSourceData(EAddressingMode::Register, Register, Info) == 0x69, "Source-Register wasn't 0x69");
	}

	static void TestBytecodeExecutor()
	{
		Dispatcher::Init();

		// Prepare registers
		Registers.ZeroAll();
		Registers.Q0 = 0b11011;
		Registers.Q3 = 0b10101;

		uint8_t ByteScript[] =
		{
			/*
			* Opcode Operand OperandSizeInfo RegInfo Reginfo
			*
			* AND Q0, Q3
			*
			* AND 0b1|01|00010 0b0|10|00001 0b0|11|00000 0b0|11|00011
			*/
			0b00000000, 0b10100001, 0b01000000, 0b01100000, 0b01100011
		};

		ByteStream Bytecode(ByteScript, sizeof(ByteScript));

		Dispatcher::Dispatch(Bytecode);

		assert(Registers.Q0 == 0b10001, "Instructin AND [Reg, Reg] did not work!");
	}

	static void TestOpcodeExecution()
	{
		RegisterInfo DestRegInfo;
		DestRegInfo.RegisterIdx = 0x0;
		DestRegInfo.SubRegShiftCount = 0b111;
		DestRegInfo.bIsFloatRegister = false;

		RegisterInfo SrcRegInfo;
		SrcRegInfo.RegisterIdx = 0x3;
		SrcRegInfo.SubRegShiftCount = 0b111;
		SrcRegInfo.bIsFloatRegister = false;

		// Prepare registers
		Registers.ZeroAll();
		Registers.Q0 = 0b11011;
		Registers.Q3 = 0b10101;

		Tests::TestOpcode<true>(Mnemonic::OR, EAddressingMode::Register, DestRegInfo, EAddressingMode::Register, SrcRegInfo, 0b11011 | 0b10101, { EFlags::Overflow });
		Tests::TestOpcode<true>(Mnemonic::OR, EAddressingMode::Register, DestRegInfo, EAddressingMode::Immediate, 0b10101ull, 0b11011 | 0b10101, { EFlags::Overflow });
	}

private:
	template<bool bHasSrcOperand, typename DestType, typename SrcType>
	static void TestOpcode(Mnemonic M, EAddressingMode DestAddrMode, DestType Dest, EAddressingMode SrcAddrMode, SrcType Src, uint64_t Result, const std::vector<EFlags>& ExpectedFlags)
	{
		Operand Op;
		Op.bHasOperandSizeInfo = bHasSrcOperand;
		Op.DestAddressingMode = static_cast<uint8_t>(DestAddrMode);
		Op.Size = (sizeof(DestType) + sizeof(SrcType)) - 1;

		OperandSizeInfo OpInfo;
		OpInfo.DestSize = sizeof(DestType) - 1;
		OpInfo.SrcSize = sizeof(SrcType) - 1;
		OpInfo.SrcAddressingMode = static_cast<uint8_t>(SrcAddrMode);

		uint8_t MnemonicAsByte = static_cast<uint8_t>(M);
		uint8_t OperandAsByte = *reinterpret_cast<uint8_t*>(&Op);
		uint8_t SizeInfoAsByte = *reinterpret_cast<uint8_t*>(&OpInfo);

		constexpr int32_t ArraySize = sizeof(Mnemonic) + sizeof(Operand) + sizeof(DestType) + (bHasSrcOperand ? (sizeof(OperandSizeInfo) + sizeof(SrcType)) : 0);
		constexpr int32_t ArrayInitialSize = sizeof(Mnemonic) + sizeof(Operand) + (bHasSrcOperand ? sizeof(OperandSizeInfo) : 0);

		uint8_t ByteScript[ArraySize] = { MnemonicAsByte, OperandAsByte }; // initialSize = 0x3

		if constexpr (bHasSrcOperand)
			ByteScript[0x2] = SizeInfoAsByte;

		int Addon = ArrayInitialSize;
		for (int i = sizeof(DestType) - 1; i >= 0; i--)
		{
			ByteScript[i + Addon] = *(reinterpret_cast<uint8_t*>(&Dest) + i);
		}

		Addon = sizeof(DestType) + ArrayInitialSize;
		for (int i = sizeof(SrcType) - 1; i >= 0; i--)
		{
			ByteScript[i + Addon] = *(reinterpret_cast<uint8_t*>(&Src) + i);
		}

		ByteStream Bytecode(ByteScript, sizeof(ByteScript));

		Dispatcher::Dispatch(Bytecode);

		assert(GetDestinationData(DestAddrMode, (uint8_t*)&Dest) == Result, "Instructin did not yield expected result!");

		for (auto Flag : ExpectedFlags)
		{
			assert(FL.IsFlagSet(Flag), "Expected flag was not set!");
		}
	}
};