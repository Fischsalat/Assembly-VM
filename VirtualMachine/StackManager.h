#pragma once
#include <inttypes.h>
#include <cstdlib>
#include <cstring>


class StackManager
{
private:
	static constexpr int32_t InitialStackSize = 0x1000;

private:
	uint8_t* Stack;
	int32_t StackSize;

public:
	StackManager(const StackManager&) = delete;
	StackManager operator=(const StackManager&) = delete;

private:
	StackManager();
	~StackManager();

public:
	void PushToStack(uint64_t RegContent, uint8_t Size);

	void PopToReg(uint64_t& Register, uint8_t Size);
	
public:
	static StackManager& Get();
};