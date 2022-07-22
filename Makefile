CXX = em++

# path #
SRC_PATH = CSHackCreator-2
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/web

# executable # 
BIN_NAME = index.html

# extensions #
SRC_EXT = cpp

# code lists #
# Find all source files in the source directory, sorted by
# most recently modified
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)

# flags #
EMS = -s USE_SDL=2
EMS += -s DISABLE_EXCEPTION_CATCHING=1
COMPILE_FLAGS = -Wall -Os
LINKER_FLAGS += -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1

# Emscripten allows preloading a file or folder to be accessible at runtime.
# The Makefile for this example project suggests embedding the misc/fonts/ folder into our application, it will then be accessible as "/fonts"
# See documentation for more details: https://emscripten.org/docs/porting/files/packaging_files.html
# (Default value is 0. Set to 1 to enable file-system and include the misc/fonts/ folder as part of the build.)
USE_FILE_SYSTEM = 0
ifeq ($(USE_FILE_SYSTEM), 0)
LINKER_FLAGS += -s NO_FILESYSTEM=1
COMPILE_FLAGS += -DIMGUI_DISABLE_FILE_FUNCTIONS
endif
ifeq ($(USE_FILE_SYSTEM), 1)
LINKER_FLAGS += -s FORCE_FILESYSTEM=1
LINKER_FLAGS += --no-heap-copy --preload-file $(SRC_PATH)/fonts@/fonts
endif

COMPILE_FLAGS += $(EMS)
LINKER_FLAGS += $(EMS)
INCLUDES = -I include/ -I /usr/local/include
# Space-separated pkg-config libraries used by this project
LIBS =

.PHONY: default_target
default_target: release

.PHONY: release
release: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS)
release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) $(BIN_NAME)
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
#	@$(RM) -r $(BIN_PATH)

# checks the executable and symlinks to the output
.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
#	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

# Creation of the executable
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	$(CXX) $(OBJECTS) $(LINKER_FLAGS) --shell-file $(SRC_PATH)/shell_minimal.html -o $@ ${LIBS}

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
#	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@
	$(CXX) $(CXXFLAGS) -c $< -o $@
