#pragma once
#include <iostream>

#include "Registers.h"
#include "Tests.h"


int main()
{
	Tests::TestGetDestinationData();
	Tests::TestBytecodeExecutor();

	Tests::TestOpcodeExecution();
}
