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
        if(tmp != '\r')
            {
            if (!(tmp == prev && tmp == '\n'))
                {
                ++StrSize;
                prev = tmp;
                }
            if (!(tmp == prev && tmp == ' '))
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
        *(LastChar + 1) = '\0';
    }
    *(LastChar)     = '\0';

    return buff;
    }
