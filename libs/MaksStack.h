/*!
* @mainpage
* This is program that sorts texts
*/

/*!
* \file main.cpp
*/

#include <cstdio>
#include <cstdlib>
#include <string>
#include <cassert>

#ifdef _DEBUG
    #define DEB( code ) code
#else
    #define DEB( code )
#endif

typedef int StackElem_t;

const int ReallocConst      = 2;  /// Const of resizing in func StackResize
const int SizeDecreeseConst = 10; /// Const of decrisezing in func NormalizeSize


struct DedStack_t /// struct of stack
{
    DEB(int CanaryLeft); /// left canary of our stack
    int NowElem; /// index of next element in data
    int SizeStack;  /// size of used memory
    DEB(int* CanaryDataLeft); /// left canary of our data in stack
    StackElem_t* data; /// data that needs to be saved
    DEB(int* CanaryDataRight);/// right canary of our data in stack
    DEB(unsigned long long StkHash);
    DEB(int Err);  /// number of error !(USED ONLY WHILE DEBUGED)!
    /// 0 - all good
    /// 1 - if there isn't any memory
    /// 2 - if in stack maybe a poison element
    /// 3 - if size of stack is under the 0
    /// 4 - if Canary isn't good
    /// 5 - if data pointer in NULL
    /// 6 - if hash isn't good
    DEB(std::string* Name); /// name of our stack !(USED ONLY WHILE DEBUGED)!
    DEB(int CanaryRight); /// right canary of our stack
};

/**
    \brief StackHash
    \author andreevmaxi
	\version 1.0
	\date october  2019 year
	\copyright korobcom
    \details This is function that does hash of our stack
    \return hash of our stack
    \param[in] aStack pointer on dStack with that we work
*/

unsigned long long StackHash(DedStack_t* aStack)
    {

    assert(aStack != NULL);
    assert(&aStack->SizeStack != NULL);
    assert(&aStack->data != NULL);

    const unsigned long long MaxHashKey = 0x5BD1E995;

	const int ByteShift = 10;

    unsigned long long HashSum = 0;

    // initializing HashSum

    DEB(HashSum = MaxHashKey ^ aStack->SizeStack);

    StackElem_t tmp = 0;

    // adding NowElem to hash

    DEB(tmp = aStack->NowElem);
    DEB(tmp *= MaxHashKey);
    DEB(tmp ^= tmp >> ByteShift);
    DEB(tmp *= MaxHashKey);

    DEB(HashSum *= MaxHashKey);
    DEB(HashSum ^= tmp);

    // adding SizeStack to hash

    DEB(tmp = aStack->SizeStack);
    DEB(tmp *= MaxHashKey);
    DEB(tmp ^= tmp >> ByteShift);
    DEB(tmp *= MaxHashKey);

    DEB(HashSum *= MaxHashKey);
    DEB(HashSum ^= tmp);

    // adding CanaryDataLeft to hash

    DEB(tmp = *aStack->CanaryDataLeft);
    DEB(tmp *= MaxHashKey);
    DEB(tmp ^= tmp >> ByteShift);
    DEB(tmp *= MaxHashKey);

    DEB(HashSum *= MaxHashKey);
    DEB(HashSum ^= tmp);

    // adding all Data to hash

    for (int i = 0; i < aStack->SizeStack; ++i)
        {
        DEB(tmp = *(aStack->data + i));
        DEB(tmp *= MaxHashKey);
        DEB(tmp ^= tmp >> ByteShift);
        DEB(tmp *= MaxHashKey);

        DEB(HashSum *= MaxHashKey);
        DEB(HashSum ^= tmp);
        }

    // adding CanaryDataRight to hash

    DEB(tmp = *aStack->CanaryDataRight);
    DEB(tmp *= MaxHashKey);
    DEB(tmp ^= tmp >> ByteShift);
    DEB(tmp *= MaxHashKey);

    DEB(HashSum *= MaxHashKey);
    DEB(HashSum ^= tmp);

    // adding StackName to hash

    int i = 0;

    DEB(while ( *((char*)aStack->Name + i) != '\0'))
        {
        DEB(tmp = (unsigned long long)(*((char*)aStack->Name + i)));
        DEB(tmp *= MaxHashKey);
        DEB(tmp ^= tmp >> ByteShift);
        DEB(tmp *= MaxHashKey);

        DEB(HashSum *= MaxHashKey);
        DEB(HashSum ^= tmp);

        DEB(++i);
        }

    return HashSum;
    }

/**
    \brief DUMP
    \author andreevmaxi
	\version 1.0
	\date october  2019 year
	\copyright korobcom
    \details This is function that prints status of our stack
    \return 1 if all good
    \param[in] aStack pointer on dStack with that we work
    \param[in] NowFile file that used this stack
    \param[in] Line line in file where that stack used
    \param[in] FuncName function that used stack
*/

