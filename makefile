#based on: scottmcpeak.com/autodepend/autodepend.html
#if depencies are renamed a make clean will be required

_MAKEFILE_ABS = $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
_TARGET := $(basename $(notdir $(realpath $(lastword $(_MAKEFILE_ABS)))))
_CWD = $(_MAKEFILE_ABS)
.DEFAULT_GOAL = lib
$(info target->$(_TARGET) makefile_abs->$(_MAKEFILE_ABS) cwd->$(_MAKEFILE_ABS))


IDIR = include
INC = -I$(IDIR)
ODIR = obj
SDIR = src
CFLAGS = -Wall -W -ggdb -std=c99 $(INC)


ifeq ($(DEST),)
DEST := lib
endif


# https://www.gnu.org/software/make/manual/html_node/Wildcard-Function.html
SRCS := $(wildcard $(SDIR)/*.c)
OBJS := $(patsubst $(SDIR)/%,$(ODIR)/%,$(patsubst %.c,%.o,$(SRCS)))
$(info target=lib$(_TARGET).a, SRCS=[$(SRCS)], OBJS=[$(OBJS)])


clean_filenames := $(ODIR)/*.o $(ODIR)/*.d $(DEST)/lib$(_TARGET).a
clean_files := $(strip $(foreach f,$(clean_filenames),$(wildcard $(f))))

_dummy := $(shell mkdir -p "$(ODIR)" "$(DEST)")

# link
lib: $(OBJS) $(DEST)/lib$(_TARGET).a

$(DEST)/lib$(_TARGET).a:
	ar -cvqs $(DEST)/lib$(_TARGET).a $(OBJS)

# pull in dependency info for *existing* .o files
-include $(OBJS:.o=.d)

# compile and generate dependency info
$(ODIR)/%.o: $(SDIR)/%.c $(SDIR)/%.c
	gcc -c $(CFLAGS) $(SDIR)/$*.c -o $(ODIR)/$*.o
	gcc -MM $(CFLAGS) $(SDIR)/$*.c > $(ODIR)/$*.d


# remove compilation products
clean:
ifneq ($(clean_files),)
	rm -f $(clean_files)
endif