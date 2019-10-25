#include <stack>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <string>
#include <cassert>
#include "config\CPUStackUseHelper.h"
#include "config\default_enum.h"

bool FileOpen(FILE** f, bool mode, int argc, char* argv[]);

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

    DedStack_t cpu_stack;
    INIT;
    fscanf(TmpFile, "%d", &CharNum);
    char* buffer = (char*)calloc (CharNum, sizeof(char));
    fread(buffer, sizeof(char), CharNum, TmpFile);
    fclose(TmpFile);
    FILE* errors;
    FileOpen(&errors, 1, argc, argv);

    std::stack<int> label_stack;
    char* NowLabel = buffer;
    bool EndFlag = 1;
    int AsmLine = 1;

    while (EndFlag && NowLabel < (buffer + CharNum))
        {
        switch (*NowLabel)
            {
                #define MAKS_CMD( Name, Id, Args, Mode, Code )   \
                                                                 \
                case CMD_##Name:                                     \
                    {                                            \
                    Code;                                        \
			        break;                                       \
                    };


                #include "config/default.h" // tyt kizda

                #undef CONF_CMD

                default: {
                free(buffer);
                DESTRUCK;
                time_t now = std::chrono::system_clock::to_time_t ( std::chrono::system_clock::now() );
                fprintf(errors, "(!!!CRITICAL ERROR!!!)\nThere is undefined function\nO N L I N E HA HA HA: %d\non label number: %d\nIn program: %s\ntime: %s\n", AsmLine,(int)(NowLabel - buffer), OpenFile, ctime(&now));
                abort;
                };
            }

        ++AsmLine;
        ++NowLabel;
        }
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
