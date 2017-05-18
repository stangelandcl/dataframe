#include "dataframe/config.h"
#include "dataframe/column.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataFrame_ColumnString DataFrame_ColumnString;
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_ColumnString* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_ColumnString* self);
    bool (*DecRef)(DataFrame_ColumnString* self);
    size_t (*Size)(DataFrame_ColumnString* self);
    void (*Remove)(DataFrame_ColumnString* self, size_t i);
    void (*Clear)(DataFrame_ColumnString* self);
    char* (*GetName)(DataFrame_ColumnString* self);
    const char* (*SetName)(DataFrame_ColumnString* self, const char* name);
    bool (*HasValue)(DataFrame_ColumnString* self, size_t index);
    uint8_t* (*GetNAs)(DataFrame_ColumnString* self);


    /* type specific */
    bool (*TryGet)(DataFrame_ColumnString* self, size_t index, char** v);
    /* returns a reference */
    char** (*Get)(DataFrame_ColumnString* self, size_t index);

    const char* (*Add)(DataFrame_ColumnString* self, const char* v);

    const char* (*AddNA)(DataFrame_ColumnString* self);

    void (*Set)(DataFrame_ColumnString* self, size_t i, const char* v);

    void (*SetNA)(DataFrame_ColumnString* self, size_t i);
} DataFrame_ColumnStringMethods;

struct DataFrame_ColumnString
{
    DataFrame_ColumnStringMethods* methods;
};


DATAFRAME_EXPORT DataFrame_ColumnString* DataFrame_ColumnString_New();


#ifdef __cplusplus
}
#endif