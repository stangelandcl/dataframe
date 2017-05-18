#include "dataframe/config.h"
#include "dataframe/column.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataFrame_ColumnInt64 DataFrame_ColumnInt64;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnInt64* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnInt64* self);
    bool (*DecRef)(DataFrame_ColumnInt64* self);
    size_t (*Size)(DataFrame_ColumnInt64* self);
    void (*Remove)(DataFrame_ColumnInt64* self, size_t i);
    void (*Clear)(DataFrame_ColumnInt64* self);
    char* (*GetName)(DataFrame_ColumnInt64* self);
    const char* (*SetName)(DataFrame_ColumnInt64* self, const char* name);
    bool (*HasValue)(DataFrame_ColumnInt64* self, size_t index);
    uint8_t* (*GetNAs)(DataFrame_ColumnInt64* self);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnInt64* self, size_t index, int64_t* v);
    /* returns a reference */
    int64_t* (*Get)(DataFrame_ColumnInt64* self, size_t index);

    const char* (*Add)(DataFrame_ColumnInt64* self, int64_t v);

    const char* (*AddNA)(DataFrame_ColumnInt64* self);

    void (*Set)(DataFrame_ColumnInt64* self, size_t i, int64_t v);

    void (*SetNA)(DataFrame_ColumnInt64* self, size_t i);
} DataFrame_ColumnInt64Methods;

struct DataFrame_ColumnInt64
{
    DataFrame_ColumnInt64Methods* methods;
};


DATAFRAME_EXPORT DataFrame_ColumnInt64* DataFrame_ColumnInt64_New();


#ifdef __cplusplus
}
#endif