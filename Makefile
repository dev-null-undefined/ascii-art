.PHONY: all clean run compile release debug
.DEFAULT_GOAL = all
PROJECT := ascii-art

SOURCE_EXT = .cpp
HEADER_EXT = .h

INCLUDE_DIR = include
SOURCE_DIR = src
OUT_DIR = build
LIB_DIR = lib
BIN_DIR = bin
MAKE_INCLUDE = Makefile.d

CXX = g++
LD = g++

LIBS=

CXXFLAGS := -Wall -Wextra -pedantic --std=c++17 -I$(INCLUDE_DIR)

LDFLAGS := $(LIBS)

SOURCE_FILES = $(wildcard $(SOURCE_DIR)/*$(SOURCE_EXT) $(SOURCE_DIR)/*/*$(SOURCE_EXT) $(SOURCE_DIR)/*/*/*$(SOURCE_EXT) $(SOURCE_DIR)/*/*/*/*$(SOURCE_EXT))
OBJECTS = $(patsubst %$(SOURCE_EXT),$(OUT_DIR)/%.o,$(notdir $(SOURCE_FILES)))

all: release

debug: CXXFLAGS += -g -O0 -DDEBUG -fsanitize=address -pg -fno-omit-frame-pointer
debug: LDFLAGS += -g
debug: compile

release: CXXFLAGS += -O2
release: compile

compile: directories binaries

run: release
	./$(BIN_DIR)/$(PROJECT)


$(OUT_DIR)/%.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN_DIR)/$(PROJECT): $(OBJECTS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

binaries: $(BIN_DIR)/$(PROJECT)

clean:
	rm -rf $(MAKE_INCLUDE)
	rm -rf $(OUT_DIR)
	rm -rf $(BIN_DIR)

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

deps: $(MAKE_INCLUDE)/deps

$(MAKE_INCLUDE)/deps: $(MAKE_INCLUDE) $(SOURCE_DIR)
	printf "" > $(MAKE_INCLUDE)/deps
	$(foreach SOURCE_FILE,$(SOURCE_FILES),$(call execute-command,$(CXX) -MT $(OUT_DIR)/$(notdir $(basename $(SOURCE_FILE))).o -MM $(SOURCE_FILE) >> $(MAKE_INCLUDE)/deps))

include $(MAKE_INCLUDE)/deps