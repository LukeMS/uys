.DEFAULT_GOAL = run

_MAKEFILE_ABS = $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
_TARGET := $(basename $(notdir $(realpath $(lastword $(CURDIR)))))


BDIR = .
IDIR = ../../include
INC = -I$(IDIR)
LDIR = ../../lib
LIB = -L$(LDIR)
LIBS = $(LDIR)/libuys.a
LIBS_SHORT = -luys
ODIR = obj
SDIR = .
CFLAGS = -DUYS_TEST_MALLOC=1 -static -Wall -W -ggdb -std=c99 $(INC)


# https://www.gnu.org/software/make/manual/html_node/Wildcard-Function.html
SRCS := $(wildcard $(SDIR)/*.c)
OBJS := $(patsubst $(SDIR)/%,$(ODIR)/%,$(patsubst %.c,%.o,$(SRCS)))


clean_filenames := $(ODIR)/*.o $(ODIR)/*.d $(DEST)/lib$(_TARGET).a
clean_files := $(strip $(foreach f,$(clean_filenames),$(wildcard $(f))))


_dummy := $(shell mkdir -p "$(ODIR)" "$(BDIR)")


$(info target=$(_TARGET), SRCS=[$(SRCS)], OBJS=[$(OBJS)])

all: $(OBJS)
	gcc $(OBJS) $(LIB) $(LIBS_SHORT) -o $(BDIR)/$(_TARGET)


run: all
	$(BDIR)/$(_TARGET).exe  || gdb -batch -ex "run" -ex "bt full" $(BDIR)/$(_TARGET).exe 2>&1


rundrm: all
	C:/dr_m/bin/drmemory -batch -- ./$(BDIR)/$(_TARGET).exe


# pull in dependency info for *existing* .o files
-include $(OBJS:.o=.d)


# compile and generate dependency info
$(ODIR)/%.o: $(SDIR)/%.c
	make -C ../.. lib
	gcc -c $(CFLAGS) $(SDIR)/$*.c -o $(ODIR)/$*.o
	( echo -n $(ODIR)/ ; echo -n `gcc -MM $(CFLAGS) $(SDIR)/$*.c` ; echo -n " " ; echo -n $(LIBS) ) | tee $(ODIR)/$*.d


# remove compilation products
clean:
ifneq ($(clean_files),)
	rm -f $(clean_files)
endif