void DUMP(DedStack_t* aStack, std::string NowFile, int Line, std::string FuncName)
    {
    FILE* f = fopen("errors.txt", "a");
    fprintf(f, "StuckDump with pointer: [%o]\n", aStack);
    fprintf(f, "Printed from: %s in line: %d in FUCtion: %s\n", NowFile.c_str(), Line, FuncName.c_str());

    fclose(f);
    f = fopen("errors.txt", "a");

    std::string status = {};

    DEB(if(aStack->Err != 0))
        {
        DEB(if(aStack->Err == 2))
            {
            status = "UNK"; // unknown
            } DEB(else)
            {
            status = "BAD";
            }
        } DEB(else)
        {
        status = "OK";
        }

    DEB(fprintf(f, "Stack \"%s\" [%o](%s)\n", (*aStack->Name).c_str(), (int)aStack, status.c_str()));
    DEB(fprintf(f, "    {\n"));
    DEB(fprintf(f, "    size = %d\n", aStack->SizeStack));
    DEB(fprintf(f, "    data[%d] = [%o]\n", aStack->SizeStack, (int)&aStack->data));
    DEB(fprintf(f, "    memorized hash    = [%o] \n", aStack->StkHash));
    DEB(if(StackHash(aStack) != aStack->StkHash))
        {
        fprintf(f, "*!!!hash of stack now = [%o]  !!!*\n", StackHash(aStack));
        }

    fprintf(f, "        {\n");
    fclose(f);
    f = fopen("errors.txt", "a");

    for(int i = 0; i < aStack->SizeStack; ++i)
        {
        if(i < aStack->NowElem)
            {
            fprintf(f, "       *");
            }else
            {
            fprintf(f, "        ");
            }
        fprintf(f, "[%d] = %d", i, *(aStack->data + i));
        if( *(aStack->data + i) == -230)
            {
            fprintf(f, "[MB Poison]");
            }
        fprintf(f, "\n");
        }
    fprintf(f, "        }\n");
    DEB(fprintf(f, "err = %d (%s)\n", aStack->Err, status.c_str()));
    fprintf(f, "    }\n\n");

    fclose(f);
    return;
    }

/**
    \brief ASSERT_OK
    \author andreevmaxi
	\version 1.0
	\date october  2019 year
	\copyright korobcom
    \details This is function that verifies our stack
    \return 1 if all good
    \param[in] aStack pointer on dStack with that we work
    \param[in] NowFile file that used this stack
    \param[in] Line line in file where that stack used
    \param[in] FuncName function that used stack
*/

void ASSERT_OK(DedStack_t* aStack, std::string NowFile, int Line, std::string FuncName)
    {
    assert(aStack != NULL);
    DEB(assert(&aStack->StkHash != NULL));

    bool FlagOfSecondError = 0;

    DEB(if(aStack->StkHash != StackHash(aStack)))
        {
        DEB(aStack->Err = 6);
        DUMP(aStack, NowFile, Line, FuncName);
        }

    if(aStack->data == NULL)
        {
        DEB(aStack->Err = 5);
        DUMP(aStack, NowFile, Line, FuncName);
        }

    DEB(if(aStack->CanaryRight != 230 || aStack->CanaryLeft != 230))
        {
        DEB(aStack->Err = 4);
        DUMP(aStack, NowFile, Line, FuncName);
        }

    DEB(if(*aStack->CanaryDataRight != 230 || *aStack->CanaryDataLeft != 230))
        {
        DEB(aStack->Err = 4);
        DUMP(aStack, NowFile, Line, FuncName);
        }

    for(int i = 0; i < aStack->NowElem; ++i)
        {
        if( *(aStack->data + i) == -230)
            {
            FlagOfSecondError = 1;
            }
        }

    if(FlagOfSecondError)
        {
        DEB(aStack->Err = 2);
        } else
        {
        DEB(aStack->Err = 0);
        }

    DEB(if(aStack->Err != 0))
        {
        DUMP(aStack, NowFile, Line, FuncName);
        }
    return;
    }

/**
    \brief IsEmptyStack
    \author andreevmaxi
	\version 1.0
	\date october  2019 year
	\copyright korobcom
    \details This is function that checks if our stack is empty
    \return 1 if it's empty and 0 if it's not empty
    \param[in] aStack pointer on dStack with that we work
*/

bool IsEmptyStack(DedStack_t* aStack)
    {

    assert(aStack != NULL);

    DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));
    if(aStack->NowElem == 0)
        {
        return 1;
        }
    return 0;
    }

/**
    \brief StackResize
    \author andreevmaxi
	\version 1.0
	\date october  2019 year
	\copyright korobcom
    \details This is function that resizes our stack
    \return 1 if all good
    \param[in] aStack pointer on dStack with that we work
*/

bool StackResize(DedStack_t* aStack)
    {
        DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));

        aStack->SizeStack *= ReallocConst;

        #ifdef _DEBUG
            aStack->data = (StackElem_t*) realloc(--aStack->data, sizeof(StackElem_t) * (aStack->SizeStack + 2));
        #else
            aStack->data = (StackElem_t*) realloc(aStack->data, sizeof(StackElem_t) * aStack->SizeStack);
        #endif
        DEB(++aStack->data);
        DEB(aStack->CanaryDataRight = aStack->data + aStack->SizeStack);
        DEB(*aStack->CanaryDataRight = 230);

        if(aStack->data == NULL)
            {
            DEB(aStack->Err = 1);
            DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));
            }
        for(int i = aStack->NowElem; i < aStack->SizeStack; ++i)
            {
            *(aStack->data + i) = -230;
            }
        DEB(aStack->StkHash   = StackHash(aStack));
        DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));

        return 1;
    }

