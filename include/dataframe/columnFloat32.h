#include "dataframe/config.h"
#include "dataframe/column.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataFrame_ColumnFloat32 DataFrame_ColumnFloat32;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnFloat32* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnFloat32* self);
    bool (*DecRef)(DataFrame_ColumnFloat32* self);
    size_t (*Size)(DataFrame_ColumnFloat32* self);
    void (*Remove)(DataFrame_ColumnFloat32* self, size_t i);
    void (*Clear)(DataFrame_ColumnFloat32* self);
    char* (*GetName)(DataFrame_ColumnFloat32* self);
    const char* (*SetName)(DataFrame_ColumnFloat32* self, const char* name);
    bool (*HasValue)(DataFrame_ColumnFloat32* self, size_t index);
    uint8_t* (*GetNAs)(DataFrame_ColumnFloat32* self);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnFloat32* self, size_t index, float* v);
    /* returns a reference */
    float* (*Get)(DataFrame_ColumnFloat32* self, size_t index);

    const char* (*Add)(DataFrame_ColumnFloat32* self, float v);

    const char* (*AddNA)(DataFrame_ColumnFloat32* self);

    void (*Set)(DataFrame_ColumnFloat32* self, size_t i, float v);

    void (*SetNA)(DataFrame_ColumnFloat32* self, size_t i);
} DataFrame_ColumnFloat32Methods;

struct DataFrame_ColumnFloat32
{
    DataFrame_ColumnFloat32Methods* methods;
};


DATAFRAME_EXPORT DataFrame_ColumnFloat32* DataFrame_ColumnFloat32_New();


#ifdef __cplusplus
}
#endif