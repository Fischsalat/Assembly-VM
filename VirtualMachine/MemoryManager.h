#pragma once
#include <stdint.h>

class MemoryManager
{
private:
	static constexpr uint32_t CodeSegmentSize = 0x2000;
	static constexpr uint32_t DataSetgmentSize = 0x1000;
	static constexpr uint32_t StackSize = 0x1000;
	static constexpr uint32_t HeapSize = 0x500;

	static constexpr uint32_t MemorySize = CodeSegmentSize + DataSetgmentSize + StackSize + HeapSize;

private:
	uint8_t* MemBlock;
	uint32_t CurrentCodePos;
	uint32_t LoadedCodeSize;
	uint32_t LoadedDataSize;
	uint32_t CurrentStackPos;
	//HeapHere;

public:
	MemoryManager(const MemoryManager&) = delete;
	MemoryManager operator=(const MemoryManager&) = delete;

private:
	MemoryManager();
	~MemoryManager();

public:
	void LoadCodeSegment(uint8_t* Code, uint32_t Size);
	void LoadDataSegment(uint8_t* Data, uint32_t Size);

	void ReadInstructionsFromCode(void* OutData, int Size, bool bRead = true);

	void JumpToPosition(uint32_t Pos);
	void JumpToRelativePosition(int32_t RelativePos);


	void PushToStack(uint64_t RegContent, uint8_t Size);

	void PopToReg(uint64_t& Register, uint8_t Size);

public:
	static MemoryManager& Get();
};

