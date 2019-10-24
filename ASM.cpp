#include <chrono>
#include <ctime>
#include <cstdio>
#include <vector>
#include <cstdlib>

#include "libs/MaksAssert.h"
#include "libs/MaksStack.h"

struct Opers
    {
    char* name;
    int asmnum;
    int argo;
    int mode;
    };

struct UserFunc
    {
    char* name;
    int label;
    };

bool FileOpen(FILE** f, bool mode, int argc, char* argv[]);

char* FileRead(FILE* read);

bool ConfigOpen(char** rules, int argc, char* argv[]);

char* BufferAsm(char* buffer, char* conf, long long* CharNum);

bool ConfInit(char* conf, std::vector<Opers>& Operations, int* OperNum);

int AnsSizeSuggestion(char* buff);

bool IntReverse(int* Number, int dozens);

int main(int argc, char *argv[])
    {
    FILE* CodeFile;
    long long CharNum = 0;
    FileOpen(&CodeFile, 0, argc, argv);
    char* buffer = FileRead(CodeFile);
    fclose(CodeFile);

    char* config = {};
    ConfigOpen(&config, argc, argv);

    char* AsmOutput = BufferAsm(buffer, config, &CharNum);

    free(buffer);
    free(config);
    FileOpen(&CodeFile, 1, argc, argv);

    fwrite(AsmOutput, sizeof(char), CharNum + 4, CodeFile);

    free(AsmOutput);
    return 0;
    }

