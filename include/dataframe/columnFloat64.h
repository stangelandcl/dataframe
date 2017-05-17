#include "dataframe/column.h"

typedef struct DataFrame_ColumnFloat64 DataFrame_ColumnFloat64;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnFloat64* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnFloat64* self);
    bool (*DecRef)(DataFrame_ColumnFloat64* self);
    size_t (*GetSize)(DataFrame_ColumnFloat64* self);
    void (*Remove)(DataFrame_ColumnFloat64* self, size_t i);
    void (*Clear)(DataFrame_ColumnFloat64* self);
    char* (*GetName)(DataFrame_ColumnFloat64* self);
    const char* (*SetName)(DataFrame_ColumnFloat64* self, const char* name);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnFloat64* self, size_t index, double* v);
    const char* (*Add)(DataFrame_ColumnFloat64* self, double v);
    const char* (*AddNA)(DataFrame_ColumnFloat64* self);
    void (*Set)(DataFrame_ColumnFloat64* self, size_t i, double v);
    void (*SetNA)(DataFrame_ColumnFloat64* self, size_t i);
} DataFrame_ColumnFloat64Methods;

DATAFRAME_EXPORT DataFrame_ColumnFloat64* DataFrame_ColumnFloat64_New();