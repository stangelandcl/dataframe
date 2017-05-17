#include "dataframe/config.h"
#include "dataframe/column.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataFrame_ColumnUInt32 DataFrame_ColumnUInt32;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnUInt32* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnUInt32* self);
    bool (*DecRef)(DataFrame_ColumnUInt32* self);
    size_t (*Size)(DataFrame_ColumnUInt32* self);
    void (*Remove)(DataFrame_ColumnUInt32* self, size_t i);
    void (*Clear)(DataFrame_ColumnUInt32* self);
    char* (*GetName)(DataFrame_ColumnUInt32* self);
    const char* (*SetName)(DataFrame_ColumnUInt32* self, const char* name);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnUInt32* self, size_t index, uint32_t* v);

    const char* (*Add)(DataFrame_ColumnUInt32* self, uint32_t v);

    const char* (*AddNA)(DataFrame_ColumnUInt32* self);

    void (*Set)(DataFrame_ColumnUInt32* self, size_t i, uint32_t v);

    void (*SetNA)(DataFrame_ColumnUInt32* self, size_t i);
} DataFrame_ColumnUInt32Methods;

struct DataFrame_ColumnUInt32
{
    DataFrame_ColumnUInt32Methods* methods;
};


DATAFRAME_EXPORT DataFrame_ColumnUInt32* DataFrame_ColumnUInt32_New();


#ifdef __cplusplus
}
#endif