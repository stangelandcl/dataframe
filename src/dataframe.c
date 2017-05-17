#include "dataframe/dataframe.h"
#include <memory.h>


static void*
Cast(DataFrame* self, DataFrame_Type type)
{
    if(self->methods->_typeID == type)
        return self;
    return NULL;
}

static bool
IncRef(DataFrame* self)
{
    return ++self->ref_count > 0;
}

static bool
DecRef(DataFrame* self)
{
    size_t i;
    if(--self->ref_count == 0)
    {
        for(i=0;i<self->size;i++)
        {
            self->columns[i]->methods->DecRef(self->columns[i]);
            free(self->columns[i]);
        }
        free(self->columns);
        return true;
    }
    return false;
}

static size_t RowSize(DataFrame* self)
{
    DataFrame_Column* c;
    if(!self->size) return 0;
    c = self->columns[0];
    return c->methods->GetSize(c);
}

static size_t ColumnSize(DataFrame* self)
{
    return self->size;
}

static bool IsRectangular(DataFrame* self)
{
    size_t i;
    DataFrame_Column* c;
    size_t sz;

    if(!self->size) return true;

    c = self->columns[0];
    sz = c->methods->GetSize(c);

    for(i=1;i<self->size;i++)
    {
        c = self->columns[i];
        if(c->methods->GetSize(c) != sz)
            return false;
    }

    return true;
}

static const char* AddColumn(DataFrame* self, DataFrame_Column* c)
{
    DataFrame_Column** d;
    size_t newSize;
    if(self->size == self->capacity)
    {
        newSize = self->size * 2;
        if(!newSize) newSize = 4;

        d = realloc(self->columns, newSize * sizeof(DataFrame_Column*));
        if(!d) return "DataFrame_AddColumn: Out of memory";

        self->columns = d;
        self->size = newSize;
    }

    self->columns[self->size++] = c;
    return NULL;
}

static void RemoveColumn(DataFrame* self, size_t index)
{
    memmove(&self->columns[index],
            &self->columns[index+1],
            self->size - index - 1);
}

static void RemoveRow(DataFrame* self, size_t index)
{
    DataFrame_Column* c;
    size_t i;

    for(i=0;i<self->size;i++)
    {
        c = self->columns[i];
        c->methods->Remove(c, index);
    }
}


static DataFrame_Column* GetColumnByIndex(DataFrame* self, size_t index)
{
    DataFrame_Column* n =  self->columns[index];
    /* n->methods->IncRef(n); */
    return n;
}

static DataFrameMethods methods =
{
    DataFrame_Type_DataFrame,
    Cast,
    IncRef,
    DecRef,
    RowSize,
    ColumnSize,

    AddColumn,
    RemoveColumn,
    RemoveRow,
    GetColumnByIndex,
    IsRectangular
};

DataFrame* DataFrame_New()
{
    DataFrame* f = calloc(1, sizeof(DataFrame));
    if(!f) return NULL;

    f->methods = &methods;
    f->ref_count = 1;
    return f;
}
