#define MAKS_CMD( Id, Name, Args, Code, AsmCode ) CMD_##Name = Id,

enum MAKS_ENUM
{
    #include "default.h"
    MAKS_count
};

#undef MAKS_CMD
