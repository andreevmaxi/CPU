#include <cassert>

/**
    \brief FileRead
    \author andreevmaxi
	\version 1.0
	\date september 2019 year
	\copyright korobcom
    \details This is function that reads symbols of windows file without '\r'
    \return pointer on reserved buffer of things in text file
    \param[in] read pointer on binar reading file
*/

char* FileRead(FILE* read)
    {
    assert(read != NULL);

    char tmp, prev;
    int StrSize = 0;
    tmp = 0;
    prev = 0;

    tmp = (char)fgetc(read);

    while((tmp != EOF))
        {
        if(tmp != '\r' && tmp != (char)(9) && tmp != ':')
            {
            if (!(tmp == prev && tmp == '\n') && !(tmp == ' ' && prev == '\n') && !(tmp == prev && tmp == ' ') && !(tmp == '\n' && prev == ' '))
                {
                ++StrSize;
                prev = tmp;
                }
            }
        tmp = (char)fgetc(read);
        }

    rewind(read);

    char* buff = (char*)calloc ( (StrSize + 3), sizeof(char));

    char* LastChar = buff;
    tmp  = '\0';
    prev = '\n';

    while ( (tmp = getc (read)) != EOF )
    {
        if(tmp != '\r' && tmp != (char)9 && tmp != ':')
        {
            if (!(tmp == prev && tmp == '\n') && !(tmp == ' ' && prev == '\n') && !(tmp == prev && tmp == ' ') && !(tmp == '\n' && prev == ' '))
            {
                *LastChar = tmp;
                prev = tmp;
                ++LastChar;
            }
        }
    }

    if(*(LastChar - 1) != '\0')
    {
        *(LastChar + 2) = '\0';
    }
    *(LastChar)     = '\0';
    *(LastChar + 1) = '\0';

    return buff;
    }