bool FileOpen(FILE** f, bool mode, int argc, char* argv[])
    {
    ASSERT(f != NULL);

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

char* FileRead(FILE* read)
    {
    ASSERT(read != NULL);

    char tmp, prev;
    int StrSize = 0;
    tmp = {};
    prev = {};

    tmp = (char)fgetc(read);

    while((tmp != EOF))
        {
        if(tmp != '\r')
            {
            if (!(tmp == prev && tmp == '\n'))
                {
                ++StrSize;
                prev = tmp;
                }
            }
        tmp = (char)fgetc(read);
        }

    rewind(read);

    char* buff = (char*)calloc ( (StrSize + 2), sizeof(char));

    char* LastChar = buff;
    tmp  = '\0';
    prev = '\n';

    while ( (tmp = getc (read)) != EOF )
    {
        if(tmp != '\r')
        {
            if (!(tmp == prev && tmp == '\n'))
            {
                *LastChar = tmp;
                prev = tmp;
                ++LastChar;
            }

        }
    }

    if(*(LastChar - 1) != '\0')
    {
        *(LastChar) = '\0';
    }

    return buff;

    }

bool ConfigOpen(char** rules, int argc, char* argv[])
    {
    FILE* f;
    if(argc > 3)
        {
        f = fopen(argv[3], "rb");
        } else
        {
        f = fopen("config/default.h", "rb");
        }

    *rules = FileRead(f);

    return 1;
    }

char* BufferAsm(char* buffer, char* conf, long long* CharNum)
    {
    ASSERT(conf != NULL);
    ASSERT(buffer != NULL);
    char* ans;


    std::vector <Opers> Operations;
    std::vector <UserFunc> Labels;
    int LabelNum = 0;
    int Current = 0;
    char* ForLabels = buffer;
    char* FirstLabel = (char*)3; // bog lubit 3cy

    while (*ForLabels != '\0')
        {
        if(*ForLabels == '\n' && *(ForLabels + 1) != '\n' && *(ForLabels + 1) != '\r')
            {
            ++Current;
            }
        if(*ForLabels == ':')
            {
            --Current;
            if(FirstLabel == (char*)3)
                {
                FirstLabel = ForLabels;
                }
            int i = 1;
            while (*(ForLabels - i) != '\n')
                {
                ++i;
                }
            Labels.push_back({});
            Labels[LabelNum].name = ForLabels - i + 1;
            Labels[LabelNum].label = Current + 1;
            ++LabelNum;
            }
        ++ForLabels;
        }

    int OperNum = 0;

    ConfInit(conf, Operations, &OperNum);

    Operations.push_back(Opers());
    Operations[OperNum].name = {};
    Operations[OperNum].asmnum = 0;
    Operations[OperNum].argo = 0;
    Operations[OperNum].mode = 0;
    ans = (char*)calloc (AnsSizeSuggestion(buffer) + 4, sizeof(char));

    char* TmpB;
    char* TmpO;
    char* TmpAns;

    int OperAns = 0;

    TmpAns = ans;
    TmpAns += sizeof(int);
    while (*buffer != '\0' )
        {

        while(*buffer == ' ')
            {
            ++buffer;
            }
        ++(*CharNum);
        for(int i = 0; i < OperNum; ++i)
            {
            OperAns = OperNum;
            TmpB = buffer;
            TmpO = Operations[i].name;
            while(*TmpB == *TmpO && *TmpB != ' ' && *TmpB != '\n' && *TmpO != '\0')
                {
                if(*TmpB == ':')
                    {
                    buffer = TmpB;
                    i = OperNum;
                    break;
                    }
                ++TmpB;
                ++TmpO;
                }
            if( (*TmpB == ' ' || *TmpB == '\n' || *TmpB == '\0') && *TmpO == '\0')
                {
                OperAns = i;
                i = OperNum;
                }
            }

        if(OperAns == OperNum)
            {
            while (*buffer != ' ' && *buffer != '\0' && *buffer != '\n' && *buffer != ':')
                {
                ++buffer;
                }
            }
        if(*buffer == ':')
            {
            ++buffer;
            } else
            {
            ASSERT(OperAns != OperNum);
            }

        while(*buffer != ' ' && *buffer != '\n' && *buffer != '\0')
            {
            ++buffer;
            }
        if(*buffer != '\0')
            {
            ++buffer;
            }

        if(OperAns != OperNum)
            {
            *TmpAns = (char)(Operations[OperAns].asmnum);
            ++TmpAns;

            for(int i = 0; i < Operations[OperAns].argo; ++i)
                {
                if(Operations[OperAns].mode == 1)
                    {
                    int TmpIntAns = 0;
                    int dozens = 1;
                    while (*buffer != '\n' && *buffer !='\0')
                        {
                        TmpIntAns += (int)(*buffer - '0') * dozens;
                        dozens *= 10;
                        ++buffer;
                        }
                    dozens /= 10;
                    IntReverse(&TmpIntAns, dozens);
                    for(int i = 0; i < 4; ++i)
                        {
                        *TmpAns = (char)*((char*)(&TmpIntAns) + i);
                        ++TmpAns;
                        ++(*CharNum);
                        }
                    if(*buffer != '\0')
                        {
                        ++buffer;
                        }
                    } else
                    {

                    int LabelAns = 0;
                    for(int i = 0; i < LabelNum; ++i)
                        {
                        LabelAns = LabelNum;
                        TmpB = buffer;
                        TmpO = Labels[i].name;
                        while(*TmpB == *TmpO && *TmpB != ' ' && *TmpB != '\n' && *TmpO != ':')
                            {
                            ++TmpB;
                            ++TmpO;
                            }

                        if( (*TmpB == ' ' || *TmpB == '\n' || *TmpB == '\0') && *TmpO == ':')
                            {
                            LabelAns = i;
                            i = LabelNum;
                            }
                        }

                    ASSERT(LabelAns != LabelNum);

                    for(int i = 0; i < 4; ++i)
                        {
                        *TmpAns = (char)*((char*)(&Labels[LabelAns].label) + i);
                        ++TmpAns;
                        ++(*CharNum);
                        }
                    while(*buffer != ' ' && *buffer != '\n' && *buffer != '\0')
                        {
                        ++buffer;
                        }
                    if(*buffer != '\0')
                        {
                        ++buffer;
                        }
                    }
                }
            }
        }
        *((int*)ans) = *CharNum;
        return ans;
    }

bool ConfInit(char* conf, std::vector<Opers>& Operations, int* OperNum)
    {
    while (*conf != '\0')
        {
        int i = 0;
        while (i != 8 && *conf != '\0')
            {
            switch (i)
                {
                case 0:
                    if(*conf == 'C')
                        {
                        ++i;
                        }
                    break;

                case 1:
                    if(*conf == 'O')
                        {
                        ++i;
                        } else
                        {
                        i = 0;
                        }
                    break;

                case 2:
                    if(*conf == 'N')
                        {
                        ++i;
                        } else
                        {
                        i = 0;
                        }
                    break;

                case 3:
                    if(*conf == 'F')
                        {
                        ++i;
                        } else
                        {
                        i = 0;
                        }
                    break;

                case 4:
                    if(*conf == '_')
                        {
                        ++i;
                        } else
                        {
                        i = 0;
                        }
                    break;

                case 5:
                    if(*conf == 'C')
                        {
                        ++i;
                        } else
                        {
                        i = 0;
                        }
                    break;

                case 6:
                    if(*conf == 'M')
                        {
                        ++i;
                        } else
                        {
                        i = 0;
                        }
                    break;

                case 7:
                    if(*conf == 'D')
                        {
                        ++i;
                        } else
                        {
                        i = 0;
                        }
                    break;
                }
            ++conf;
            }

        if(*conf == '\0')
            {
            break;
            }
        ++conf;
        ++conf;

        Operations.push_back(Opers());
        Operations[*OperNum].name = conf;
        while(*conf != ',')
            {
            ++conf;
            }
        *conf = '\0';
        ++conf;

        while(*conf == ' ')
            {
            ++conf;
            }

        Operations[*OperNum].asmnum = 0;
        int dozens = 1;
        while (*conf != ' ' && *conf != '\0' && *conf != ',')
            {
            Operations[*OperNum].asmnum += (int)(*conf - '0') * dozens;
            dozens *= 10;
            ++conf;
            }
        ++conf;
        Operations[*OperNum].argo = 0;
        dozens = 1;
        while(*conf == ' ')
            {
            ++conf;
            }
        while (*conf != '\n' && *conf !='\0' && *conf != ',')
            {
            Operations[*OperNum].argo += (int)(*conf - '0') * dozens;
            dozens *= 10;
            ++conf;
            }
        ++conf;
        while(*conf == ' ')
            {
            ++conf;
            }
        dozens /= 10;
        IntReverse(&Operations[*OperNum].argo, dozens);
        Operations[*OperNum].mode = (int)(*conf - '0');
        ++conf;
        ++(*OperNum);
        }
    return 1;
    }

int AnsSizeSuggestion(char* buff)
    {
    ASSERT(buff != NULL);

    int SSize = 0; // suggestion size
    while(*buff != '\0')
        {
        if(*buff == ' ')
            {
            ++SSize;
            }
        ++buff;
        }
    SSize = (SSize / 2) * 5 + SSize % 2;
    return SSize;
    }

bool IntReverse(int* Number, int dozens)
    {
    ASSERT(Number != 0);

    int tmp = 0;

    int TmpDoz = 1;
    while(*Number != 0 && dozens > 0)
        {
        tmp += (*Number / dozens) * TmpDoz;
        *Number = *Number % dozens;
        TmpDoz *= 10;
        dozens /= 10;
        }
    *Number = tmp;

    return 1;
    }
