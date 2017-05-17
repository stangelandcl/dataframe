#include "dataframe/column.h"

typedef struct DataFrame_ColumnUInt16 DataFrame_ColumnUInt16;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnUInt16* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnUInt16* self);
    bool (*DecRef)(DataFrame_ColumnUInt16* self);
    size_t (*GetSize)(DataFrame_ColumnUInt16* self);
    void (*Remove)(DataFrame_ColumnUInt16* self, size_t i);
    void (*Clear)(DataFrame_ColumnUInt16* self);
    char* (*GetName)(DataFrame_ColumnUInt16* self);
    const char* (*SetName)(DataFrame_ColumnUInt16* self, const char* name);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnUInt16* self, size_t index, uint16_t* v);
    const char* (*Add)(DataFrame_ColumnUInt16* self, uint16_t v);
    const char* (*AddNA)(DataFrame_ColumnUInt16* self);
    void (*Set)(DataFrame_ColumnUInt16* self, size_t i, uint16_t v);
    void (*SetNA)(DataFrame_ColumnUInt16* self, size_t i);
} DataFrame_ColumnUInt16Methods;

DATAFRAME_EXPORT DataFrame_ColumnUInt16* DataFrame_ColumnUInt16_New();