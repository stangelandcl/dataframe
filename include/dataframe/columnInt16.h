#include "dataframe/config.h"
#include "dataframe/column.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataFrame_ColumnInt16 DataFrame_ColumnInt16;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnInt16* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnInt16* self);
    bool (*DecRef)(DataFrame_ColumnInt16* self);
    size_t (*GetSize)(DataFrame_ColumnInt16* self);
    void (*Remove)(DataFrame_ColumnInt16* self, size_t i);
    void (*Clear)(DataFrame_ColumnInt16* self);
    char* (*GetName)(DataFrame_ColumnInt16* self);
    const char* (*SetName)(DataFrame_ColumnInt16* self, const char* name);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnInt16* self, size_t index, int16_t* v);
    const char* (*Add)(DataFrame_ColumnInt16* self, int16_t v);
    const char* (*AddNA)(DataFrame_ColumnInt16* self);
    void (*Set)(DataFrame_ColumnInt16* self, size_t i, int16_t v);
    void (*SetNA)(DataFrame_ColumnInt16* self, size_t i);
} DataFrame_ColumnInt16Methods;

DATAFRAME_EXPORT DataFrame_ColumnInt16* DataFrame_ColumnInt16_New();


#ifdef __cplusplus
}
#endif