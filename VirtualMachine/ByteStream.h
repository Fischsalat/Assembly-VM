#pragma once
#include <inttypes.h>
#include <cstring>

class ByteStream
{
private:
	uint8_t* Data;
	uint32_t Pos;
	uint32_t EndPos;

public:
	ByteStream() = default;

	ByteStream(uint8_t* Bytes, int32_t End);

public:
	void Read(void* OutData, int Size);

	void ReadSamePos(void* OutData, int Size) const;

	void IncrementPos(bool bIncrement);
};
