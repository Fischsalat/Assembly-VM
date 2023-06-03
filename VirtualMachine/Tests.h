#pragma once
#include <cassert>
#include <vector>

#include <bitset>

#include "Mnemonics.h"
#include "Dispatcher.h"
#include "Registers.h"
#include "Opcode.h"

extern uint64_t GetDestinationData(EAddressingMode Mode, uint8_t* Data, uint8_t& OutByteCount);
extern uint64_t GetSourceData(EAddressingMode Mode, uint8_t* Data, OperandSizeInfo SizeInfo);

class Tests
{
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

		uint8_t OutByteSize = 0;
		assert(GetDestinationData(EAddressingMode::Immediate, Immediate, OutByteSize) == 0, "GetDestinationData can not handle Immediate destinations. Invalid!");
		assert(GetDestinationData(EAddressingMode::Register, Register + 1, OutByteSize) == 0x69, "Register was NOT 0x69");

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
		static auto SetRgisters = [](ERegisters Reg1, uint64_t Val1, ERegisters Reg2, uint64_t Val2) -> void
		{
			Registers.ZeroAll();
			*(reinterpret_cast<uint64_t*>(&Registers) + static_cast<uint8_t>(Reg1)) = Val1;
			*(reinterpret_cast<uint64_t*>(&Registers) + static_cast<uint8_t>(Reg2)) = Val2;
		};

		RegisterInfo DestRegInfo;
		DestRegInfo.RegisterIdx = 0x0;
		DestRegInfo.SubRegShiftCount = 0b10;
		DestRegInfo.bIsFloatRegister = false;

