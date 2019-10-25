/*
* It is default config file for assembler and cpu
* 1 arg - name of func
* 2 arg - id of func
* 3 arg - num of args of func
* 4 arg - mode (1 - for default functions, 2 - for funcs with labels)
*/

#include <stack>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include "CPUStackUseHelper.h"

MAKS_CMD (0, END, 0,
    {
    free(buffer);
    MAKS_DESTRUCK;
    EndFlag = 0;
    break;
    },
    {
    ++(*CharNum);
    })

MAKS_CMD (1, PUSH, 1,
    {
    PUSH(*((StackElem_t*)(NowLabel)));
    NowLabel += sizeof(StackElem_t);
    break;
    },
    {
    ++(*CharNum);
    while(*TmpBuffer != '\0')
        {
        ++TmpBuffer;
        }
    for(int i = 0, i < Argc, ++i)
        {
        int  TmpInt = atoi(TmpBuffer);

        for(int i = 0; i < 4; ++i)
            {
            *TmpAns = (char)*((char*)(&TmpInt) + i);
            ++TmpAns;
            ++(*CharNum);
            }
        }
    })

MAKS_CMD (2, POP, 0,
    {
    MAKS_POP;
    break;
    },
    {
    ++(*CharNum);
    })

MAKS_CMD (3, ADD, 0,
    {
    StackElem_t tmp1 = MAKS_POP;
    StackElem_t tmp2 = MAKS_POP;
    MAKS_PUSH(tmp1 + tmp2);
    break;
    },
    {
    ++(*CharNum);
    })

MAKS_CMD (4, SUB, 0,
    {
    StackElem_t tmp1 = MAKS_POP;
    StackElem_t tmp2 = MAKS_POP;
    MAKS_PUSH(tmp2 - tmp1);
    break;
    },
    {
    ++(*CharNum);
    })

MAKS_CMD (5, MUL, 0,
    {
    StackElem_t tmp1 = MAKS_POP;
    StackElem_t tmp2 = MAKS_POP;
    MAKS_PUSH(tmp1 * tmp2);
    break;
    },
    {
    ++(*CharNum);
    })

MAKS_CMD (6, DIV, 0,
    {
    StackElem_t tmp1 = MAKS_POP;
    StackElem_t tmp2 = MAKS_POP;
    if(tmp1 == 0)
        {
        free(buffer);
        MAKS_DESTRUCK;
        time_t now = std::chrono::system_clock::to_time_t ( std::chrono::system_clock::now() );
        fprintf(errors, "(!!!CRITICAL ERROR!!!)\nYou divided on zero\nO N L I N E HA HA HA: %d\non label number: %d\nIn program: %s\ntime: %s\n", AsmLine, (int)(NowLabel - buffer), OpenFile, ctime(&now));
        assert(tmp1 != 0);
        }
    MAKS_PUSH(tmp2 / tmp1);
    break;
    },
    {
    ++(*CharNum);
    })

MAKS_CMD (7, SQRT, 0,
    {
    StackElem_t tmp1 = MAKS_POP;

    if(tmp1 < 0)
        {
        free(buffer);
        MAKS_DESTRUCK;
        time_t now = std::chrono::system_clock::to_time_t ( std::chrono::system_clock::now() );
        fprintf(errors, "(!!!CRITICAL ERROR!!!)\nYou wanted to get a square from a negative number\nO N L I N E HA HA HA: %d\non label number: %d\nIn program: %s\ntime: %s\n", AsmLine, (int)(NowLabel - buffer), OpenFile, ctime(&now));
        assert(tmp1 > 0);
        }
    MAKS_PUSH(sqrt(tmp1));
    break;
    },
    {
    ++(*CharNum);
    })

MAKS_CMD (8, IN, 0,
    {
    StackElem_t tmp1 = {};

    std::cin >> tmp1;
    MAKS_PUSH(tmp1);
    break;
    },
    {
    ++(*CharNum);
    })

MAKS_CMD (9, OUT, 0,
    {
    StackElem_t tmp1 = MAKS_POP;
    std::cout << tmp1;
    break;
    },
    {
    ++(*CharNum);
    })

MAKS_CMD (10, OUTFILE, 0,
    {
    StackElem_t tmp1 = MAKS_POP;
    fprintf(OutputFile, "%d ",tmp1);
    break;
    },
    {
    ++(*CharNum);
    })


