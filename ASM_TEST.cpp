#include <chrono>
#include <ctime>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <string>

#include "libs/MaksStack.h"
#include "libs/FileWork.h"
#include "config/default_enum.h"
struct signatyre
    {
    const char FirstChar;  /// what
    const char SecondChar; /// endian
    const char ProgramN1;  /// Name
    const char ProgramN2;  /// DadName
    const char ProgramN3;  /// SurName
    int SizeOfBytesInFile;
    };

struct UserFunc
    {
    char* name;
    int label;
    };

bool FileOpen(FILE** f, bool mode, int argc, char* argv[]);

int AnsSizeSuggestion(char* buff);

bool SignatyreCreate(signatyre* a);

bool BufferTreatment(char* buffer, char* ans, int* CharNum);

bool BufferReplaceEnterOnZero(char* buffer);

int main(int argc, char* argv[])
    {
    FILE* TmpFile;
    FileOpen(&TmpFile, 0, argc, argv);
    char* buffer = FileRead(TmpFile);

    signatyre KorobCom;
    SignatyreCreate(&KorobCom);

    char* ans = (char*) calloc(AnsSizeSuggestion(buffer), sizeof(char));

    BufferTreatment(buffer, ans, &(KorobCom.SizeOfBytesInFile));

    FileOpen(&TmpFile, 1, argc, argv);
    FileWrite(ans, &KorobCom, TmpFile);
    free(ans);
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
            *f = fopen("test.code", "rb");
            }
        } else
        {
        if(argc > 2)
            {
            *f = fopen(argv[2], "wb");
            } else
            {
            *f = fopen("test.asm", "wb");
            }
        }


    return 1;
    }

int AnsSizeSuggestion(char* buff)
    {
    assert(buff != NULL);

    int SSize = 0; // suggestion size
    while(*buff != '\0')
        {
        if(*buff == '\n')
            {
            ++SSize;
            }
        ++buff;
        }
    ++SSize;
    SSize = (SSize / 2) * 5 + SSize % 2 + 1;
    return SSize;
    }

bool SignatyreCreate(signatyre* a)
    {
    a->FirstChar  = 'I';
    a->SecondChar = 'I';
    a->ProgramN1  = 'M';
    a->ProgramN2  = 'D';
    a->ProgramN3  = 'K';

    return 1;
    }

bool BufferTreatment(char* buffer, char* ans, int* CharNum)
    {
    std::vector <UserFunc> Labels;

    int NowLabel = 0;
    char* TmpAns    = ans;
    char* TmpBuffer = buffer;

    int NowOper = 0;
    while(*TmpBuffer != '\0')
        {
        NowOper = -1;
        #define MAKS_CMD( Id, Name, Args, Code, AsmCode )    \
            if(!(strcmp(buffer, #Name)))                 \
                NowOper = CMD_##Name;

        #include "config/default.h"

        #undef MAKS_CMD

        switch(NowOper)
            {
            #define MAKS_CMD( Id, Name, Args, Code, AsmCode )    \
            case(CMD_##Name):                                    \
                ++NowLabel                                   \
                while(*TmpBuffer != '\0')                    \
                    {                                        \
                    ++TmpBuffer;                             \
                    }                                        \
                NowLabel += Args * 4;                        \
                break;

            #include "config/default.h"

            default:
                Labels[LabelNum].name = TmpBuffer;
                Labels[LabelNum].label = NowLabel;
                while(*TmpBuffer != '\0')
                    {
                    ++TmpBuffer;
                    }
                break;
            }

        #undef MAKS_CMD
        while(*TmpBuffer != '\0')
            {
            ++TmpBuffer;
            }

        ++TmpBuffer;
        }FileWrite(ans, KorobCom, TmpFile);

    TmpAns    = ans;
    TmpBuffer = buffer;

    while(*TmpBuffer != '\0')
        {
        NowOper = -1;
        #define MAKS_CMD( Id, Name, Args, Code, AsmCode )    \
            if(!(strcmp(buffer, #Name)))                 \
                NowOper = CMD_##Name;

        #include "config/default.h"

        #undef MAKS_CMD

        switch(NowOper)
            {
            #define MAKS_CMD( Id, Name, Args, Code, AsmCode )    \
            case(CMD_##Name):                                    \
                                                                 \
                *TmpAns = NowOper;                               \
                ++TmpAns;                                        \
                AsmCode                                          \
                break;

            #include "config/default.h"

            default:
                break;
            }

        #undef MAKS_CMD
        while(*TmpBuffer != '\0')
            {
            ++TmpBuffer;
            }

        ++TmpBuffer;
        }
    return 1;
    }

bool BufferReplaceEnterOnZero(char* buffer)
    {
    while(*buffer != '\0');
        {
        if(*buffer == ' ' || *buffer == '\n')
            {
            *buffer = '\0';
            }
        ++buffer;
        }
    return 1;
    }

bool FileWrite(char* ans, signatyre* KorobCom, FILE* Output)
    {
    fwrite(KorobCom, sizeof(char), sizeof(signatyre), Output);
    fwrite(ans, sizeof(char), KorobCom->SizeOfBytesInFile, CodeFile);

    return 1;
    }
