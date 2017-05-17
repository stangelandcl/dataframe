#include "dataframe/column{{name}}.h"
#include "dataframe/atomic.h"
#include <string.h>

#define SELF ((DataFrame_Column{{name}}Impl*)self)

typedef struct
{
    /* public */
    DataFrame_Column{{name}}Methods* methods;

    /* private */
    volatile uint32_t ref_count;
    char* name;
    {{type}}* data;
    size_t size;
    size_t capacity;
    DataFrame_BitVector na;
} DataFrame_Column{{name}}Impl;

static void*
Cast(DataFrame_Column{{name}}* self, DataFrame_Type type)
{
    if(SELF->methods->_typeID == type)
        return self;
    return NULL;
}

static bool
IncRef(DataFrame_Column{{name}}* self)
{
    return InterlockedIncrement(&SELF->ref_count) > 0;
}

static bool
DecRef(DataFrame_Column{{name}}* self)
{
    if(!InterlockedDecrement(&SELF->ref_count))
    {
        free(SELF->data);
        DataFrame_BitVector_Destroy(&SELF->na);
        free(SELF);
        return false;
    }
    return true;
}

static size_t
Size(DataFrame_Column{{name}}* self)
{
    return SELF->size;
}

static void
Clear(DataFrame_Column{{name}}* self)
{
    SELF->size = 0;
    SELF->na.size = 0;
}

static bool
TryGet(
    DataFrame_Column{{name}}* self, size_t index, {{type}}* v)
{
    bool na = DataFrame_BitVector_Get(&SELF->na, index);
    if(na) return false;
    *v = SELF->data[index];
    return true;
}

static const char*
Resize(DataFrame_Column{{name}}Impl* self)
{
    {{type}}* d;
    size_t newSize;

    newSize = SELF->size * 2;
    if(!newSize) newSize = 4;

    d = realloc(SELF->data, newSize + sizeof({{type}}));
    if(!d) return "DataFrame_ColumnInt8_Add: Out of memory";
    SELF->data = d;
    SELF->capacity = newSize;

    return NULL;
}

{% if "{{type}}" == "char*" %}

static const char*
Add(DataFrame_Column{{name}}* self, const char* v)
{
    size_t newSize;
    char** d;
    char* v2;
    bool na = v ? false : true;

    const char* e = DataFrame_BitVector_Add(&SELF->na, na);
    if(e) return e;

    if(SELF->size == SELF->capacity)
    {
        e = Resize(SELF);
        if(e) return e;
    }

    if(na)
        SELF->data[SELF->size++] = NULL;
    else
    {
        v2 = strdup(v);
        if(!v2)
            return "DataFrame_ColumnCString_Add: Out of memory";
        SELF->data[SELF->size++] = v2;
    }
    return NULL;
}

static void
Set(DataFrame_Column{{name}}* self, size_t i, const char* v)
{
    bool na = v ? false : true;
    if(na)
    {
        DataFrame_BitVector_Set(&SELF->na, i, na);
        SELF->data[i] = NULL;
    }
    else
    {
        SELF->data[i] = strdup(v);
        DataFrame_BitVector_Set(&SELF->na, i, !na && SELF->data[i]);
    }
}

{% else %}
static const char*
Add(DataFrame_Column{{name}}* self, {{type}} v)
{
    size_t newSize;
    {{type}}* d;
    const char* e = DataFrame_BitVector_Add(&SELF->na, false);
    if(e) return e;

    if(SELF->size == SELF->capacity)
    {
        e = Resize(SELF);
        if(e) return e;
    }


    SELF->data[SELF->size++] = v;
    return NULL;
}

static void
Set(DataFrame_Column{{name}}* self, size_t i, {{type}} v)
{
    DataFrame_BitVector_Set(&SELF->na, i, false);
    SELF->data[i] = v;
}

{% endif %}

static const char*
AddNA(DataFrame_Column{{name}}* self)
{
    size_t newSize;
    {{type}}* d;
    const char* e = DataFrame_BitVector_Add(&SELF->na, true);
    if(e) return e;

    if(SELF->size == SELF->capacity)
    {
        e = Resize(SELF);
        if(e) return e;
    }

    SELF->size++;
    return NULL;
}

static void
Remove(DataFrame_Column{{name}}* self, size_t i)
{
    DataFrame_BitVector_Remove(&SELF->na, i);
    memmove(&SELF->data[i], &SELF->data[i+1], SELF->size - i - 1);
    SELF->size--;
}


static void
SetNA(DataFrame_Column{{name}}* self, size_t i)
{
    DataFrame_BitVector_Set(&SELF->na, i, true);
}

static char*
GetName(DataFrame_Column{{name}}* self)
{
    return SELF->name;
}

static const char*
SetName(DataFrame_Column{{name}}* self, const char* name)
{
    const char* e;
    char* n;

    if(!name)
    {
        SELF->name = NULL;
        return NULL;
    }

    n = strdup(name);
    if(!n) return "DataFrame_Column{{name}}_SetName: Out of memory";

    SELF->name = n;
    return NULL;
}


static DataFrame_Column{{name}}Methods {{name}}Methods =
{
/* shared */
    DataFrame_Type_Column{{name}},
    Cast,
    IncRef,
    DecRef,
    Size,
    Remove,
    Clear,
    GetName,
    SetName,

/* type specific */
    TryGet,
    Add,
    AddNA,
    Set,
    SetNA
};


DataFrame_Column{{name}}* DataFrame_Column{{name}}_New()
{
    DataFrame_Column{{name}}Impl* c;

    c = calloc(1, sizeof(DataFrame_Column{{name}}Impl));
    if(!c) return NULL;
    c->methods = &{{name}}Methods;
    c->ref_count = 1;
    return (DataFrame_Column{{name}}*)c;
}
