#include "dataframe/columnUInt32.h"
#include "dataframe/atomic.h"
#include <string.h>

#define SELF ((DataFrame_ColumnUInt32Impl*)self)

typedef struct
{
    /* public */
    DataFrame_ColumnUInt32Methods* methods;

    /* private */
    volatile uint32_t ref_count;
    char* name;
    uint32_t* data;
    size_t size;
    size_t capacity;
    DataFrame_BitVector na;
} DataFrame_ColumnUInt32Impl;

static void*
Cast(DataFrame_ColumnUInt32* self, DataFrame_Type type)
{
    if(SELF->methods->_typeID == type)
        return self;
    return NULL;
}

static bool
IncRef(DataFrame_ColumnUInt32* self)
{
    return InterlockedIncrement(&SELF->ref_count) > 0;
}

static bool
DecRef(DataFrame_ColumnUInt32* self)
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
Size(DataFrame_ColumnUInt32* self)
{
    return SELF->size;
}

static void
Clear(DataFrame_ColumnUInt32* self)
{
    SELF->size = 0;
    SELF->na.size = 0;
}

static bool
TryGet(
    DataFrame_ColumnUInt32* self, size_t index, uint32_t* v)
{
    bool na = DataFrame_BitVector_Get(&SELF->na, index);
    if(na) return false;
    *v = SELF->data[index];
    return true;
}

static const char*
Resize(DataFrame_ColumnUInt32Impl* self)
{
    uint32_t* d;
    size_t newSize;

    newSize = SELF->size * 2;
    if(!newSize) newSize = 4;

    d = realloc(SELF->data, newSize + sizeof(uint32_t));
    if(!d) return "DataFrame_ColumnInt8_Add: Out of memory";
    SELF->data = d;
    SELF->capacity = newSize;

    return NULL;
}

static const char*
Add(DataFrame_ColumnUInt32* self, uint32_t v)
{
    size_t newSize;
    uint32_t* d;
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
Set(DataFrame_ColumnUInt32* self, size_t i, uint32_t v)
{
    DataFrame_BitVector_Set(&SELF->na, i, false);
    SELF->data[i] = v;
}


static const char*
AddNA(DataFrame_ColumnUInt32* self)
{
    size_t newSize;
    uint32_t* d;
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
Remove(DataFrame_ColumnUInt32* self, size_t i)
{
    DataFrame_BitVector_Remove(&SELF->na, i);
    memmove(&SELF->data[i], &SELF->data[i+1], SELF->size - i - 1);
    SELF->size--;
}


static void
SetNA(DataFrame_ColumnUInt32* self, size_t i)
{
    DataFrame_BitVector_Set(&SELF->na, i, true);
}

static char*
GetName(DataFrame_ColumnUInt32* self)
{
    return SELF->name;
}

static const char*
SetName(DataFrame_ColumnUInt32* self, const char* name)
{
    const char* e;
    char* n;

    if(!name)
    {
        SELF->name = NULL;
        return NULL;
    }

    n = strdup(name);
    if(!n) return "DataFrame_ColumnUInt32_SetName: Out of memory";

    SELF->name = n;
    return NULL;
}


static DataFrame_ColumnUInt32Methods UInt32Methods =
{
/* shared */
    DataFrame_Type_ColumnUInt32,
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


DataFrame_ColumnUInt32* DataFrame_ColumnUInt32_New()
{
    DataFrame_ColumnUInt32Impl* c;

    c = calloc(1, sizeof(DataFrame_ColumnUInt32Impl));
    if(!c) return NULL;
    c->methods = &UInt32Methods;
    c->ref_count = 1;
    return (DataFrame_ColumnUInt32*)c;
}