#pragma once
#include <array>
#include <inttypes.h>

#include "Mnemonics.h"
#include "ByteStream.h"

class Dispatcher
{
private:
	static constexpr int MaxNumMnemonics = (uint8_t)Mnemonic::SYS + 1;

private:
	static std::array<void(*)(Operand OperantSpecifics, OperandSizeInfo SizeInfo, uint8_t* Data), MaxNumMnemonics> Mappings;

public:
	static void Init();

	static void Dispatch(ByteStream& Stream);
};