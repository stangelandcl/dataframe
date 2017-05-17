#include "dataframe/columnInt32.h"
#include "dataframe/atomic.h"
#include <memory.h>

struct DataFrame_ColumnInt32
{
    DataFrame_ColumnInt32Methods* methods;
    volatile uint32_t ref_count;
    char* name;
    int32_t* data;
    size_t size;
    size_t capacity;
    DataFrame_BitVector na;
};

static void*
Cast(DataFrame_ColumnInt32* self, DataFrame_Type type)
{
    if(self->methods->_typeID == type)
        return self;
    return NULL;
}

static bool
IncRef(DataFrame_ColumnInt32* self)
{
    return InterlockedIncrement(&self->ref_count) > 0;
}

static bool
DecRef(DataFrame_ColumnInt32* self)
{
    DataFrame_ColumnInt32* s = (DataFrame_ColumnInt32*)self;

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
Size(DataFrame_ColumnInt32* self)
{
    return self->size;
}

static void
Clear(DataFrame_ColumnInt32* self)
{
    self->size = 0;
    self->na.size = 0;
}

static bool
TryGet(
    DataFrame_ColumnInt32* self, size_t index, int32_t* v)
{
    bool have = DataFrame_BitVector_Get(&self->na, index);
    if(!have) return false;
    *v = self->data[index];
    return true;
}

static const char*
Resize(DataFrame_ColumnInt32* self)
{
    int32_t* d;
    size_t newSize;

    newSize = self->size * 2;
    if(!newSize) newSize = 4;

    d = realloc(self->data, newSize + sizeof(int32_t));
    if(!d) return "DataFrame_ColumnInt8_Add: Out of memory";
    self->data = d;
    self->capacity = newSize;

    return NULL;
}


static const char*
Add(DataFrame_ColumnInt32* self, int32_t v)
{
    size_t newSize;
    int32_t* d;
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
AddNA(DataFrame_ColumnInt32* self)
{
    size_t newSize;
    int32_t* d;
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
Remove(DataFrame_ColumnInt32* self, size_t i)
{
    DataFrame_BitVector_Remove(&self->na, i);
    memmove(&self->data[i], &self->data[i+1], self->size - i - 1);
}

static void
Set(DataFrame_ColumnInt32* self, size_t i, int32_t v)
{
    DataFrame_BitVector_Set(&self->na, i, false);
    self->data[i] = v;
}

static void
SetNA(DataFrame_ColumnInt32* self, size_t i)
{
    DataFrame_BitVector_Set(&self->na, i, true);
}

static char*
GetName(DataFrame_ColumnInt32* self)
{
    return self->name;
}

static const char*
SetName(DataFrame_ColumnInt32* self, const char* name)
{
    const char* e;
    char* n;

    if(!name)
    {
        self->name = NULL;
        return NULL;
    }

    n = strdup(name);
    if(!n) return "DataFrame_ColumnInt32_SetName: Out of memory";

    self->name = n;
    return NULL;
}


static DataFrame_ColumnInt32Methods Int32Methods =
{
/* shared */
    DataFrame_Type_ColumnInt32,
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


DataFrame_ColumnInt32* DataFrame_ColumnInt32_New()
{
    DataFrame_ColumnInt32* c;

    c = calloc(1, sizeof(DataFrame_ColumnInt32));
    if(!c) return NULL;
    c->methods = &Int32Methods;
    c->ref_count = 1;
    return c;
}