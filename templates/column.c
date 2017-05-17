#include "dataframe/column{{name}}.h"
#include <memory.h>

struct DataFrame_Column{{name}}
{
    DataFrame_Column{{name}}Methods* methods;
    uint32_t ref_count;
    char* name;
    {{type}}* data;
    size_t size;
    size_t capacity;
    DataFrame_BitVector na;
};

static void*
Cast(DataFrame_Column{{name}}* self, DataFrame_Type type)
{
    if(self->methods->_typeID == type)
        return self;
    return NULL;
}

static bool
IncRef(DataFrame_Column{{name}}* self)
{
    return ++self->ref_count > 0;
}

static bool
DecRef(DataFrame_Column{{name}}* self)
{
    DataFrame_Column{{name}}* s = (DataFrame_Column{{name}}*)self;

    if(--s->ref_count == 0)
    {
        free(s->data);
        DataFrame_BitVector_Destroy(&s->na);
	free(self);
        return true;
    }
    return false;
}

static size_t
Size(DataFrame_Column{{name}}* self)
{
    return self->size;
}

static void
Clear(DataFrame_Column{{name}}* self)
{
    self->size = 0;
    self->na.size = 0;
}

static bool
TryGet(
    DataFrame_Column{{name}}* self, size_t index, {{type}}* v)
{
    bool have = DataFrame_BitVector_Get(&self->na, index);
    if(!have) return false;
    *v = self->data[index];
    return true;
}

static const char*
Resize(DataFrame_Column{{name}}* self)
{
    {{type}}* d;
    size_t newSize;

    newSize = self->size * 2;
    if(!newSize) newSize = 4;

    d = realloc(self->data, newSize + sizeof({{type}}));
    if(!d) return "DataFrame_ColumnInt8_Add: Out of memory";
    self->data = d;
    self->capacity = newSize;

    return NULL;
}


static const char*
Add(DataFrame_Column{{name}}* self, {{type}} v)
{
    size_t newSize;
    {{type}}* d;
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
AddNA(DataFrame_Column{{name}}* self)
{
    size_t newSize;
    {{type}}* d;
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
Remove(DataFrame_Column{{name}}* self, size_t i)
{
    DataFrame_BitVector_Remove(&self->na, i);
    memmove(&self->data[i], &self->data[i+1], self->size - i - 1);
}

static void
Set(DataFrame_Column{{name}}* self, size_t i, {{type}} v)
{
    DataFrame_BitVector_Set(&self->na, i, false);
    self->data[i] = v;
}

static void
SetNA(DataFrame_Column{{name}}* self, size_t i)
{
    DataFrame_BitVector_Set(&self->na, i, true);
}

static char*
GetName(DataFrame_Column{{name}}* self)
{
    return self->name;
}

static const char*
SetName(DataFrame_Column{{name}}* self, const char* name)
{
    const char* e;
    char* n;

    if(!name)
    {
        self->name = NULL;
        return NULL;
    }

    n = strdup(name);
    if(!n) return "DataFrame_Column{{name}}_SetName: Out of memory";

    self->name = n;
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
    DataFrame_Column{{name}}* c;

    c = calloc(1, sizeof(DataFrame_Column{{name}}));
    if(!c) return NULL;
    c->methods = &{{name}}Methods;
    c->ref_count = 1;
    return c;
}
