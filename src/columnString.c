#include "dataframe/columnString.h"
#include "dataframe/atomic.h"
#include <string.h>

#define SELF ((DataFrame_ColumnStringImpl*)self)

#ifdef _MSC_VER
   #define strdup _strdup
#endif

typedef struct
{
    /* public */
    DataFrame_ColumnStringMethods* methods;

    /* private */
    volatile uint32_t ref_count;
    char* name;
    char** data;
    size_t size;
    size_t capacity;
    DataFrame_BitVector na;
} DataFrame_ColumnStringImpl;

static void*
Cast(DataFrame_ColumnString* self, DataFrame_Type type)
{
    if(SELF->methods->_typeID == type)
        return self;
    return NULL;
}

static bool
IncRef(DataFrame_ColumnString* self)
{
    return InterlockedIncrement(&SELF->ref_count) > 0;
}

static bool
DecRef(DataFrame_ColumnString* self)
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
Size(DataFrame_ColumnString* self)
{
    return SELF->size;
}

static void
Clear(DataFrame_ColumnString* self)
{
    SELF->size = 0;
    SELF->na.size = 0;
}

static bool
TryGet(
    DataFrame_ColumnString* self, size_t index, char** v)
{
    bool na = DataFrame_BitVector_Get(&SELF->na, index);
    if(na) return false;
    *v = SELF->data[index];
    return true;
}

static const char*
Resize(DataFrame_ColumnStringImpl* self)
{
    char** d;
    size_t newSize;

    newSize = SELF->size * 2;
    if(!newSize) newSize = 4;

    d = realloc(SELF->data, newSize + sizeof(char*));
    if(!d) return "DataFrame_ColumnInt8_Add: Out of memory";
    SELF->data = d;
    SELF->capacity = newSize;

    return NULL;
}


static const char*
Add(DataFrame_ColumnString* self, const char* v)
{
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
Set(DataFrame_ColumnString* self, size_t i, const char* v)
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


static const char*
AddNA(DataFrame_ColumnString* self)
{
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
Remove(DataFrame_ColumnString* self, size_t i)
{
    DataFrame_BitVector_Remove(&SELF->na, i);
    memmove(&SELF->data[i], &SELF->data[i+1], SELF->size - i - 1);
    SELF->size--;
}


static void
SetNA(DataFrame_ColumnString* self, size_t i)
{
    DataFrame_BitVector_Set(&SELF->na, i, true);
}

static char*
GetName(DataFrame_ColumnString* self)
{
    return SELF->name;
}

static const char*
SetName(DataFrame_ColumnString* self, const char* name)
{
    char* n;

    if(!name)
    {
        SELF->name = NULL;
        return NULL;
    }

    n = strdup(name);
    if(!n) return "DataFrame_ColumnString_SetName: Out of memory";

    SELF->name = n;
    return NULL;
}

static bool
HasValue(DataFrame_ColumnString* self, size_t index)
{
    bool na = DataFrame_BitVector_Get(&SELF->na, index);
    return !na;
}

static char**
Get(DataFrame_ColumnString* self, size_t index)
{
    return &SELF->data[index];
}

static uint8_t*
GetNAs(DataFrame_ColumnString* self)
{
    return SELF->na.data;
}

static DataFrame_ColumnStringMethods StringMethods =
{
/* shared */
    DataFrame_Type_ColumnString,
    Cast,
    IncRef,
    DecRef,
    Size,
    Remove,
    Clear,
    GetName,
    SetName,
    HasValue,
    GetNAs,

/* type specific */
    TryGet,
    Get,
    Add,
    AddNA,
    Set,
    SetNA
};


DataFrame_ColumnString* DataFrame_ColumnString_New()
{
    DataFrame_ColumnStringImpl* c;

    c = calloc(1, sizeof(DataFrame_ColumnStringImpl));
    if(!c) return NULL;
    c->methods = &StringMethods;
    c->ref_count = 1;
    return (DataFrame_ColumnString*)c;
}