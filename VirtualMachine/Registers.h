#pragma once
#include <inttypes.h>
#include <type_traits>

#include "ByteStream.h"

struct RegisterSet
{
    /* Integer Registers*/
    //Volatile
    union { uint64_t Q0 = 0x0; uint32_t D0; uint16_t W0; uint8_t B0; };
    union { uint64_t Q1 = 0x0; uint32_t D1; uint16_t W1; uint8_t B1; };
    union { uint64_t Q2 = 0x0; uint32_t D2; uint16_t W2; uint8_t B2; };
    union { uint64_t Q3 = 0x0; uint32_t D3; uint16_t W3; uint8_t B3; };
    union { uint64_t Q4 = 0x0; uint32_t D4; uint16_t W4; uint8_t B4; };
    union { uint64_t Q5 = 0x0; uint32_t D5; uint16_t W5; uint8_t B5; };
    union { uint64_t Q6 = 0x0; uint32_t D6; uint16_t W6; uint8_t B6; };
    union { uint64_t Q7 = 0x0; uint32_t D7; uint16_t W7; uint8_t B7; };

    //Preserved
    union { uint64_t Q8 = 0x0; uint32_t D8; uint16_t W8; uint8_t B8; };
    union { uint64_t Q9 = 0x0; uint32_t D9; uint16_t W9; uint8_t B9; };
    union { uint64_t Q10 = 0x0; uint32_t D10; uint16_t W10; uint8_t B10; };
    union { uint64_t Q11 = 0x0; uint32_t D11; uint16_t W11; uint8_t B11; };


    /* Floating Point Registers*/
    //Volatile
    double F0 = 0.0;
    double F1 = 0.0;
    double F2 = 0.0;
    double F3 = 0.0;
    double F4 = 0.0;
    double F5 = 0.0;
    double F6 = 0.0;
    double F7 = 0.0;

    //Preserved
    double F8 = 0.0;
    double F9 = 0.0;
    double F10 = 0.0;
    double F11 = 0.0;

private:
    friend class Tests;
    inline void ZeroAll() { *this = RegisterSet(); }
};

enum class ERegisters
{
    Q0, Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9, Q10, Q11,

    F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11,
};

enum class ESubRegister
{
    Byte        = 0b00000000,
    Word        = 0b00100000,
    DoubleWord  = 0b01000000,
    QuadWord    = 0b01100000,

    FloatingPointRegister = 0b10000000
};

struct RegisterInfo
{
private:
    uint8_t RegisterIdx : 5;
    uint8_t SubRegShiftCount : 2;
    uint8_t bIsFloatRegister : 1;

public:
    inline uint8_t GetRegIdx()
    {
        return RegisterIdx;
    }

    inline uint8_t GetSubregShiftCount()
    {
        return SubRegShiftCount;
    }

    inline uint8_t GetSubregByteCount()
    {
        return 1 << SubRegShiftCount;
    }

    inline bool IsFloatRegister()
    {
        return bIsFloatRegister;
    }

private:
    friend class Tests;
};

enum class EFlags : uint64_t
{
    //Zero - Flag[ZF] - [0b0001] - comparison resulted is equal
    Zero = 0b0001,

    //Greater - Flag[GF] - [0b0010] - comparison resulted is greater
    Greater = 0b0010,

    //Lower - Flag[LF] - [0b0100] - comparison resulted is lower
    Lower = 0b0100,

    //Overflow-Flag [OF] - [0b1000] - operation resulted in unsigned overflow (eg. byte(255) + 1)
    Overflow = 0b1000
};

bool operator&(int L, EFlags R);
EFlags operator|(EFlags L, EFlags R);

struct FlagsRegister
{
private:
    using UnderlayingFlagType = std::underlying_type<EFlags>::type;

private:
    //Zero - Flag[ZF] - [0b0001] - comparison resulted is equal
    uint64_t Zero : 1;

    //Greater - Flag[GF] - [0b0010] - comparison resulted is greater
    uint64_t Greater : 1;

    //Lower - Flag[LF] - [0b0100] - comparison resulted is lower
    uint64_t Lower : 1;

    //Overflow-Flag [OF] - [0b1000] - operation resulted in unsigned overflow (eg. byte(255) + 1)
    uint64_t Overflow : 1;

public:
    void ClearAll();

    // Clears any existing flags
    void SetFlags(EFlags F);

    void AddFlags(EFlags F);

    bool IsFlagSet(EFlags F);

    bool IsZeroSet() const;
    bool IsGreaterSet() const;
    bool IsLowerSet() const;
    bool IsOverflowSet() const;
};

inline FlagsRegister FL;

inline ByteStream PC;

inline RegisterSet Registers;
