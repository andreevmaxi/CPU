#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cassert>

struct Opers
    {
    char* name;
    int asmnum;
    int argo; /// arguments for this operation
    };

bool FileOpen(FILE** f, bool mode, int argc, char* argv[]);

char* FileRead(FILE* read);

bool ConfigOpen(char** rules, int argc, char* argv[]);

char* BufferAsm(char* buffer, char* conf);

bool ConfInit(char* conf, std::vector<Opers>& Operations, int* OperNum);

int AnsSizeSuggestion(char* buff);

bool IntReverse(int* Number, int dozens);

int main(int argc, char *argv[])
    {
    FILE* CodeFile;

    FileOpen(&CodeFile, 0, argc, argv);
    char* buffer = FileRead(CodeFile);
    fclose(CodeFile);
    printf("%d \n", __LINE__);
    char* config = {};
    ConfigOpen(&config, argc, argv);
    printf("%d \n", __LINE__);
    char* AsmOutput = BufferAsm(buffer, config);
    printf("%d \n", __LINE__);
    free(buffer);
    free(config);

    FileOpen(&CodeFile, 1, argc, argv);
    fprintf(CodeFile, "%s", AsmOutput);

    free(AsmOutput);
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

char* FileRead(FILE* read)
    {
    assert(read != NULL);

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
        f = fopen("default.conf", "rb");
        }

    *rules = FileRead(f);

    return 1;
    }

char* BufferAsm(char* buffer, char* conf)
    {
    assert(conf != NULL);
    assert(buffer != NULL);
    char* ans;

    printf("%d \n", __LINE__);
    std::vector <Opers> Operations;

    int OperNum = 0;

    ConfInit(conf, Operations, &OperNum);
    printf("%d \n", __LINE__);
    Operations.push_back(Opers());
    Operations[OperNum].name = {};
    Operations[OperNum].asmnum = 0;
    Operations[OperNum].argo = 0;
    ans = (char*)calloc (AnsSizeSuggestion(buffer), sizeof(char));

    char* TmpB;
    char* TmpO;
    char* TmpAns;

    int OperAns = 0;

    TmpAns = ans;
    printf("%d \n", __LINE__);
    for(int i = 0; i < OperNum; ++i)
    {
    printf("%s\n", Operations[i].name);
    }

    while (*buffer != '\0')
        {
        for(int i = 0; i < OperNum; ++i)
            {
            OperAns = OperNum;
            TmpB = buffer;
            TmpO = Operations[i].name;
            while(*TmpB == *TmpO && *TmpB != ' ' && *TmpB != '\n' && *TmpO != '\0')
                {
                ++TmpB;
                ++TmpO;
                }
            if( (*TmpB == ' ' || *TmpB == '\n' || *TmpB == '\0') && *TmpO == '\0')
                {
                OperAns = i;
                i = OperNum;
                }
            }

        while(*buffer != ' ' && *buffer != '\n' && *buffer != '\0')
            {
            ++buffer;
            }
        if(*buffer != '\0')
            {
            ++buffer;
            }

        assert(OperAns != 7);
        *TmpAns = (char)(Operations[OperAns].asmnum);
        ++TmpAns;

        for(int i = 0; i < Operations[OperAns].argo; ++i)
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
                *TmpAns = (char)*((char*)(&TmpIntAns) + i) + 1;
                ++TmpAns;
                }
            if(*buffer != '\0')
                {
                ++buffer;
                }
            }
        }

        return ans;
    }

bool ConfInit(char* conf, std::vector<Opers>& Operations, int* OperNum)
    {
    while (*conf != '\0')
        {
        Operations.push_back(Opers());
        Operations[*OperNum].name = conf;
        while (*conf != ' ' && *conf != '\0')
            {
            ++conf;
            }

        *conf = '\0';
        ++conf;

        Operations[*OperNum].asmnum = 0;
        int dozens = 1;
        while (*conf != ' ' && *conf != '\0')
            {
            Operations[*OperNum].asmnum += (int)(*conf - '0') * dozens;
            dozens *= 10;
            ++conf;
            }
        ++conf;
        Operations[*OperNum].argo = 0;
        dozens = 1;
        while (*conf != '\n' && *conf !='\0')
            {
            Operations[*OperNum].argo += (int)(*conf - '0') * dozens;
            dozens *= 10;
            ++conf;
            }

        dozens /= 10;
        IntReverse(&Operations[*OperNum].argo, dozens);
        if(*conf == '\n')
            {
            ++conf;
            }

        ++(*OperNum);
        }
    return 1;
    }

int AnsSizeSuggestion(char* buff)
    {
    assert(buff != NULL);

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
    assert(Number != 0);

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
