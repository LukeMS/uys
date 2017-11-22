uys = libuys.so


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
$(info target=lib$(_TARGET).a, SRCS=[$(SRCS)], OBJS=[$(OBJS)])


clean_filenames := $(ODIR)/*.o $(ODIR)/*.d $(DEST)/lib$(_TARGET).a
clean_files := $(strip $(foreach f,$(clean_filenames),$(wildcard $(f))))
_dummy := $(shell mkdir -p "$(ODIR)" "$(DEST)")


$(uys): $(OBJS)
	$(AR) rcs $@ $^


.PHONY: install
install: $(uys)
	mkdir -p /usr/lib
	mkdir -p /usr/include/uys/
	cp $(uys) /usr/lib/$(uys)
	cp $(IDIR)/*.h /usr/include/uys/


.PHONY: uninstall
uninstall:
	rm -f /usr/lib/$(uys)
	rm -f /usr/include/uys/*.h


gcov:
	gcc -shared $(LDIR) $(CFLAGS) -fPIC -O0 -fprofile-arcs -ftest-coverage $(SDIR)/*.c -lgcov -o /usr/lib/$(uys)

#All test folders as dependencies
test: $(TESTS)

#In a multiple target pattern rule ‘$@’ is the name of whichever target caused the rule’s recipe to be run.
#Run once for each test folder passing its path to the recursive make
$(TESTS):
	make -C $@ --file=$(CURDIR)/linux_test.makefile valgrind


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