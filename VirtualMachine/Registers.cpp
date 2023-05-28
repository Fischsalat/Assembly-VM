#include "Registers.h"


bool operator&(int L, EFlags R)
{
    return L & static_cast<std::underlying_type<EFlags>::type>(R);
}

EFlags operator|(EFlags L, EFlags R)
{
    using UnderlayingType = std::underlying_type<EFlags>::type;

    return static_cast<EFlags>(static_cast<UnderlayingType>(L) | static_cast<UnderlayingType>(R));
}


void FlagsRegister::ClearAll()
{
    Zero = 0;
    Greater = 0;
    Lower = 0;
    Overflow = 0;
}

// Clears any existing flags
void FlagsRegister::SetFlags(EFlags F)
{
    using UnderlayingType = std::underlying_type<EFlags>::type;

    *reinterpret_cast<uint64_t*>(this) = static_cast<UnderlayingType>(F);
}

void FlagsRegister::AddFlags(EFlags F)
{
    using UnderlayingType = std::underlying_type<EFlags>::type;

    *reinterpret_cast<uint64_t*>(this) |= static_cast<UnderlayingType>(F);
}

bool FlagsRegister::IsZeroSet() const
{
    return Zero;
}

bool FlagsRegister::IsGreaterSet() const
{
    return Greater;
}

bool FlagsRegister::IsLowerSet() const
{
    return Lower;
}

bool FlagsRegister::IsOverflowSet() const
{
    return Overflow;
}