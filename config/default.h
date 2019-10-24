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
#include "..\libs\MaksAssert.h"
#include "CPUStackUseHelper.h"

CONF_CMD (END, 0, 0, 1,
    {
    free(buffer);
    DESTRUCK;
    EndFlag = 0;
    break;
    })

CONF_CMD (PUSH, 1, 1, 1,
    {
    PUSH(*((StackElem_t*)(NowLabel)));
    NowLabel += sizeof(StackElem_t);
    break;
    })

CONF_CMD (POP, 2, 0, 1,
    {
    POP;
    break;
    })

CONF_CMD (ADD, 3, 0, 1,
    {
    StackElem_t tmp1 = POP;
    StackElem_t tmp2 = POP;
    PUSH(tmp1 + tmp2);
    break;
    })

CONF_CMD (SUB, 4, 0, 1,
    {
    StackElem_t tmp1 = POP;
    StackElem_t tmp2 = POP;
    PUSH(tmp2 - tmp1);
    break;
    })

CONF_CMD (MUL, 5, 0, 1,
    {
    StackElem_t tmp1 = POP;
    StackElem_t tmp2 = POP;
    PUSH(tmp1 * tmp2);
    break;
    })

CONF_CMD (DIV, 6, 0, 1,
    {
    StackElem_t tmp1 = POP;
    StackElem_t tmp2 = POP;
    if(tmp1 == 0)
        {
        free(buffer);
        DESTRUCK;
	time_t now = std::chrono::system_clock::to_time_t ( std::chrono::system_clock::now() );
        fprintf(errors, "(!!!CRITICAL ERROR!!!)\nYou divided on zero\nO N L I N E HA HA HA: %d\non label number: %d\nIn program: %s\ntime: %s\n", AsmLine, (int)(NowLabel - buffer), OpenFile, ctime(&now));
        ASSERT(tmp1 != 0);
        }
    PUSH(tmp2 / tmp1);
    break;
    })

CONF_CMD (SQRT, 7, 0, 1,
    {
    StackElem_t tmp1 = POP;

    if(tmp1 < 0)
        {
        free(buffer);
        DESTRUCK;
        time_t now = std::chrono::system_clock::to_time_t ( std::chrono::system_clock::now() );
        fprintf(errors, "(!!!CRITICAL ERROR!!!)\nYou wanted to get a square from a negative number\nO N L I N E HA HA HA: %d\non label number: %d\nIn program: %s\ntime: %s\n", AsmLine, (int)(NowLabel - buffer), OpenFile, ctime(&now));
        ASSERT(tmp1 > 0);
        }
    PUSH(sqrt(tmp1));
    break;
    })

CONF_CMD (IN, 8, 0, 1,
    {
    StackElem_t tmp1 = {};

    std::cin >> tmp1;
    PUSH(tmp1);
    break;
    })

CONF_CMD (OUT, 9, 0, 1,
    {
    StackElem_t tmp1 = POP;
    std::cout << tmp1;
    break;
    })

CONF_CMD (OUTFILE, 19, 0, 1,
    {
    StackElem_t tmp1 = POP;
    fprintf(OutputFile, "%d ",tmp1);
    break;
    })


CONF_CMD (JMP, 10, 1, 2,
    {
    NowLabel = buffer + *((int*)(NowLaber));
    break;
    })

CONF_CMD (JA, 11, 1, 2,
    {
    StackElem_t tmp1 = POP;
    StackElem_t tmp2 = POP;

    if (tmp2 > tmp1)
        {
        NowLabel = buffer + *((int*)(NowLaber));
        }else
        {
        NowLabel += sizeof(int);
        }
    break;
    })

CONF_CMD (JAE, 12, 1, 2,
    {
    StackElem_t tmp1 = POP;
    StackElem_t tmp2 = POP;

    if (tmp2 >= tmp1)
        {
        NowLabel = buffer + *((int*)(NowLaber));
        }else
        {
        NowLabel += sizeof(int);
        }
    break;
    })

CONF_CMD (JB, 13, 1, 2,
    {
    StackElem_t tmp1 = POP;
    StackElem_t tmp2 = POP;

    if (tmp2 < tmp1)
        {
        NowLabel = buffer + *((int*)(NowLaber));
        }else
        {
        NowLabel += sizeof(int);
        }
    break;
    })

CONF_CMD (JBE, 14, 1, 2,
    {
    StackElem_t tmp1 = POP;
    StackElem_t tmp2 = POP;

    if (tmp2 <= tmp1)
        {
        NowLabel = buffer + *((int*)(NowLaber));
        }else
        {
        NowLabel += sizeof(int);
        }
    break;
    })

CONF_CMD (JE, 15, 1, 2,
    {
    StackElem_t tmp1 = POP;
    StackElem_t tmp2 = POP;

    if (tmp2 == tmp1)
        {
        NowLabel = buffer + *((int*)(NowLaber));
        }else
        {
        NowLabel += sizeof(int);
        }
    break;
    })

CONF_CMD (JNE, 16, 1, 2,
    {
    StackElem_t tmp1 = POP;
    StackElem_t tmp2 = POP;

    if (tmp2 != tmp1)
        {
        NowLabel = buffer + *((int*)(NowLaber));
        }else
        {
        NowLabel += sizeof(int);
        }
    break;
    })

CONF_CMD (CALL, 17, 1, 2,
    {
    label_stack.push(((int)(NowLaber - buffer) + sizeof(int)));
    NowLabel = buffer + *((int*)(NowLaber));
    break;
    })

CONF_CMD (RET, 18, 1, 2,
    {
    NowLabel = buffer + label_stack.top;
    label_stack.pop;
    break;
    })
