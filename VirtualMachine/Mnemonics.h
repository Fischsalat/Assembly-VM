#pragma once

enum class Mnemonic : uint8_t
{
    //Bitwise operations
    AND = 0x00,
    OR = 0x01,
    XOR = 0x02,
    NOT = 0x03,
    NOR = 0x04,
    NAND = 0x05,


    //Algebraic operations
    INC = 0x06,
    DEC = 0x07,
    ADD = 0x08,
    SUB = 0x09,
    MUL = 0x0A,
    DIV = 0x0B,


    //Comparison operations
    TEST = 0x0C, //Bitwise AND but modifies flags
    CMP = 0x0D, //Substraction but modifies flags


    //Jump operations
    JMP = 0x0E, // Jump (no condition)

    JZ = 0x0F, // Jump zero (zero-flag set)
    JNZ = 0x10, // Jump not zero (zero-flag not set)

    JE = 0x11, // Jump equal (zero-flag set) [same as JZ]
    JNE = 0x12, // Jump not equal (zero-flag not set) [same as JNZ]

    JG = 0x13, // Jump greater (greater-flag set)
    JGE = 0x14, // Jump greater-equal (greater-/ or zero-flag set)

    JL = 0x15, // Jump lower (lower-flag set)
    JLE = 0x16, // Jump lower-equal (lower-/ or zero-flag set)

    JO = 0x17, // Jump overflow (overflow-flag set)
    JNO = 0x18, // Jump not overflow (overflow-floag not set)

    //Load operations
    LEA = 0x19, // Load effective address (no type limitations)

    //Move operations
    MOV = 0x1A, // Move (no condition)

    CMOVZ = 0x1B,  // Move zero (zero-flag set)
    CMOVNZ = 0x1C, // Move not zero (zero-flag not set)

    CMOVE = 0x1D,  // Move equal (zero-flag set) [same as JZ]
    CMOVNE = 0x1E, // Move not equal (zero-flag not set) [same as JNZ]

    CMOVG = 0x1F,  // Move greater (greater-flag set)
    CMOVGE = 0x20, // Move greater-equal (greater-/ or zero-flag set)

    CMOVL = 0x21,  // Move lower (lower-flag set)
    CMOVLE = 0x22, // Move lower-equal (lower-/ or zero-flag set)

    CMOVO = 0x23,  // Move overflow (overflow-flag set)
    CMOVNO = 0x24, // Move not overflow (overflow-flag not set)

    PUSH = 0x25, // Pushes a value onto the stack (register -> stack)
    POP = 0x26,  // Pops a value from the stack (stack -> register)

    CALL = 0x27, // Call a function 
    RET = 0x28,  // Return from a function 

    NOP = 0x29, // No Operation

    SYS = 0x30 // Systemcall
};

enum class EAddressingMode : uint8_t
{
    Immediate = 0b00, //  5
    Register = 0b001, //  Q2
    Memory = 0b010,   // [Q2]
    Relative = 0b11,  // [Q2 + 0x10]

    None = 0b100      // Invalid
};

struct Operand
{
private:
    uint8_t Size : 5; // Size of bytes belonging to this opcode
    uint8_t DestAddressingMode : 2;  // DestAddressingMode -> MOVED TO struct OperandSizeInfo
    uint8_t bHasOperandSizeInfo : 1;

    // 0b|1|00|00100

public:
    inline int32_t GetNumBytes()
    {
        return Size + 1;
    }
     
    inline EAddressingMode GetDestAddressingMode()
    {
        return static_cast<EAddressingMode>(DestAddressingMode);
    }

    inline bool HasSizeInfoByte()
    {
        return bHasOperandSizeInfo;
    }

private:
    friend class Tests;
};

struct OperandSizeInfo
{
private:
    uint8_t DestSize : 3;
    uint8_t SrcSize : 3;
    uint8_t SrcAddressingMode : 2;

public:
    inline int32_t GetDestinationSizeBytes()
    {
        return DestSize + 1;
    }
    
    inline int32_t GetDestinationBitSize()
    {
        return GetDestinationSizeBytes() << 3;
    }

    inline uint64_t GetDestinationHighestBit()
    {
        return 1ull << GetDestinationBitSize();
    }

    inline uint64_t GetMask()
    {
        return ~(0xFFFFFFFFFFFFFFFFull >> GetDestinationBitSize());
    }

    inline int32_t GetSourceSizeBytes()
    {
        return SrcSize + 1;
    }

    inline EAddressingMode GetSrcAddressingMode()
    {
        return static_cast<EAddressingMode>(SrcAddressingMode);
    }

private:
    friend class Tests;
};
