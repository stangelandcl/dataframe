#include "dataframe/columnInt16.h"
#include "dataframe/atomic.h"
#include <memory.h>

struct DataFrame_ColumnInt16
{
    DataFrame_ColumnInt16Methods* methods;
    volatile uint32_t ref_count;
    char* name;
    int16_t* data;
    size_t size;
    size_t capacity;
    DataFrame_BitVector na;
};

static void*
Cast(DataFrame_ColumnInt16* self, DataFrame_Type type)
{
    if(self->methods->_typeID == type)
        return self;
    return NULL;
}

static bool
IncRef(DataFrame_ColumnInt16* self)
{
    return InterlockedIncrement(&self->ref_count) > 0;
}

static bool
DecRef(DataFrame_ColumnInt16* self)
{
    DataFrame_ColumnInt16* s = (DataFrame_ColumnInt16*)self;

    if(!InterlockedDecrement(&s->ref_count))
    {
        free(s->data);
        DataFrame_BitVector_Destroy(&s->na);
	free(self);
        return true;
    }
    return false;
}

static size_t
Size(DataFrame_ColumnInt16* self)
{
    return self->size;
}

static void
Clear(DataFrame_ColumnInt16* self)
{
    self->size = 0;
    self->na.size = 0;
}

static bool
TryGet(
    DataFrame_ColumnInt16* self, size_t index, int16_t* v)
{
    bool have = DataFrame_BitVector_Get(&self->na, index);
    if(!have) return false;
    *v = self->data[index];
    return true;
}

static const char*
Resize(DataFrame_ColumnInt16* self)
{
    int16_t* d;
    size_t newSize;

    newSize = self->size * 2;
    if(!newSize) newSize = 4;

    d = realloc(self->data, newSize + sizeof(int16_t));
    if(!d) return "DataFrame_ColumnInt8_Add: Out of memory";
    self->data = d;
    self->capacity = newSize;

    return NULL;
}


static const char*
Add(DataFrame_ColumnInt16* self, int16_t v)
{
    size_t newSize;
    int16_t* d;
    const char* e = DataFrame_BitVector_Add(&self->na, false);
    if(e) return e;

    if(self->size == self->capacity)
    {
        e = Resize(self);
        if(e) return e;
    }


    self->data[self->size++] = v;
    return NULL;
}

static const char*
AddNA(DataFrame_ColumnInt16* self)
{
    size_t newSize;
    int16_t* d;
    const char* e = DataFrame_BitVector_Add(&self->na, true);
    if(e) return e;

    if(self->size == self->capacity)
    {
        e = Resize(self);
        if(e) return e;
    }

    self->size++;
    return NULL;
}

static void
Remove(DataFrame_ColumnInt16* self, size_t i)
{
    DataFrame_BitVector_Remove(&self->na, i);
    memmove(&self->data[i], &self->data[i+1], self->size - i - 1);
}

static void
Set(DataFrame_ColumnInt16* self, size_t i, int16_t v)
{
    DataFrame_BitVector_Set(&self->na, i, false);
    self->data[i] = v;
}

static void
SetNA(DataFrame_ColumnInt16* self, size_t i)
{
    DataFrame_BitVector_Set(&self->na, i, true);
}

static char*
GetName(DataFrame_ColumnInt16* self)
{
    return self->name;
}

static const char*
SetName(DataFrame_ColumnInt16* self, const char* name)
{
    const char* e;
    char* n;

    if(!name)
    {
        self->name = NULL;
        return NULL;
    }

    n = strdup(name);
    if(!n) return "DataFrame_ColumnInt16_SetName: Out of memory";

    self->name = n;
    return NULL;
}


static DataFrame_ColumnInt16Methods Int16Methods =
{
/* shared */
    DataFrame_Type_ColumnInt16,
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


DataFrame_ColumnInt16* DataFrame_ColumnInt16_New()
{
    DataFrame_ColumnInt16* c;

    c = calloc(1, sizeof(DataFrame_ColumnInt16));
    if(!c) return NULL;
    c->methods = &Int16Methods;
    c->ref_count = 1;
    return c;
}