MAKS_CMD (11, JMP, 1,
    {
    NowLabel = buffer + *((int*)(NowLaber));
    break;
    },
    {
    ++(*CharNum);
    while(*TmpBuffer != '\0')
        {
        ++TmpBuffer;
        }
    for(int i = 0, i < Argc, ++i)
        {
        int TmpInt = -1;
        for(int i = 0; i < Labels.size(); ++i)
            {
            if(TmpBuffer == Labels[i].name)
                {
                TmpInt = Labels[i].label;
                }
            }
        if(TmpInt == -1)
            {
            printf("(!!!CRIT_ERROR)!!!\nYour jmp points to undefined label!");
            }
        assert(TmpInt != -1);
        for(int i = 0; i < 4; ++i)
            {
            *TmpAns = (char)*((char*)(&TmpInt) + i);
            ++TmpAns;
            ++(*CharNum);
            }
        }
    })

MAKS_CMD (12, JA, 1,
    {
    StackElem_t tmp1 = MAKS_POP;
    StackElem_t tmp2 = MAKS_POP;

    if (tmp2 > tmp1)
        {
        NowLabel = buffer + *((int*)(NowLaber));
        }else
        {
        NowLabel += sizeof(int);
        }
    break;
    },
    {
    ++(*CharNum);
    while(*TmpBuffer != '\0')
        {
        ++TmpBuffer;
        }
    for(int i = 0, i < Argc, ++i)
        {
        int TmpInt = -1;
        for(int i = 0; i < Labels.size(); ++i)
            {
            if(TmpBuffer == Labels[i].name)
                {
                TmpInt = Labels[i].label;
                }
            }
        if(TmpInt == -1)
            {
            printf("(!!!CRIT_ERROR)!!!\nYour jmp points to undefined label!");
            }
        assert(TmpInt != -1);
        for(int i = 0; i < 4; ++i)
            {
            *TmpAns = (char)*((char*)(&TmpInt) + i);
            ++TmpAns;
            ++(*CharNum);
            }
        }
    })

MAKS_CMD (13, JAE, 1,
    {
    StackElem_t tmp1 = MAKS_POP;
    StackElem_t tmp2 = MAKS_POP;

    if (tmp2 >= tmp1)
        {
        NowLabel = buffer + *((int*)(NowLaber));
        }else
        {
        NowLabel += sizeof(int);
        }
    break;
    },
    {
    ++(*CharNum);
    while(*TmpBuffer != '\0')
        {
        ++TmpBuffer;
        }
    for(int i = 0, i < Argc, ++i)
        {
        int TmpInt = -1;
        for(int i = 0; i < Labels.size(); ++i)
            {
            if(TmpBuffer == Labels[i].name)
                {
                TmpInt = Labels[i].label;
                }
            }
        if(TmpInt == -1)
            {
            printf("(!!!CRIT_ERROR)!!!\nYour jmp points to undefined label!");
            }
        assert(TmpInt != -1);
        for(int i = 0; i < 4; ++i)
            {
            *TmpAns = (char)*((char*)(&TmpInt) + i);
            ++TmpAns;
            ++(*CharNum);
            }
        }
    })

MAKS_CMD (14, JB, 1,
    {
    StackElem_t tmp1 = MAKS_POP;
    StackElem_t tmp2 = MAKS_POP;

    if (tmp2 < tmp1)
        {
        NowLabel = buffer + *((int*)(NowLaber));
        }else
        {
        NowLabel += sizeof(int);
        }
    break;
    },
    {
    ++(*CharNum);
    while(*TmpBuffer != '\0')
        {
        ++TmpBuffer;
        }
    for(int i = 0, i < Argc, ++i)
        {
        int TmpInt = -1;
        for(int i = 0; i < Labels.size(); ++i)
            {
            if(TmpBuffer == Labels[i].name)
                {
                TmpInt = Labels[i].label;
                }
            }
        if(TmpInt == -1)
            {
            printf("(!!!CRIT_ERROR)!!!\nYour jmp points to undefined label!");
            }
        assert(TmpInt != -1);
        for(int i = 0; i < 4; ++i)
            {
            *TmpAns = (char)*((char*)(&TmpInt) + i);
            ++TmpAns;
            ++(*CharNum);
            }
        }
    })

