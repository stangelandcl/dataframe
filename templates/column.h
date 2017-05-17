#include "dataframe/config.h"
#include "dataframe/column.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataFrame_Column{{name}} DataFrame_Column{{name}};
typedef struct
{
    /* shared */
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame_Column{{name}}* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame_Column{{name}}* self);
    bool (*DecRef)(DataFrame_Column{{name}}* self);
    size_t (*GetSize)(DataFrame_Column{{name}}* self);
    void (*Remove)(DataFrame_Column{{name}}* self, size_t i);
    void (*Clear)(DataFrame_Column{{name}}* self);
    char* (*GetName)(DataFrame_Column{{name}}* self);
    const char* (*SetName)(DataFrame_Column{{name}}* self, const char* name);


    /* type specific */
    bool (*TryGet)(DataFrame_Column{{name}}* self, size_t index, {{type}}* v);
    const char* (*Add)(DataFrame_Column{{name}}* self, {{type}} v);
    const char* (*AddNA)(DataFrame_Column{{name}}* self);
    void (*Set)(DataFrame_Column{{name}}* self, size_t i, {{type}} v);
    void (*SetNA)(DataFrame_Column{{name}}* self, size_t i);
} DataFrame_Column{{name}}Methods;

DATAFRAME_EXPORT DataFrame_Column{{name}}* DataFrame_Column{{name}}_New();


#ifdef __cplusplus
}
#endif
