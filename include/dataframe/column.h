#ifndef GUARD_CB3CD7B2_38EC_4C7A_AF1F_6AE50B3BF70C
#define GUARD_CB3CD7B2_38EC_4C7A_AF1F_6AE50B3BF70C

#include "dataframe/config.h"
#include "dataframe/bitvector.h"
#include "dataframe/types.h"

typedef struct DataFrame_Column DataFrame_Column;
typedef struct
{
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_Column* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_Column* self);
    bool (*DecRef)(DataFrame_Column* self);
    size_t (*GetSize)(DataFrame_Column* self);
    void (*Remove)(DataFrame_Column* self, size_t i);
    void (*Clear)(DataFrame_Column* self);
    char* (*GetName)(DataFrame_Column* self);
    const char* (*SetName)(DataFrame_Column* self, const char*);
} DataFrame_ColumnMethods;

struct DataFrame_Column
{
    DataFrame_ColumnMethods* methods;
};



#endif
