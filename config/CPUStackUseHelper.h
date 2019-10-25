/*
 * This is library that used to
 * help writing assembler, because
 * there is only 1 stack
*/

#include "../libs/MaksStack.h"

#define MAKS_DUMP_ASM( params ) DUMP(&cpu_stack, params)
#define MAKS_POP StackPop(&cpu_stack)
#define MAKS_INIT StackInit(&cpu_stack)
#define MAKS_PUSH( params ) StackPush(&cpu_stack, params)
#define MAKS_DESTRUCK StackDestruck(&cpu_stack)
#define MAKS_EMPTY IsEmptyStack(&cpu_stack)
