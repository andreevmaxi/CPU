/*
 * This is library that used to
 * help writing assembler, because
 * there is only 1 stack
*/

#include "../libs/MaksStack.h"

#define DUMP_ASM( params ) DUMP(&cpu_stack, params)
#define POP StackPop(&cpu_stack)
#define INIT StackInit(&cpu_stack)
#define PUSH( params ) StackPush(&cpu_stack, params)
#define DESTRUCK StackDestruck(&cpu_stack)
#define EMPTY IsEmptyStack(&cpu_stack)