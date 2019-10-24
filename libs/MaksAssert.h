#define ASSERT( cond )                                                                                                     \
    {                                                                                                                      \
        if (!(cond))                                                                                                       \
        {                                                                                                                  \
            printf("Ass failed: %s, file: %s,\n"                                                                           \
                   "line: %d, function %s:\n", #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__);                            \
            abort();                                                                                                       \
        }                                                                                                                  \
    }