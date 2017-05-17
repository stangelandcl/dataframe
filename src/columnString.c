#include "dataframe/columnString.h"
#include <memory.h>
#include <string.h>

#ifdef _MSC_VER
    #define strdup _strdup
#endif

struct DataFrame_ColumnCString
{
    DataFrame_ColumnCStringMethods* methods;
    uint32_t ref_count;
    char* name;
    char** data;
    size_t size;
    size_t capacity;
    DataFrame_BitVector na;
};

static void*
Cast(DataFrame_ColumnCString* self, DataFrame_Type type)
{
    if(self->methods->_typeID == type)
        return self;
    return NULL;
}

static bool
IncRef(DataFrame_ColumnCString* self)
{
    return ++self->ref_count > 0;
}

static bool
DecRef(DataFrame_ColumnCString* self)
{
    size_t i;
    DataFrame_ColumnCString* s = (DataFrame_ColumnCString*)self;

    if(--s->ref_count == 0)
    {
        for(i=0;i<s->size;i++)
            free(s->data[i]);

        free(s->data);
        DataFrame_BitVector_Destroy(&s->na);
        return true;
    }
    return false;
}

static size_t
Size(DataFrame_ColumnCString* self)
{
    return self->size;
}

static void
Clear(DataFrame_ColumnCString* self)
{
    self->size = 0;
    self->na.size = 0;
}

static bool
TryGet(
    DataFrame_ColumnCString* self, size_t index, char** v)
{
    bool have = DataFrame_BitVector_Get(&self->na, index);
    if(!have) return false;
    *v = self->data[index];
    return true;
}

static const char*
Resize(DataFrame_ColumnCString* self)
{
    char** d;
    size_t newSize;

    newSize = self->size * 2;
    if(!newSize) newSize = 4;

    d = realloc(self->data, newSize + sizeof(char*));
    if(!d) return "DataFrame_ColumnInt8_Add: Out of memory";
    self->data = d;
    self->capacity = newSize;

    return NULL;
}


static const char*
Add(DataFrame_ColumnCString* self, const char* v)
{
    size_t newSize;
    char** d;
    char* v2;
    bool na = v ? false : true;

    const char* e = DataFrame_BitVector_Add(&self->na, na);
    if(e) return e;

    if(self->size == self->capacity)
    {
        e = Resize(self);
        if(e) return e;
    }

    if(na)
        self->data[self->size++] = NULL;
    else
    {
        v2 = strdup(v);
        if(!v2)
            return "DataFrame_ColumnCString_Add: Out of memory";
        self->data[self->size++] = v2;
    }
    return NULL;
}

static const char*
AddNA(DataFrame_ColumnCString* self)
{
    size_t newSize;
    char** d;
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
Remove(DataFrame_ColumnCString* self, size_t i)
{
    DataFrame_BitVector_Remove(&self->na, i);
    memmove(&self->data[i], &self->data[i+1], self->size - i - 1);
}

static void
Set(DataFrame_ColumnCString* self, size_t i, const char* v)
{
    bool na = v ? false : true;
    if(na)
    {
        DataFrame_BitVector_Set(&self->na, i, na);
        self->data[i] = NULL;
    }
    else
    {
        self->data[i] = strdup(v);
        DataFrame_BitVector_Set(&self->na, i, !na && self->data[i]);
    }
}

static void
SetNA(DataFrame_ColumnCString* self, size_t i)
{
    DataFrame_BitVector_Set(&self->na, i, true);
}

static char*
GetName(DataFrame_ColumnCString* self)
{
    return self->name;
}

static const char*
SetName(DataFrame_ColumnCString* self, const char* name)
{
    const char* e;
    char* n;

    if(!name)
    {
        self->name = NULL;
        return NULL;
    }

    n = strdup(name);
    if(!n) return "DataFrame_ColumnCString_SetName: Out of memory";

    self->name = n;
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
    DataFrame_ColumnCString* c;

    c = calloc(1, sizeof(DataFrame_ColumnCString));
    if(!c) return NULL;
    c->methods = &CStringMethods;
    c->ref_count = 1;
    return c;
}
