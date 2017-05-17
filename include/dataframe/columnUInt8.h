#include "dataframe/config.h"
#include "dataframe/column.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataFrame_ColumnUInt8 DataFrame_ColumnUInt8;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnUInt8* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnUInt8* self);
    bool (*DecRef)(DataFrame_ColumnUInt8* self);
    size_t (*GetSize)(DataFrame_ColumnUInt8* self);
    void (*Remove)(DataFrame_ColumnUInt8* self, size_t i);
    void (*Clear)(DataFrame_ColumnUInt8* self);
    char* (*GetName)(DataFrame_ColumnUInt8* self);
    const char* (*SetName)(DataFrame_ColumnUInt8* self, const char* name);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnUInt8* self, size_t index, uint8_t* v);
    const char* (*Add)(DataFrame_ColumnUInt8* self, uint8_t v);
    const char* (*AddNA)(DataFrame_ColumnUInt8* self);
    void (*Set)(DataFrame_ColumnUInt8* self, size_t i, uint8_t v);
    void (*SetNA)(DataFrame_ColumnUInt8* self, size_t i);
} DataFrame_ColumnUInt8Methods;

DATAFRAME_EXPORT DataFrame_ColumnUInt8* DataFrame_ColumnUInt8_New();


#ifdef __cplusplus
}
#endif