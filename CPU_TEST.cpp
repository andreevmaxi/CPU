#include <stack>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <string>
#include <cassert>
#include <math.h>
#include "config\CPUStackUseHelper.h"
#include "libs\MaksStack.h"
#include "config\default_enum.h"

const int RAM_SIZE_CONST  = 150;

bool FileOpen(FILE** f, bool mode, int argc, char* argv[]);

int CPU_func(FILE* errors, char* OpenFile, char* buffer, int* CharNum, FILE* OutputFile);

int main(int argc, char *argv[])
    {
    FILE* TmpFile;
    FILE* OutputFile;
    int CharNum = 0;
    char* OpenFile;

    if(argc > 1)
        {
        OpenFile = argv[1];
        } else
        {
        char NameFile[100] = "test.asm";
        OpenFile = &NameFile[0];
        }
    if(argc > 2)
        {
        OutputFile = fopen(argv[2], "w");
        } else
        {
        OutputFile = fopen("DefaultOutput.txt", "w");
        }
    FileOpen(&TmpFile, 0, argc, argv);

    char syng[5];
    fread(syng, sizeof(char), 4, TmpFile);
    fread(&CharNum, sizeof(int), 1, TmpFile);

    char* buffer = (char*)calloc (CharNum, sizeof(char));
    fread(buffer, sizeof(char), CharNum, TmpFile);
    fclose(TmpFile);
    FILE* errors;
    FileOpen(&errors, 1, argc, argv);
    CPU_func(errors, OpenFile, buffer, &CharNum, OutputFile);
    free(buffer);
    return 0;
    }

bool FileOpen(FILE** f, bool mode, int argc, char* argv[])
    {
    assert(f != NULL);

    int i = 0;
    if(mode == 0)
        {
        if(argc > 1)
            {
            *f = fopen(argv[1], "rb");
            } else
            {
            *f = fopen("test.asm", "rb");
            }
        } else
        {
        if(argc > 2)
            {
            *f = fopen(argv[2], "a");
            } else
            {
            *f = fopen("errors.txt", "a");
            }
        }


    return 1;
    }

int CPU_func(FILE* errors, char* OpenFile, char* buffer, int* CharNum, FILE* OutputFile)
    {
    assert(errors != NULL);
    assert(OpenFile != NULL);
    assert(buffer != NULL);
    assert(CharNum != NULL);
    assert(OutputFile != NULL);

    StackElem_t Registers[4] = {};
    StackElem_t RAM[RAM_SIZE_CONST] = {};
    std::stack<int> label_stack;
    char* NowLabel = buffer;
    bool EndFlag = 1;
    int AsmLine = 1;
    DedStack_t cpu_stack;
    MAKS_INIT;

    while (EndFlag && NowLabel < (buffer + *CharNum))
        {
        switch ((int)(*NowLabel) )
            {
                #define MAKS_CMD( Id, Name, Args, Code, AsmCode )   \
                                                                    \
                case CMD_##Name:                                    \
                    {                                               \
                    ++NowLabel;                                     \
                    Code;                                           \
			        break;                                          \
                    };

                #include "config/default.h"

                #undef CONF_CMD

                default: {
                free(buffer);
                MAKS_DESTRUCK;
                time_t now = std::chrono::system_clock::to_time_t ( std::chrono::system_clock::now() );
                fprintf(errors, "(!!!CRITICAL ERROR!!!)\nThere is undefined function\nO N L I N E HA HA HA: %d\non label number: %d\nIn program: %s\ntime: %s\n", AsmLine,(int)(NowLabel - buffer), OpenFile, ctime(&now));
                abort;
                }
            }
        ++AsmLine;
        }
    return 0;
    }
