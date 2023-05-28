#include "ByteStream.h"

ByteStream::ByteStream(uint8_t* Bytes, int32_t End)
	: Data(Bytes), Pos(0), EndPos(End)
{
}

void ByteStream::Read(void* OutData, int Size)
{
	if (OutData && (Pos + Size) <= EndPos && Size > 0)
	{
		memcpy(OutData, (Data + Pos), Size);

		Pos += Size;
	}
}

void ByteStream::ReadSamePos(void* OutData, int Size) const
{
	if (OutData && (Pos + Size) <= EndPos && Size > 0)
		memcpy(OutData, (Data + Pos), Size);
}

void ByteStream::IncrementPos(bool bIncrement)
{
	Pos += bIncrement;
}