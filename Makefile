.PHONY: all clean run compile release debug fast fast-debug doc perf-compile fast-perf report benchmark directories test-compile fast-test test binaries
.DEFAULT_GOAL = all
PROJECT = ascii-art
VERSION = v0.1.2

SOURCE_EXT = .cpp
HEADER_EXT = .h

SOURCE_DIR = src
OUT_DIR = build
LIB_DIR = lib
BIN_DIR = bin
DOC_DIR = doc
MAKE_INCLUDE = Makefile.d

CXX = g++
LD = g++

LIBS = -ljpeg -lncurses -lncursesw -lpng -lz -lstdc++fs -lform

CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 $(INCLUDE_DIR)

LDFLAGS = $(LIBS) $(LIBPATH)

SOURCE_FILES = $(wildcard $(SOURCE_DIR)/*$(SOURCE_EXT) $(SOURCE_DIR)/*/*$(SOURCE_EXT) $(SOURCE_DIR)/*/*/*$(SOURCE_EXT) $(SOURCE_DIR)/*/*/*/*$(SOURCE_EXT) $(SOURCE_DIR)/*/*/*/*/*$(SOURCE_EXT))
OBJECTS = $(patsubst %$(SOURCE_EXT),$(OUT_DIR)/%.o,$(notdir $(SOURCE_FILES)))

fast:
	$(MAKE) -j12 release

fast-debug:
	$(MAKE) -j12 debug

fast-perf:
	$(MAKE) -j12 perf-compile

fast-test:
	$(MAKE) -j12 test-compile

all: release doc

doc: Doxyfile
	doxygen Doxyfile

debug: CXXFLAGS += -g -pg -O0 -DDEBUG -DLOG_LEVEL=0 -fPIE -fno-omit-frame-pointer # -fsanitize=address
debug: LDFLAGS +=
debug: compile

perf-compile: CXXFLAGS += -g -pg -O2 -fno-omit-frame-pointer
perf-compile: LDFLAGS +=
perf-compile: compile

release: CXXFLAGS += -O2
release: compile

test-compile: CXXFLAGS += -rdynamic -D __TESTING__ -fsanitize=address
test-compile: compile

compile: directories binaries

ifeq (run,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

run: fast
	./$(BIN_DIR)/$(PROJECT) $(RUN_ARGS)

test: fast-test
	./$(BIN_DIR)/$(PROJECT)

benchmark: fast-perf
	perf record -g ./$(BIN_DIR)/$(PROJECT) ./examples


$(OUT_DIR)/%.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN_DIR)/$(PROJECT): $(OBJECTS)
	$(LD) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

binaries: $(BIN_DIR)/$(PROJECT)

clean:
	rm -rf $(MAKE_INCLUDE)
	rm -rf $(OUT_DIR)
	rm -rf $(BIN_DIR)/$(PROJECT)
	rm -rf $(DOC_DIR)

directories: $(OUT_DIR) $(BIN_DIR) $(MAKE_INCLUDE)

$(MAKE_INCLUDE):
	mkdir -p $(MAKE_INCLUDE)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

define execute-command
$(1)

endef


$(MAKE_INCLUDE)/deps: $(MAKE_INCLUDE) $(SOURCE_DIR) $(SOURCE_FILES)
	: > $(MAKE_INCLUDE)/deps
	$(foreach SOURCE_FILE,$(SOURCE_FILES),$(call execute-command,$(CXX) -MT $(OUT_DIR)/$(notdir $(basename $(SOURCE_FILE))).o -MM $(SOURCE_FILE) >> $(MAKE_INCLUDE)/deps))

include $(MAKE_INCLUDE)/deps
