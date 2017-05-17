#include "dataframe/config.h"
#include "dataframe/column.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataFrame_ColumnInt32 DataFrame_ColumnInt32;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnInt32* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnInt32* self);
    bool (*DecRef)(DataFrame_ColumnInt32* self);
    size_t (*Size)(DataFrame_ColumnInt32* self);
    void (*Remove)(DataFrame_ColumnInt32* self, size_t i);
    void (*Clear)(DataFrame_ColumnInt32* self);
    char* (*GetName)(DataFrame_ColumnInt32* self);
    const char* (*SetName)(DataFrame_ColumnInt32* self, const char* name);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnInt32* self, size_t index, int32_t* v);

    const char* (*Add)(DataFrame_ColumnInt32* self, int32_t v);

    const char* (*AddNA)(DataFrame_ColumnInt32* self);

    void (*Set)(DataFrame_ColumnInt32* self, size_t i, int32_t v);

    void (*SetNA)(DataFrame_ColumnInt32* self, size_t i);
} DataFrame_ColumnInt32Methods;

struct DataFrame_ColumnInt32
{
    DataFrame_ColumnInt32Methods* methods;
};


DATAFRAME_EXPORT DataFrame_ColumnInt32* DataFrame_ColumnInt32_New();


#ifdef __cplusplus
}
#endif