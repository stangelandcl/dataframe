CC?=ccache gcc
ifdef DEBUG
OPT:=-O0 #-DDEBUG
else
OPT:=-O3 -fomit-frame-pointer -mtune=native -DNDEBUG
endif

ERRORS:= -Wall -Werror -Wno-unused-but-set-variable -Wno-unused-function -Wno-unused-variable -pedantic -Wc90-c99-compat -Wc99-c11-compat
CFLAGS+=-ggdb3 -Iinclude $(ERRORS) $(OPT)
#CC=clang
#OPT=-O3
#ERRORS=
LIBNAME=dataframe
STATIC_LIB=lib$(LIBNAME).a
SHARED_LIB=lib$(LIBNAME).so

SOURCES=\
	src/bitvector.c \
	src/columnInt8.c \
	src/columnInt16.c \
	src/columnInt32.c \
	src/columnInt64.c \
	src/columnUInt8.c \
	src/columnUInt16.c \
	src/columnUInt32.c \
	src/columnUInt64.c \
	src/columnFloat32.c \
	src/columnFloat64.c \
	src/columnCString.c \
	src/dataframe.c
TEST_SOURCES=test/test.c
STATIC_OBJDIR=obj/static
SHARED_OBJDIR=obj/shared
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
SHARED_OBJECTS=$(addprefix $(SHARED_OBJDIR)/, $(OBJECTS))
STATIC_OBJECTS=$(addprefix $(STATIC_OBJDIR)/, $(OBJECTS))
TEST=check

.PHONY: all compile check

compile: generate
	@mkdir -p obj/shared obj/static
	@echo "make $(STATIC_LIB)"
	@make --no-print-directory $(STATIC_LIB)
	@echo "make $(SHARED_LIB)"
	@make --no-print-directory $(SHARED_LIB)

generate:
	cd templates && python3 generate.py

$(SHARED_OBJDIR)/%.o:	%.c
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo "CC -fPIC $<"

$(STATIC_OBJDIR)/%.o:	%.c
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo "CC $<"

$(STATIC_LIB):
$(STATIC_LIB): $(STATIC_OBJECTS)
	@ar rcs $@ $(STATIC_OBJECTS)
	@echo "AR $(STATIC_LIB)"

$(SHARED_LIB): CFLAGS += -fPIC
$(SHARED_LIB): $(SHARED_OBJECTS)
	@$(CC) $(LDFLAGS) $(SHARED_OBJECTS) -shared -o $@
	@echo "LD $(SHARED_LIB)"

check: $(STATIC_LIB)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(TEST_SOURCES) -o $(TEST) -L. -Wl,-Bstatic -l$(LIBNAME) -Wl,-Bdynamic
	./check

clean:
	@rm -rf obj $(OBJECTS) $(STATIC_LIB) $(SHARED_LIB) $(TEST)

all: 
	@make --no-print-directory clean
	@make --no-print-directory compile
	@make --no-print-directory check
