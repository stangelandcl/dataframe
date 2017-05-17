#include "dataframe/columnUInt16.h"
#include "dataframe/atomic.h"
#include <memory.h>

#define SELF ((DataFrame_ColumnUInt16Impl*)self)

typedef struct
{
    /* public */
    DataFrame_ColumnUInt16Methods* methods;

    /* private */
    volatile uint32_t ref_count;
    char* name;
    uint16_t* data;
    size_t size;
    size_t capacity;
    DataFrame_BitVector na;
} DataFrame_ColumnUInt16Impl;

static void*
Cast(DataFrame_ColumnUInt16* self, DataFrame_Type type)
{
    if(SELF->methods->_typeID == type)
        return self;
    return NULL;
}

static bool
IncRef(DataFrame_ColumnUInt16* self)
{
    return InterlockedIncrement(&SELF->ref_count) > 0;
}

static bool
DecRef(DataFrame_ColumnUInt16* self)
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
Size(DataFrame_ColumnUInt16* self)
{
    return SELF->size;
}

static void
Clear(DataFrame_ColumnUInt16* self)
{
    SELF->size = 0;
    SELF->na.size = 0;
}

static bool
TryGet(
    DataFrame_ColumnUInt16* self, size_t index, uint16_t* v)
{
    bool na = DataFrame_BitVector_Get(&SELF->na, index);
    if(na) return false;
    *v = SELF->data[index];
    return true;
}

static const char*
Resize(DataFrame_ColumnUInt16Impl* self)
{
    uint16_t* d;
    size_t newSize;

    newSize = SELF->size * 2;
    if(!newSize) newSize = 4;

    d = realloc(SELF->data, newSize + sizeof(uint16_t));
    if(!d) return "DataFrame_ColumnInt8_Add: Out of memory";
    SELF->data = d;
    SELF->capacity = newSize;

    return NULL;
}

static const char*
Add(DataFrame_ColumnUInt16* self, uint16_t v)
{
    size_t newSize;
    uint16_t* d;
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
Set(DataFrame_ColumnUInt16* self, size_t i, uint16_t v)
{
    DataFrame_BitVector_Set(&SELF->na, i, false);
    SELF->data[i] = v;
}


static const char*
AddNA(DataFrame_ColumnUInt16* self)
{
    size_t newSize;
    uint16_t* d;
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
Remove(DataFrame_ColumnUInt16* self, size_t i)
{
    DataFrame_BitVector_Remove(&SELF->na, i);
    memmove(&SELF->data[i], &SELF->data[i+1], SELF->size - i - 1);
    SELF->size--;
}


static void
SetNA(DataFrame_ColumnUInt16* self, size_t i)
{
    DataFrame_BitVector_Set(&SELF->na, i, true);
}

static char*
GetName(DataFrame_ColumnUInt16* self)
{
    return SELF->name;
}

static const char*
SetName(DataFrame_ColumnUInt16* self, const char* name)
{
    const char* e;
    char* n;

    if(!name)
    {
        SELF->name = NULL;
        return NULL;
    }

    n = strdup(name);
    if(!n) return "DataFrame_ColumnUInt16_SetName: Out of memory";

    SELF->name = n;
    return NULL;
}


static DataFrame_ColumnUInt16Methods UInt16Methods =
{
/* shared */
    DataFrame_Type_ColumnUInt16,
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


DataFrame_ColumnUInt16* DataFrame_ColumnUInt16_New()
{
    DataFrame_ColumnUInt16Impl* c;

    c = calloc(1, sizeof(DataFrame_ColumnUInt16Impl));
    if(!c) return NULL;
    c->methods = &UInt16Methods;
    c->ref_count = 1;
    return (DataFrame_ColumnUInt16*)c;
}