.PHONY: build lib clean distclean check info

# ==============================
# Config
# ==============================

BUILD_DIR := build

# FEIG SDK root (override if needed)
FEIG_SDK_ROOT := .

FEIG_INCLUDE_DIR := $(FEIG_SDK_ROOT)/x64/include
FEIG_LIB_DIR := $(FEIG_SDK_ROOT)/x64/bin/release

CXX := g++
PYTHON := python3

# Python / pybind11 includes
PY_INCLUDES := $(shell $(PYTHON) -m pybind11 --includes)
PY_LDFLAGS := $(shell $(PYTHON) -m pybind11 --ldflags)

# ==============================
# Compiler flags
# ==============================

CXXFLAGS := -std=c++11 -Wall -Wextra -O2 -fPIC \
	$(PY_INCLUDES) \
	-Iinclude \
	-I$(FEIG_INCLUDE_DIR)

# ==============================
# Linker flags
# ==============================

LDFLAGS := -L$(FEIG_LIB_DIR)

LIBS := -lfedm -lfecom -lfeisp -lusb-1.0 -lpthread -ldl

# IMPORTANT: runtime library path
RPATH := -Wl,--disable-new-dtags -Wl,-rpath,'$$ORIGIN/libs'
# RPATH := -Wl,--disable-new-dtags -Wl,-rpath,'$$ORIGIN/../x64/bin/release'
# RPATH := -Wl,-rpath,'$(FEIG_LIB_DIR)'

# ==============================
# Sources
# ==============================

SOURCES := \
	src/bindings.cpp \
	src/reader_manager.cpp \
	src/reader_api.cpp

# Python extension output
# TARGET := $(BUILD_DIR)/feig_reader$(shell python3-config --extension-suffix)
PY_EXT := $(shell python3-config --extension-suffix)
TARGET := $(BUILD_DIR)/feig_reader$(PY_EXT)


# ==============================
# Targets
# ==============================

build: lib

lib: $(BUILD_DIR)
	@echo "Building Python extension (.so)..."
	$(CXX) $(CXXFLAGS) -shared $(SOURCES) -o $(TARGET) \
	$(LDFLAGS) $(LIBS) $(PY_LDFLAGS) $(RPATH)
	@echo "✓ Built: $(TARGET)"
	rm -f ./python/feig_reader/feig_reader.so
	cp -f $(TARGET) ./python/feig_reader/feig_reader.so
	@echo "✓ Build Copy to python/feig_reader/feig_reader.so"
	

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@echo "Created build directory"

clean:
	@echo "Cleaning..."
	rm -f $(TARGET)

distclean:
	@echo "Removing build directory..."
	rm -rf $(BUILD_DIR)

check:
	@echo "Checking environment..."
	@command -v $(CXX) >/dev/null || { echo "✗ g++ not found"; exit 1; }
	@command -v $(PYTHON) >/dev/null || { echo "✗ python3 not found"; exit 1; }
	@$(PYTHON) -c "import pybind11" 2>/dev/null || { echo "✗ pybind11 not installed"; exit 1; }
	@[ -d "$(FEIG_INCLUDE_DIR)" ] || { echo "✗ FEIG include missing"; exit 1; }
	@[ -d "$(FEIG_LIB_DIR)" ] || { echo "✗ FEIG lib missing"; exit 1; }
	@echo "✓ Environment OK"

info:
	@echo "FEIG_INCLUDE_DIR=$(FEIG_INCLUDE_DIR)"
	@echo "FEIG_LIB_DIR=$(FEIG_LIB_DIR)"
	@echo "TARGET=$(TARGET)"