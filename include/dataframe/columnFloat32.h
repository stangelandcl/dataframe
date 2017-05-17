#include "dataframe/column.h"

typedef struct DataFrame_ColumnFloat32 DataFrame_ColumnFloat32;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnFloat32* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnFloat32* self);
    bool (*DecRef)(DataFrame_ColumnFloat32* self);
    size_t (*GetSize)(DataFrame_ColumnFloat32* self);
    void (*Remove)(DataFrame_ColumnFloat32* self, size_t i);
    void (*Clear)(DataFrame_ColumnFloat32* self);
    char* (*GetName)(DataFrame_ColumnFloat32* self);
    const char* (*SetName)(DataFrame_ColumnFloat32* self, const char* name);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnFloat32* self, size_t index, float* v);
    const char* (*Add)(DataFrame_ColumnFloat32* self, float v);
    const char* (*AddNA)(DataFrame_ColumnFloat32* self);
    void (*Set)(DataFrame_ColumnFloat32* self, size_t i, float v);
    void (*SetNA)(DataFrame_ColumnFloat32* self, size_t i);
} DataFrame_ColumnFloat32Methods;

DATAFRAME_EXPORT DataFrame_ColumnFloat32* DataFrame_ColumnFloat32_New();