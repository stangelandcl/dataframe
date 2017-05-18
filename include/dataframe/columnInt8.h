#include "dataframe/config.h"
#include "dataframe/column.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataFrame_ColumnInt8 DataFrame_ColumnInt8;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnInt8* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnInt8* self);
    bool (*DecRef)(DataFrame_ColumnInt8* self);
    size_t (*Size)(DataFrame_ColumnInt8* self);
    void (*Remove)(DataFrame_ColumnInt8* self, size_t i);
    void (*Clear)(DataFrame_ColumnInt8* self);
    char* (*GetName)(DataFrame_ColumnInt8* self);
    const char* (*SetName)(DataFrame_ColumnInt8* self, const char* name);
    bool (*HasValue)(DataFrame_ColumnInt8* self, size_t index);
    uint8_t* (*GetNAs)(DataFrame_ColumnInt8* self);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnInt8* self, size_t index, int8_t* v);
    /* returns a reference */
    int8_t* (*Get)(DataFrame_ColumnInt8* self, size_t index);

    const char* (*Add)(DataFrame_ColumnInt8* self, int8_t v);

    const char* (*AddNA)(DataFrame_ColumnInt8* self);

    void (*Set)(DataFrame_ColumnInt8* self, size_t i, int8_t v);

    void (*SetNA)(DataFrame_ColumnInt8* self, size_t i);
} DataFrame_ColumnInt8Methods;

struct DataFrame_ColumnInt8
{
    DataFrame_ColumnInt8Methods* methods;
};


DATAFRAME_EXPORT DataFrame_ColumnInt8* DataFrame_ColumnInt8_New();


#ifdef __cplusplus
}
#endif