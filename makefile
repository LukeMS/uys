#based on: scottmcpeak.com/autodepend/autodepend.html
#if depencies are renamed a make clean will be required

_MAKEFILE_ABS = $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
_TARGET := $(basename $(notdir $(realpath $(lastword $(_MAKEFILE_ABS)))))
_CWD = $(_MAKEFILE_ABS)
TESTS = $(sort $(dir $(wildcard $(CURDIR)/tests/*/)))
.DEFAULT_GOAL = lib
#$(info target->$(_TARGET) makefile_abs->$(_MAKEFILE_ABS) cwd->$(_MAKEFILE_ABS))
#$(info TESTS->$(TESTS))
#$(info .VARIABLES->[$(.VARIABLES)]) #show all variables
#$(info CURDIR->[$(CURDIR)])


IDIR = include
INC = -I$(IDIR)
ODIR = obj
SDIR = src
CFLAGS = -DUYS_TEST_MALLOC=1 -Wall -W -ggdb -std=c99 $(INC)


ifeq ($(DEST),)
DEST := lib
endif


# https://www.gnu.org/software/make/manual/html_node/Wildcard-Function.html
SRCS := $(wildcard $(SDIR)/*.c)
OBJS := $(patsubst $(SDIR)/%,$(ODIR)/%,$(patsubst %.c,%.o,$(SRCS)))
DEPS := $(patsubst %.o,%.d,$(OBJS))
$(info CURDIR=$(CURDIR), target=lib$(_TARGET).a, SRCS=[$(SRCS)], OBJS=[$(OBJS)], DEPS=[$(DEPS)])


clean_filenames := $(ODIR)/*.o $(ODIR)/*.d $(DEST)/lib$(_TARGET).a
clean_files := $(strip $(foreach f,$(clean_filenames),$(wildcard $(f))))

_dummy := $(shell mkdir -p "$(ODIR)" "$(DEST)")


# link
lib: $(OBJS)
	ar -cvqs $(DEST)/lib$(_TARGET).a $(OBJS)

# pull in dependency info for *existing* .o files
-include $(OBJS:.o=.d)

# compile and generate dependency info
$(ODIR)/%.o: $(SDIR)/%.c
	gcc -c $(CFLAGS) $(SDIR)/$*.c -o $(ODIR)/$*.o
	gcc -MM $(CFLAGS) $(SDIR)/$*.c > $(ODIR)/$*.d
	( echo -n $(ODIR)/ ; cat $(ODIR)/$*.d ) | tee $(ODIR)/$*.d


# remove compilation products
clean:
ifneq ($(clean_files),)
	rm -f $(clean_files)
endif