/**
    \brief NormalizeSize
    \author andreevmaxi
	\version 1.0
	\date october  2019 year
	\copyright korobcom
    \details This is function that decreeses size of our stack
    \return 1 if all good
    \param[in] aStack pointer on dStack with that we work
*/

bool NormalizeSize(DedStack_t* aStack)
    {
    DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));

    aStack->SizeStack -= SizeDecreeseConst;

    #ifdef _DEBUG
        aStack->data = (StackElem_t*) realloc(--aStack->data, sizeof(StackElem_t) * (aStack->SizeStack + 2));
    #else
        aStack->data = (StackElem_t*) realloc(aStack->data, sizeof(StackElem_t) * aStack->SizeStack);
    #endif
    DEB(++aStack->data);
    DEB(aStack->CanaryDataRight = aStack->data + aStack->SizeStack);
    DEB(*aStack->CanaryDataRight = 230);

    if(aStack->data == NULL)
        {
        DEB(aStack->Err = 1);
        DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));
        }

    DEB(aStack->StkHash   = StackHash(aStack));

    DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));
    }

/**
    \brief StackPop
    \author andreevmaxi
	\version 1.0
	\date october  2019 year
	\copyright korobcom
    \details This is function that pops an element from our stack
    \return element of poped element
    \param[in] aStack pointer on dStack with that we work
*/

StackElem_t StackPop(DedStack_t* aStack)
    {

    assert(aStack != NULL);

    DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));

    if( aStack->SizeStack > 25 && (double)aStack->SizeStack / (double)aStack->NowElem > 1.5)
    {
        NormalizeSize(aStack);
    }

    if(aStack->NowElem > 0)
        {
            StackElem_t tmp = *( aStack->data + ( --aStack->NowElem ) );
            *( aStack->data + aStack->NowElem ) = -230;
            DEB(aStack->StkHash   = StackHash(aStack));
            return (tmp);
        }

    DEB(aStack->Err = 3);
    return (-230);
    }

/**
    \brief StackInit
    \author andreevmaxi
	\version 1.0
	\date october  2019 year
	\copyright korobcom
    \details This is function that initializes our stack
    \return 1 if all good
    \param[in] aStack pointer on dStack with that we work
*/

bool StackInit(DedStack_t* aStack)
    {

    assert(aStack != NULL);

    DEB(aStack->CanaryLeft  = 230);
    DEB(aStack->CanaryRight = 230);
    aStack->SizeStack = 4;

    #ifdef _DEBUG
        aStack->data = (StackElem_t*) calloc(aStack->SizeStack + 2, sizeof(StackElem_t));
    #else
        aStack->data = (StackElem_t*) calloc(aStack->SizeStack, sizeof(StackElem_t));
    #endif
    DEB(aStack->CanaryDataRight = (aStack->data + aStack->SizeStack + 1) );
    DEB(aStack->CanaryDataLeft = aStack->data);
    DEB(++aStack->data);
    DEB(*aStack->CanaryDataRight = 230);
    DEB(*aStack->CanaryDataLeft = 230);

    aStack->NowElem   = 0;

    DEB(aStack->StkHash   = StackHash(aStack));

    DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));
    for(int i = 0; i < aStack->SizeStack; ++i)
        {
            *(aStack->data + i) = -230; // poison element
        }
    DEB(aStack->Err = 0);
    DEB(aStack->StkHash   = StackHash(aStack));
    DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));

    return 1;

    }

/**
    \brief StackPush
    \author andreevmaxi
	\version 1.0
	\date october  2019 year
	\copyright korobcom
    \details This is function that push elements in our stack
    \return 1 if all good
    \param[in] aStack pointer on dStack with that we work
*/

bool StackPush(DedStack_t* aStack, StackElem_t tmp)
    {
    assert(aStack != NULL);

    DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));

    if(aStack->NowElem + 1 > aStack->SizeStack)
        {
        StackResize(aStack);
        DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));
        }
    if(aStack->SizeStack < 0)
        {
        DEB(aStack->Err = 3);
        DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));
        }
    *(aStack->data + aStack->NowElem) = tmp;
    ++aStack->NowElem;
    DEB(aStack->StkHash   = StackHash(aStack));

    DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));

    return 1;
    }

/**
    \brief StuckDestruck
    \author andreevmaxi
	\version 1.0
	\date october  2019 year
	\copyright korobcom
    \details This is function that destruckes our stack
    \param[in] aStack pointer on dStack with that we work
*/

void StackDestruck(DedStack_t* aStack)
    {
    DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));

    while (!IsEmptyStack)
        {
        StackPop(aStack);
        DEB(ASSERT_OK(aStack, __FILE__, __LINE__, __PRETTY_FUNCTION__));
        }
    return;
    }

