#include "dataframe/column.h"

typedef struct DataFrame_ColumnInt64 DataFrame_ColumnInt64;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnInt64* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnInt64* self);
    bool (*DecRef)(DataFrame_ColumnInt64* self);
    size_t (*GetSize)(DataFrame_ColumnInt64* self);
    void (*Remove)(DataFrame_ColumnInt64* self, size_t i);
    void (*Clear)(DataFrame_ColumnInt64* self);
    char* (*GetName)(DataFrame_ColumnInt64* self);
    const char* (*SetName)(DataFrame_ColumnInt64* self, const char* name);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnInt64* self, size_t index, int64_t* v);
    const char* (*Add)(DataFrame_ColumnInt64* self, int64_t v);
    const char* (*AddNA)(DataFrame_ColumnInt64* self);
    void (*Set)(DataFrame_ColumnInt64* self, size_t i, int64_t v);
    void (*SetNA)(DataFrame_ColumnInt64* self, size_t i);
} DataFrame_ColumnInt64Methods;

DATAFRAME_EXPORT DataFrame_ColumnInt64* DataFrame_ColumnInt64_New();