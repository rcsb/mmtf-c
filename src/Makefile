LD = $(CC)
LDFLAGS += -lmsgpackc
EXE_EXT ?= .exe
TEST_PROGRAMS = $(addsuffix $(EXE_EXT), multi demo traverse)

all: $(TEST_PROGRAMS)

clean:
	$(RM) *.o $(TEST_PROGRAMS)

test: run.demo

run.%: %$(EXE_EXT)
	./$< data/1MSH.mmtf

%$(EXE_EXT): mmtf_parser.o %.o
	$(LD) -o $@ $^ $(LDFLAGS)

.PHONY: test
