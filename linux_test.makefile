_TARGET := $(basename $(notdir $(realpath $(lastword $(CURDIR)))))
EXE = $(_TARGET).exe

$(info PATH->[$(CURDIR)])
$(info _TARGET->[$(_TARGET)])


BDIR = $(CURDIR)
ODIR = $(CURDIR)
SDIR = $(CURDIR)
IDIR = -I/usr/include/uys
LDIR = -L/usr/lib/
LIBS = -luys -lgcov
CFLAGS = -Wall -W -ggdb -std=c99 -fPIC -O0


SRCS := $(wildcard $(SDIR)/*.c)
OBJS := $(patsubst $(SDIR)/%,$(ODIR)/%,$(patsubst %.c,%.o,$(SRCS)))
DEPS := $(patsubst %.o,%.d,$(OBJS))


clean_filenames := $(BDIR)/$(EXE) $(OBJS) DEPS
clean_files := $(strip $(foreach f,$(clean_filenames),$(wildcard $(f))))

_dummy := $(shell mkdir -p "$(BDIR)" "$(ODIR)")

all:
	gcc $(IDIR) $(LDIR) $(CFLAGS) $(SRCS) $(LIBS) -o $(EXE)

run: all
	$(BDIR)/$(EXE)

valgrind: all
	valgrind --error-exitcode=666 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --track-fds=yes $(BDIR)/$(EXE) || gdb -batch -ex "run" -ex "bt full" --args $(BDIR)/$(EXE)

# remove compilation products
clean:
ifneq ($(clean_files),)
	rm -f $(clean_files)
endif