#include "dataframe/columnInt64.h"
#include "dataframe/atomic.h"
#include <string.h>

#define SELF ((DataFrame_ColumnInt64Impl*)self)

#ifdef _MSC_VER
   #define strdup _strdup
#endif

typedef struct
{
    /* public */
    DataFrame_ColumnInt64Methods* methods;

    /* private */
    volatile uint32_t ref_count;
    char* name;
    int64_t* data;
    size_t size;
    size_t capacity;
    DataFrame_BitVector na;
} DataFrame_ColumnInt64Impl;

static void*
Cast(DataFrame_ColumnInt64* self, DataFrame_Type type)
{
    if(SELF->methods->_typeID == type)
        return self;
    return NULL;
}

static bool
IncRef(DataFrame_ColumnInt64* self)
{
    return InterlockedIncrement(&SELF->ref_count) > 0;
}

static bool
DecRef(DataFrame_ColumnInt64* self)
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
Size(DataFrame_ColumnInt64* self)
{
    return SELF->size;
}

static void
Clear(DataFrame_ColumnInt64* self)
{
    SELF->size = 0;
    SELF->na.size = 0;
}

static bool
TryGet(
    DataFrame_ColumnInt64* self, size_t index, int64_t* v)
{
    bool na = DataFrame_BitVector_Get(&SELF->na, index);
    if(na) return false;
    *v = SELF->data[index];
    return true;
}

static const char*
Resize(DataFrame_ColumnInt64Impl* self)
{
    int64_t* d;
    size_t newSize;

    newSize = SELF->size * 2;
    if(!newSize) newSize = 4;

    d = realloc(SELF->data, newSize + sizeof(int64_t));
    if(!d) return "DataFrame_ColumnInt8_Add: Out of memory";
    SELF->data = d;
    SELF->capacity = newSize;

    return NULL;
}

static const char*
Add(DataFrame_ColumnInt64* self, int64_t v)
{
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
Set(DataFrame_ColumnInt64* self, size_t i, int64_t v)
{
    DataFrame_BitVector_Set(&SELF->na, i, false);
    SELF->data[i] = v;
}


static const char*
AddNA(DataFrame_ColumnInt64* self)
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
Remove(DataFrame_ColumnInt64* self, size_t i)
{
    DataFrame_BitVector_Remove(&SELF->na, i);
    memmove(&SELF->data[i], &SELF->data[i+1], SELF->size - i - 1);
    SELF->size--;
}


static void
SetNA(DataFrame_ColumnInt64* self, size_t i)
{
    DataFrame_BitVector_Set(&SELF->na, i, true);
}

static char*
GetName(DataFrame_ColumnInt64* self)
{
    return SELF->name;
}

static const char*
SetName(DataFrame_ColumnInt64* self, const char* name)
{
    char* n;

    if(!name)
    {
        SELF->name = NULL;
        return NULL;
    }

    n = strdup(name);
    if(!n) return "DataFrame_ColumnInt64_SetName: Out of memory";

    SELF->name = n;
    return NULL;
}

static bool
HasValue(DataFrame_ColumnInt64* self, size_t index)
{
    bool na = DataFrame_BitVector_Get(&SELF->na, index);
    return !na;
}

static int64_t*
Get(DataFrame_ColumnInt64* self, size_t index)
{
    return &SELF->data[index];
}

static uint8_t*
GetNAs(DataFrame_ColumnInt64* self)
{
    return SELF->na.data;
}

static DataFrame_ColumnInt64Methods Int64Methods =
{
/* shared */
    DataFrame_Type_ColumnInt64,
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


DataFrame_ColumnInt64* DataFrame_ColumnInt64_New()
{
    DataFrame_ColumnInt64Impl* c;

    c = calloc(1, sizeof(DataFrame_ColumnInt64Impl));
    if(!c) return NULL;
    c->methods = &Int64Methods;
    c->ref_count = 1;
    return (DataFrame_ColumnInt64*)c;
}