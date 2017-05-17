#include "dataframe/columnFloat64.h"
#include "dataframe/atomic.h"
#include <string.h>

#define SELF ((DataFrame_ColumnFloat64Impl*)self)

typedef struct
{
    /* public */
    DataFrame_ColumnFloat64Methods* methods;

    /* private */
    volatile uint32_t ref_count;
    char* name;
    double* data;
    size_t size;
    size_t capacity;
    DataFrame_BitVector na;
} DataFrame_ColumnFloat64Impl;

static void*
Cast(DataFrame_ColumnFloat64* self, DataFrame_Type type)
{
    if(SELF->methods->_typeID == type)
        return self;
    return NULL;
}

static bool
IncRef(DataFrame_ColumnFloat64* self)
{
    return InterlockedIncrement(&SELF->ref_count) > 0;
}

static bool
DecRef(DataFrame_ColumnFloat64* self)
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
Size(DataFrame_ColumnFloat64* self)
{
    return SELF->size;
}

static void
Clear(DataFrame_ColumnFloat64* self)
{
    SELF->size = 0;
    SELF->na.size = 0;
}

static bool
TryGet(
    DataFrame_ColumnFloat64* self, size_t index, double* v)
{
    bool na = DataFrame_BitVector_Get(&SELF->na, index);
    if(na) return false;
    *v = SELF->data[index];
    return true;
}

static const char*
Resize(DataFrame_ColumnFloat64Impl* self)
{
    double* d;
    size_t newSize;

    newSize = SELF->size * 2;
    if(!newSize) newSize = 4;

    d = realloc(SELF->data, newSize + sizeof(double));
    if(!d) return "DataFrame_ColumnInt8_Add: Out of memory";
    SELF->data = d;
    SELF->capacity = newSize;

    return NULL;
}

static const char*
Add(DataFrame_ColumnFloat64* self, double v)
{
    size_t newSize;
    double* d;
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
Set(DataFrame_ColumnFloat64* self, size_t i, double v)
{
    DataFrame_BitVector_Set(&SELF->na, i, false);
    SELF->data[i] = v;
}


static const char*
AddNA(DataFrame_ColumnFloat64* self)
{
    size_t newSize;
    double* d;
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
Remove(DataFrame_ColumnFloat64* self, size_t i)
{
    DataFrame_BitVector_Remove(&SELF->na, i);
    memmove(&SELF->data[i], &SELF->data[i+1], SELF->size - i - 1);
    SELF->size--;
}


static void
SetNA(DataFrame_ColumnFloat64* self, size_t i)
{
    DataFrame_BitVector_Set(&SELF->na, i, true);
}

static char*
GetName(DataFrame_ColumnFloat64* self)
{
    return SELF->name;
}

static const char*
SetName(DataFrame_ColumnFloat64* self, const char* name)
{
    const char* e;
    char* n;

    if(!name)
    {
        SELF->name = NULL;
        return NULL;
    }

    n = strdup(name);
    if(!n) return "DataFrame_ColumnFloat64_SetName: Out of memory";

    SELF->name = n;
    return NULL;
}


static DataFrame_ColumnFloat64Methods Float64Methods =
{
/* shared */
    DataFrame_Type_ColumnFloat64,
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


DataFrame_ColumnFloat64* DataFrame_ColumnFloat64_New()
{
    DataFrame_ColumnFloat64Impl* c;

    c = calloc(1, sizeof(DataFrame_ColumnFloat64Impl));
    if(!c) return NULL;
    c->methods = &Float64Methods;
    c->ref_count = 1;
    return (DataFrame_ColumnFloat64*)c;
}