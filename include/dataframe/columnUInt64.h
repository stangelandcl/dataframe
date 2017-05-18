#include "dataframe/config.h"
#include "dataframe/column.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataFrame_ColumnUInt64 DataFrame_ColumnUInt64;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnUInt64* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnUInt64* self);
    bool (*DecRef)(DataFrame_ColumnUInt64* self);
    size_t (*Size)(DataFrame_ColumnUInt64* self);
    void (*Remove)(DataFrame_ColumnUInt64* self, size_t i);
    void (*Clear)(DataFrame_ColumnUInt64* self);
    char* (*GetName)(DataFrame_ColumnUInt64* self);
    const char* (*SetName)(DataFrame_ColumnUInt64* self, const char* name);
    bool (*HasValue)(DataFrame_ColumnUInt64* self, size_t index);
    uint8_t* (*GetNAs)(DataFrame_ColumnUInt64* self);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnUInt64* self, size_t index, uint64_t* v);
    /* returns a reference */
    uint64_t* (*Get)(DataFrame_ColumnUInt64* self, size_t index);

    const char* (*Add)(DataFrame_ColumnUInt64* self, uint64_t v);

    const char* (*AddNA)(DataFrame_ColumnUInt64* self);

    void (*Set)(DataFrame_ColumnUInt64* self, size_t i, uint64_t v);

    void (*SetNA)(DataFrame_ColumnUInt64* self, size_t i);
} DataFrame_ColumnUInt64Methods;

struct DataFrame_ColumnUInt64
{
    DataFrame_ColumnUInt64Methods* methods;
};


DATAFRAME_EXPORT DataFrame_ColumnUInt64* DataFrame_ColumnUInt64_New();


#ifdef __cplusplus
}
#endif