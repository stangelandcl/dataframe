#include "dataframe/column.h"

typedef struct DataFrame_ColumnCString DataFrame_ColumnCString;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnCString* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnCString* self);
    bool (*DecRef)(DataFrame_ColumnCString* self);
    size_t (*GetSize)(DataFrame_ColumnCString* self);
    void (*Remove)(DataFrame_ColumnCString* self, size_t i);
    void (*Clear)(DataFrame_ColumnCString* self);
    char* (*GetName)(DataFrame_ColumnCString* self);
    const char* (*SetName)(DataFrame_ColumnCString* self, const char* name);

    /* type specific */
    bool (*TryGet)(DataFrame_ColumnCString* self, size_t index, char** v);
    const char* (*Add)(DataFrame_ColumnCString* self, const char* v);
    const char* (*AddNA)(DataFrame_ColumnCString* self);
    void (*Set)(DataFrame_ColumnCString* self, size_t i, const char* v);
    void (*SetNA)(DataFrame_ColumnCString* self, size_t i);
} DataFrame_ColumnCStringMethods;

DATAFRAME_EXPORT DataFrame_ColumnCString* DataFrame_ColumnCString_New();
