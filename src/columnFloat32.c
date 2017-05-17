#include "dataframe/columnFloat32.h"
#include <memory.h>

struct DataFrame_ColumnFloat32
{
    DataFrame_ColumnFloat32Methods* methods;
    uint32_t ref_count;
    char* name;
    float* data;
    size_t size;
    size_t capacity;
    DataFrame_BitVector na;
};

static void*
Cast(DataFrame_ColumnFloat32* self, DataFrame_Type type)
{
    if(self->methods->_typeID == type)
        return self;
    return NULL;
}

static bool
IncRef(DataFrame_ColumnFloat32* self)
{
    return ++self->ref_count > 0;
}

static bool
DecRef(DataFrame_ColumnFloat32* self)
{
    DataFrame_ColumnFloat32* s = (DataFrame_ColumnFloat32*)self;

    if(--s->ref_count == 0)
    {
        free(s->data);
        DataFrame_BitVector_Destroy(&s->na);
        return true;
    }
    return false;
}

static size_t
Size(DataFrame_ColumnFloat32* self)
{
    return self->size;
}

static void
Clear(DataFrame_ColumnFloat32* self)
{
    self->size = 0;
    self->na.size = 0;
}

static bool
TryGet(
    DataFrame_ColumnFloat32* self, size_t index, float* v)
{
    bool have = DataFrame_BitVector_Get(&self->na, index);
    if(!have) return false;
    *v = self->data[index];
    return true;
}

static const char*
Resize(DataFrame_ColumnFloat32* self)
{
    float* d;
    size_t newSize;

    newSize = self->size * 2;
    if(!newSize) newSize = 4;

    d = realloc(self->data, newSize + sizeof(float));
    if(!d) return "DataFrame_ColumnInt8_Add: Out of memory";
    self->data = d;
    self->capacity = newSize;

    return NULL;
}


static const char*
Add(DataFrame_ColumnFloat32* self, float v)
{
    size_t newSize;
    float* d;
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
AddNA(DataFrame_ColumnFloat32* self)
{
    size_t newSize;
    float* d;
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
Remove(DataFrame_ColumnFloat32* self, size_t i)
{
    DataFrame_BitVector_Remove(&self->na, i);
    memmove(&self->data[i], &self->data[i+1], self->size - i - 1);
}

static void
Set(DataFrame_ColumnFloat32* self, size_t i, float v)
{
    DataFrame_BitVector_Set(&self->na, i, false);
    self->data[i] = v;
}

static void
SetNA(DataFrame_ColumnFloat32* self, size_t i)
{
    DataFrame_BitVector_Set(&self->na, i, true);
}

static char*
GetName(DataFrame_ColumnFloat32* self)
{
    return self->name;
}

static const char*
SetName(DataFrame_ColumnFloat32* self, const char* name)
{
    const char* e;
    char* n;

    if(!name)
    {
        self->name = NULL;
        return NULL;
    }

    n = strdup(name);
    if(!n) return "DataFrame_ColumnFloat32_SetName: Out of memory";

    self->name = n;
    return NULL;
}


static DataFrame_ColumnFloat32Methods Float32Methods =
{
/* shared */
    DataFrame_Type_ColumnFloat32,
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


DataFrame_ColumnFloat32* DataFrame_ColumnFloat32_New()
{
    DataFrame_ColumnFloat32* c;

    c = calloc(1, sizeof(DataFrame_ColumnFloat32));
    if(!c) return NULL;
    c->methods = &Float32Methods;
    c->ref_count = 1;
    return c;
}