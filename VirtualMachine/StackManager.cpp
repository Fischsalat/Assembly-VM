#include "StackManager.h"

StackManager::StackManager()
	: Stack((uint8_t*)malloc(InitialStackSize)), StackSize(InitialStackSize)
{
}

StackManager::~StackManager()
{
	free(Stack);
}

void StackManager::PushToStack(uint64_t RegContent, uint8_t Size)
{
	if ((StackSize - Size) < 0)
		return;

	StackSize -= Size;
	memcpy((Stack + StackSize), &RegContent, Size);
}

void StackManager::PopToReg(uint64_t& Register, uint8_t Size)
{
	if ((StackSize + Size) > InitialStackSize)
		return;

	memcpy(&Register, (Stack + StackSize), Size);
	StackSize += Size;
}

StackManager& StackManager::Get()
{
	static StackManager Manager;

	return Manager;
}