MAKS_CMD (15, JBE, 1,
    {
    StackElem_t tmp1 = MAKS_POP;
    StackElem_t tmp2 = MAKS_POP;

    if (tmp2 <= tmp1)
        {
        NowLabel = buffer + *((int*)(NowLaber));
        }else
        {
        NowLabel += sizeof(int);
        }
    break;
    },
    {
    ++(*CharNum);
    while(*TmpBuffer != '\0')
        {
        ++TmpBuffer;
        }
    for(int i = 0, i < Argc, ++i)
        {
        int TmpInt = -1;
        for(int i = 0; i < Labels.size(); ++i)
            {
            if(TmpBuffer == Labels[i].name)
                {
                TmpInt = Labels[i].label;
                }
            }
        if(TmpInt == -1)
            {
            printf("(!!!CRIT_ERROR)!!!\nYour jmp points to undefined label!");
            }
        assert(TmpInt != -1);
        for(int i = 0; i < 4; ++i)
            {
            *TmpAns = (char)*((char*)(&TmpInt) + i);
            ++TmpAns;
            ++(*CharNum);
            }
        }
    })

MAKS_CMD (16, JE, 1,
    {
    StackElem_t tmp1 = MAKS_POP;
    StackElem_t tmp2 = MAKS_POP;

    if (tmp2 == tmp1)
        {
        NowLabel = buffer + *((int*)(NowLaber));
        }else
        {
        NowLabel += sizeof(int);
        }
    break;
    },
    {
    ++(*CharNum);
    while(*TmpBuffer != '\0')
        {
        ++TmpBuffer;
        }
    for(int i = 0, i < Argc, ++i)
        {
        int TmpInt = -1;
        for(int i = 0; i < Labels.size(); ++i)
            {
            if(TmpBuffer == Labels[i].name)
                {
                TmpInt = Labels[i].label;
                }
            }
        if(TmpInt == -1)
            {
            printf("(!!!CRIT_ERROR)!!!\nYour jmp points to undefined label!");
            }
        assert(TmpInt != -1);
        for(int i = 0; i < 4; ++i)
            {
            *TmpAns = (char)*((char*)(&TmpInt) + i);
            ++TmpAns;
            ++(*CharNum);
            }
        }
    })

MAKS_CMD (17, JNE, 1,
    {
    StackElem_t tmp1 = MAKS_POP;
    StackElem_t tmp2 = MAKS_POP;

    if (tmp2 != tmp1)
        {
        NowLabel = buffer + *((int*)(NowLaber));
        }else
        {
        NowLabel += sizeof(int);
        }
    break;
    },
    {
    ++(*CharNum);
    while(*TmpBuffer != '\0')
        {
        ++TmpBuffer;
        }
    for(int i = 0, i < Argc, ++i)
        {
        int TmpInt = -1;
        for(int i = 0; i < Labels.size(); ++i)
            {
            if(TmpBuffer == Labels[i].name)
                {
                TmpInt = Labels[i].label;
                }
            }
        if(TmpInt == -1)
            {
            printf("(!!!CRIT_ERROR)!!!\nYour jmp points to undefined label!");
            }
        assert(TmpInt != -1);
        for(int i = 0; i < 4; ++i)
            {
            *TmpAns = (char)*((char*)(&TmpInt) + i);
            ++TmpAns;
            ++(*CharNum);
            }
        }
    })

MAKS_CMD (18, CALL, 1,
    {
    label_stack.push(((int)(NowLaber - buffer) + sizeof(int)));
    NowLabel = buffer + *((int*)(NowLaber));
    break;
    },
    {
    ++(*CharNum);
    while(*TmpBuffer != '\0')
        {
        ++TmpBuffer;
        }
    for(int i = 0, i < Argc, ++i)
        {
        int TmpInt = -1;
        for(int i = 0; i < Labels.size(); ++i)
            {
            if(TmpBuffer == Labels[i].name)
                {
                TmpInt = Labels[i].label;
                }
            }
        if(TmpInt == -1)
            {
            printf("(!!!CRIT_ERROR)!!!\nYour jmp points to undefined label!\nUL: %s", TmpBuffer);
            }
        assert(TmpInt != -1);
        for(int i = 0; i < 4; ++i)
            {
            *TmpAns = (char)*((char*)(&TmpInt) + i);
            ++TmpAns;
            ++(*CharNum);
            }
        }
    })

MAKS_CMD (19, RET, 1,
    {
    NowLabel = buffer + label_stack.top;
    label_stack.pop;
    break;
    },
    {
    ++(*CharNum);
    })
