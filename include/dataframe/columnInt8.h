#include "dataframe/column.h"

typedef struct DataFrame_ColumnInt8 DataFrame_ColumnInt8;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnInt8* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnInt8* self);
    bool (*DecRef)(DataFrame_ColumnInt8* self);
    size_t (*GetSize)(DataFrame_ColumnInt8* self);
    void (*Remove)(DataFrame_ColumnInt8* self, size_t i);
    void (*Clear)(DataFrame_ColumnInt8* self);
    char* (*GetName)(DataFrame_ColumnInt8* self);
    const char* (*SetName)(DataFrame_ColumnInt8* self, const char* name);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnInt8* self, size_t index, int8_t* v);
    const char* (*Add)(DataFrame_ColumnInt8* self, int8_t v);
    const char* (*AddNA)(DataFrame_ColumnInt8* self);
    void (*Set)(DataFrame_ColumnInt8* self, size_t i, int8_t v);
    void (*SetNA)(DataFrame_ColumnInt8* self, size_t i);
} DataFrame_ColumnInt8Methods;

DATAFRAME_EXPORT DataFrame_ColumnInt8* DataFrame_ColumnInt8_New();