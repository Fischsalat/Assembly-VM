#include <stdlib.h>
#include <cstring>
#include <exception>

#include "MemoryManager.h"


MemoryManager::MemoryManager()
	: MemBlock((uint8_t*)malloc(MemorySize)), CurrentCodePos(0), LoadedCodeSize(0), CurrentStackPos(CodeSegmentSize + StackSize)
{
}

MemoryManager::~MemoryManager()
{
	free(MemBlock);
}


void MemoryManager::LoadCodeSegment(uint8_t* Code, uint32_t Size)
{
	if (Size > CodeSegmentSize)
		throw std::exception("Size of code passed to LoadCodeSegment exceeds CodeSegmentSize!");

	memset(MemBlock, 0, LoadedCodeSize);

	LoadedCodeSize = Size;
	CurrentCodePos = 0;

	memcpy(MemBlock, Code, Size);
}

void MemoryManager::LoadDataSegment(uint8_t* Data, uint32_t Size)
{
	if (Size > DataSetgmentSize)
		throw std::exception("Size of data passed to LoadDataSegment exceeds DataSetgmentSize!");

	memset(MemBlock + CodeSegmentSize, 0, LoadedCodeSize);

	LoadedDataSize = Size;

	memcpy(MemBlock + CodeSegmentSize, Data, Size);
}

void MemoryManager::ReadInstructionsFromCode(void* OutData, int Size, bool bRead)
{
	if (bRead && OutData && (CurrentCodePos + Size) <= CodeSegmentSize && Size > 0)
	{
		memcpy(OutData, (MemBlock + CurrentCodePos), Size);

		CurrentCodePos += Size;
	}
}

void MemoryManager::JumpToPosition(uint32_t Pos)
{
	CurrentCodePos = Pos;
}

void MemoryManager::JumpToRelativePosition(int32_t RelativePos) 
{
	CurrentCodePos += RelativePos;
}

void MemoryManager::PushToStack(uint64_t RegContent, uint8_t Size)
{
	if ((CurrentStackPos - Size) < 0)
		return;

	CurrentStackPos -= Size;
	memcpy((MemBlock + CurrentStackPos), &RegContent, Size);
}

void MemoryManager::PopToReg(uint64_t& Register, uint8_t Size)
{
	if ((CurrentStackPos + Size) > StackSize)
		return;

	memcpy(&Register, (MemBlock + CurrentStackPos), Size);
	CurrentStackPos += Size;
}

MemoryManager& MemoryManager::Get()
{
	static MemoryManager Manager;

	return Manager;
}