#include "dataframe/columnCString.h"
#include "dataframe/atomic.h"
#include <memory.h>

#define SELF ((DataFrame_ColumnCStringImpl*)self)

typedef struct
{
    /* public */
    DataFrame_ColumnCStringMethods* methods;

    /* private */
    volatile uint32_t ref_count;
    char* name;
    char** data;
    size_t size;
    size_t capacity;
    DataFrame_BitVector na;
} DataFrame_ColumnCStringImpl;

static void*
Cast(DataFrame_ColumnCString* self, DataFrame_Type type)
{
    if(SELF->methods->_typeID == type)
        return self;
    return NULL;
}

static bool
IncRef(DataFrame_ColumnCString* self)
{
    return InterlockedIncrement(&SELF->ref_count) > 0;
}

static bool
DecRef(DataFrame_ColumnCString* self)
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
Size(DataFrame_ColumnCString* self)
{
    return SELF->size;
}

static void
Clear(DataFrame_ColumnCString* self)
{
    SELF->size = 0;
    SELF->na.size = 0;
}

static bool
TryGet(
    DataFrame_ColumnCString* self, size_t index, char** v)
{
    bool na = DataFrame_BitVector_Get(&SELF->na, index);
    if(na) return false;
    *v = SELF->data[index];
    return true;
}

static const char*
Resize(DataFrame_ColumnCStringImpl* self)
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
Add(DataFrame_ColumnCString* self, char* v)
{
    size_t newSize;
    char** d;
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
Set(DataFrame_ColumnCString* self, size_t i, char* v)
{
    DataFrame_BitVector_Set(&SELF->na, i, false);
    SELF->data[i] = v;
}


static const char*
AddNA(DataFrame_ColumnCString* self)
{
    size_t newSize;
    char** d;
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
Remove(DataFrame_ColumnCString* self, size_t i)
{
    DataFrame_BitVector_Remove(&SELF->na, i);
    memmove(&SELF->data[i], &SELF->data[i+1], SELF->size - i - 1);
    SELF->size--;
}


static void
SetNA(DataFrame_ColumnCString* self, size_t i)
{
    DataFrame_BitVector_Set(&SELF->na, i, true);
}

static char*
GetName(DataFrame_ColumnCString* self)
{
    return SELF->name;
}

static const char*
SetName(DataFrame_ColumnCString* self, const char* name)
{
    const char* e;
    char* n;

    if(!name)
    {
        SELF->name = NULL;
        return NULL;
    }

    n = strdup(name);
    if(!n) return "DataFrame_ColumnCString_SetName: Out of memory";

    SELF->name = n;
    return NULL;
}


static DataFrame_ColumnCStringMethods CStringMethods =
{
/* shared */
    DataFrame_Type_ColumnCString,
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


DataFrame_ColumnCString* DataFrame_ColumnCString_New()
{
    DataFrame_ColumnCStringImpl* c;

    c = calloc(1, sizeof(DataFrame_ColumnCStringImpl));
    if(!c) return NULL;
    c->methods = &CStringMethods;
    c->ref_count = 1;
    return (DataFrame_ColumnCString*)c;
}