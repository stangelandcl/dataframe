#ifndef GUARD_0E442C72_2AC5_4E27_8ACF_1E1F557A5F25
#define GUARD_0E442C72_2AC5_4E27_8ACF_1E1F557A5F25

#include "dataframe/config.h"
#include "dataframe/column.h"

typedef struct DataFrame DataFrame;
typedef struct DataFrameMethods DataFrameMethods;
struct DataFrameMethods
{
    DataFrame_Type _typeID;
    void* (*Cast)(DataFrame* self, DataFrame_Type type);
    bool (*IncRef)(DataFrame* self);
    bool (*DecRef)(DataFrame* self);
    size_t (*RowSize)(DataFrame* self);
    size_t (*ColumnSize)(DataFrame* self);

    const char* (*AddColumn)(DataFrame* self, DataFrame_Column* column);
    void (*RemoveColumn)(DataFrame* self, size_t index);
    void (*RemoveRow)(DataFrame* self, size_t index);
    DataFrame_Column* (*GetColumnByIndex)(DataFrame* self, size_t index);
    bool (*IsRectangular)(DataFrame* self);
};

struct DataFrame
{
    DataFrameMethods* methods;
    int32_t ref_count;
    DataFrame_Column** columns;
    size_t size;
    size_t capacity;
};

DataFrame* DataFrame_New();



#endif