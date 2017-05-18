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
    size_t (*Size)(DataFrame_Column{{name}}* self);
    void (*Remove)(DataFrame_Column{{name}}* self, size_t i);
    void (*Clear)(DataFrame_Column{{name}}* self);
    char* (*GetName)(DataFrame_Column{{name}}* self);
    const char* (*SetName)(DataFrame_Column{{name}}* self, const char* name);
    bool (*HasValue)(DataFrame_Column{{name}}* self, size_t index);
    uint8_t* (*GetNAs)(DataFrame_Column{{name}}* self);


    /* type specific */
    bool (*TryGet)(DataFrame_Column{{name}}* self, size_t index, {{type}}* v);
    /* returns a reference */
    {{type}}* (*Get)(DataFrame_Column{{name}}* self, size_t index);

    {% if type == "char*" %}
    const char* (*Add)(DataFrame_Column{{name}}* self, const {{type}} v);
    {% else %}
    const char* (*Add)(DataFrame_Column{{name}}* self, {{type}} v);
    {% endif %}

    const char* (*AddNA)(DataFrame_Column{{name}}* self);

    {% if type == "char*" %}
    void (*Set)(DataFrame_Column{{name}}* self, size_t i, const {{type}} v);
    {% else %}
    void (*Set)(DataFrame_Column{{name}}* self, size_t i, {{type}} v);
    {% endif %}

    void (*SetNA)(DataFrame_Column{{name}}* self, size_t i);
} DataFrame_Column{{name}}Methods;

struct DataFrame_Column{{name}}
{
    DataFrame_Column{{name}}Methods* methods;
};


DATAFRAME_EXPORT DataFrame_Column{{name}}* DataFrame_Column{{name}}_New();


#ifdef __cplusplus
}
#endif