		RegisterInfo SrcRegInfo;
		SrcRegInfo.RegisterIdx = 0x3;
		SrcRegInfo.SubRegShiftCount = 0b10;
		SrcRegInfo.bIsFloatRegister = false;

		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::AND, EAddressingMode::Register, DestRegInfo, EAddressingMode::Register, SrcRegInfo, 0b11011 & 0b10101, { });
		
		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101); 
		Tests::TestOpcode<true>(Mnemonic::AND, EAddressingMode::Register, DestRegInfo, EAddressingMode::Immediate, 0b10101ull, 0b11011 & 0b10101, { });


		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::OR, EAddressingMode::Register, DestRegInfo, EAddressingMode::Register, SrcRegInfo, 0b11011 | 0b10101, { });
		
		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::OR, EAddressingMode::Register, DestRegInfo, EAddressingMode::Immediate, 0b10101ull, 0b11011 | 0b10101, { });


		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::XOR, EAddressingMode::Register, DestRegInfo, EAddressingMode::Register, SrcRegInfo, 0b11011 ^ 0b10101, { });
		
		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::XOR, EAddressingMode::Register, DestRegInfo, EAddressingMode::Immediate, 0b10101ull, 0b11011 ^ 0b10101, { });
	

		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<false>(Mnemonic::NOT, EAddressingMode::Register, DestRegInfo, EAddressingMode::None, 0, ~0b11011, { });

		SetRgisters(ERegisters::Q0, -1, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<false>(Mnemonic::NOT, EAddressingMode::Register, DestRegInfo, EAddressingMode::None, 0, 0, { EFlags::Zero });
	



		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<false>(Mnemonic::INC, EAddressingMode::Register, DestRegInfo, EAddressingMode::None, 0, 0b11011 + 1, { });

		SetRgisters(ERegisters::Q0, -1, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<false>(Mnemonic::INC, EAddressingMode::Register, DestRegInfo, EAddressingMode::None, 0, 0, { EFlags::Zero });


		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<false>(Mnemonic::DEC, EAddressingMode::Register, DestRegInfo, EAddressingMode::None, 0, 0b11011 - 1, { });

		SetRgisters(ERegisters::Q0, 1, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<false>(Mnemonic::DEC, EAddressingMode::Register, DestRegInfo, EAddressingMode::None, 0, 0, { EFlags::Zero });


		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::ADD, EAddressingMode::Register, DestRegInfo, EAddressingMode::Register, SrcRegInfo, 0b11011 + 0b10101, { });

		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::ADD, EAddressingMode::Register, DestRegInfo, EAddressingMode::Immediate, 0b10101ull, 0b11011 + 0b10101, { });


		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::SUB, EAddressingMode::Register, DestRegInfo, EAddressingMode::Register, SrcRegInfo, 0b11011 - 0b10101, { });

		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::SUB, EAddressingMode::Register, DestRegInfo, EAddressingMode::Immediate, 0b10101ull, 0b11011 - 0b10101, { });


		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::MUL, EAddressingMode::Register, DestRegInfo, EAddressingMode::Register, SrcRegInfo, 0b11011 * 0b10101, { });

		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::MUL, EAddressingMode::Register, DestRegInfo, EAddressingMode::Immediate, 0b10101ull, 0b11011 * 0b10101, { });


		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::DIV, EAddressingMode::Register, DestRegInfo, EAddressingMode::Register, SrcRegInfo, 0b11011 / 0b10101, { });

		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::DIV, EAddressingMode::Register, DestRegInfo, EAddressingMode::Immediate, 0b10101, 0b11011 / 0b10101, { });




		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::TEST, EAddressingMode::Register, DestRegInfo, EAddressingMode::Register, DestRegInfo, 0b11011, { });

		SetRgisters(ERegisters::Q0, 0b11011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::TEST, EAddressingMode::Register, DestRegInfo, EAddressingMode::Immediate, 0b00000, 0b11011, { EFlags::Zero });

		SetRgisters(ERegisters::Q0, 0b00011, ERegisters::Q3, 0b10101);
		Tests::TestOpcode<true>(Mnemonic::CMP, EAddressingMode::Register, DestRegInfo, EAddressingMode::Register, SrcRegInfo, 0b00011, { EFlags::Lower });

		SetRgisters(ERegisters::Q0, 0b00111, ERegisters::Q3, 0b000011);
		Tests::TestOpcode<true>(Mnemonic::CMP, EAddressingMode::Register, DestRegInfo, EAddressingMode::Immediate, 0b000011, 0b00111, { EFlags::Greater });

		SetRgisters(ERegisters::Q0, 0b00001, ERegisters::Q3, 0b000001);
		Tests::TestOpcode<true>(Mnemonic::CMP, EAddressingMode::Register, DestRegInfo, EAddressingMode::Immediate, 0b000001, 0b00001, { EFlags::Zero });
	}

private:
	template<bool bHasSrcOperand, typename DestType, typename SrcType>
	static void TestOpcode(Mnemonic M, EAddressingMode DestAddrMode, DestType Dest, EAddressingMode SrcAddrMode, SrcType Src, uint64_t Result, const std::vector<EFlags>& ExpectedFlags)
	{
		Operand Op;
		Op.bHasOperandSizeInfo = bHasSrcOperand;
		Op.DestAddressingMode = static_cast<uint8_t>(DestAddrMode);
		Op.Size = (sizeof(DestType) + (bHasSrcOperand ? sizeof(SrcType) : 0)) - 1;

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

		if constexpr (bHasSrcOperand)
		{
			Addon = sizeof(DestType) + ArrayInitialSize;
			for (int i = sizeof(SrcType) - 1; i >= 0; i--)
			{
				ByteScript[i + Addon] = *(reinterpret_cast<uint8_t*>(&Src) + i);
			}
		}

		ByteStream Bytecode(ByteScript, sizeof(ByteScript));

		Dispatcher::Dispatch(Bytecode);

		uint8_t ByteSize = 0;
		uint64_t DestinationDataResult = GetDestinationData(DestAddrMode, (uint8_t*)&Dest, ByteSize);
		assert(MaskValue(DestinationDataResult, ByteSize) == MaskValue(Result, ByteSize), "Instruction did not yield expected result!");

		for (auto Flag : ExpectedFlags)
		{
			assert(FL.IsFlagSet(Flag), "Expected flag was not set!");
		}
	}
};