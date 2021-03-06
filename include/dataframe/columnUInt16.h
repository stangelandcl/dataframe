#include "dataframe/config.h"
#include "dataframe/column.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataFrame_ColumnUInt16 DataFrame_ColumnUInt16;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnUInt16* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnUInt16* self);
    bool (*DecRef)(DataFrame_ColumnUInt16* self);
    size_t (*Size)(DataFrame_ColumnUInt16* self);
    void (*Remove)(DataFrame_ColumnUInt16* self, size_t i);
    void (*Clear)(DataFrame_ColumnUInt16* self);
    char* (*GetName)(DataFrame_ColumnUInt16* self);
    const char* (*SetName)(DataFrame_ColumnUInt16* self, const char* name);
    bool (*HasValue)(DataFrame_ColumnUInt16* self, size_t index);
    uint8_t* (*GetNAs)(DataFrame_ColumnUInt16* self);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnUInt16* self, size_t index, uint16_t* v);
    /* returns a reference */
    uint16_t* (*Get)(DataFrame_ColumnUInt16* self, size_t index);

    const char* (*Add)(DataFrame_ColumnUInt16* self, uint16_t v);

    const char* (*AddNA)(DataFrame_ColumnUInt16* self);

    void (*Set)(DataFrame_ColumnUInt16* self, size_t i, uint16_t v);

    void (*SetNA)(DataFrame_ColumnUInt16* self, size_t i);
} DataFrame_ColumnUInt16Methods;

struct DataFrame_ColumnUInt16
{
    DataFrame_ColumnUInt16Methods* methods;
};


DATAFRAME_EXPORT DataFrame_ColumnUInt16* DataFrame_ColumnUInt16_New();


#ifdef __cplusplus
}
#endif