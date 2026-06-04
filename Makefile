.PHONY: all lib clean distclean check info

# ==============================
# Config & Platform Detection
# ==============================
BUILD_DIR := build

ifeq ($(OS),Windows_NT)
    PLATFORM := WINDOWS
    PYTHON := venv/Scripts/python.exe
    CXX := g++
else
    PLATFORM := LINUX
    PYTHON := venv/bin/python
    CXX := g++
endif

# FEIG SDK paths
FEIG_INCLUDE_DIR := ./include
FEIG_LIB_DIR := x64/bin/release

# Python / pybind11
# Use explicit path for python to avoid CreateProcess errors
PY_INCLUDES := $(shell "$(PYTHON)" -m pybind11 --includes)
PY_EXT := $(shell "$(PYTHON)" -c "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX') or '.pyd')")

# ==============================
# Compiler & Linker Flags
# ==============================
COMMON_FLAGS := -std=c++17 -Wall -Wextra -O2 $(PY_INCLUDES) -Iinclude

ifeq ($(PLATFORM),LINUX)
    CXXFLAGS := $(COMMON_FLAGS) -fPIC
    LDFLAGS := -L$(FEIG_LIB_DIR)
    LIBS := -lfedm -lfecom -lfeisp -lusb-1.0 -lpthread -ldl
    RPATH := -Wl,--disable-new-dtags -Wl,-rpath,'$$ORIGIN'
else
    # WINDOWS SPECIFIC FIX:
    # g++ cannot use -l flags for MSVC DLLs easily. 
    # We pass the full path to the DLLs directly to the linker.
    CXXFLAGS := $(COMMON_FLAGS)
    LDFLAGS := 
    # List every DLL explicitly. This works with MinGW g++.
    FEIG_DLLS := \
        $(FEIG_LIB_DIR)/fedm.dll \
        $(FEIG_LIB_DIR)/fecom.dll \
        $(FEIG_LIB_DIR)/feisp.dll \
        $(FEIG_LIB_DIR)/feble.dll \
        $(FEIG_LIB_DIR)/fetls.dll \
        $(FEIG_LIB_DIR)/feudp.dll \
        $(FEIG_LIB_DIR)/feusb.dll \
        $(FEIG_LIB_DIR)/fedm-funit.dll \
        $(FEIG_LIB_DIR)/fedm-service.dll
    
    LIBS := $(FEIG_DLLS) -lws2_32 -lsetupapi -lhid
    RPATH :=
endif

# ==============================
# Sources & Targets
# ==============================
SOURCES := src/bindings.cpp src/reader_manager.cpp
TARGET := $(BUILD_DIR)/feig_reader$(PY_EXT)

# ==============================
# Rules
# ==============================
all: lib

# Order-only prerequisite (|) ensures directory exists
lib: | $(BUILD_DIR)
	@echo "Building Python extension ($(PLATFORM))..."
	$(CXX) $(CXXFLAGS) -shared $(SOURCES) -o $(TARGET) $(LDFLAGS) $(LIBS) $(RPATH)
	@echo "✓ Built: $(TARGET)"

	@echo "Copying to Python package..."
	@$(PYTHON) -c "import shutil, os; d='python/feig_reader'; os.makedirs(d, exist_ok=True); shutil.copy2('$(TARGET)', d)"
	
ifeq ($(PLATFORM),WINDOWS)
	# Copy FEIG DLLs
	@$(PYTHON) -c "import shutil, os, glob; d='python/feig_reader'; [shutil.copy2(f, d) for f in glob.glob('$(FEIG_LIB_DIR)/*.dll')]"
	
	# Copy MSYS2/GCC Runtime DLLs (Required for g++ compiled extensions)
	@$(PYTHON) -c "import shutil, os, subprocess; d='python/feig_reader'; \
	import sys; \
	gcc_path = subprocess.check_output(['where', 'g++']).decode().strip().split('\n')[0]; \
	gcc_dir = os.path.dirname(gcc_path); \
	for dll in ['libstdc++-6.dll', 'libgcc_s_seh-1.dll', 'libwinpthread-1.dll']: \
	    src = os.path.join(gcc_dir, dll); \
	    os.path.exists(src) and shutil.copy2(src, d)"
	@echo "✓ Copied DLLs to python/feig_reader/"
else
	@echo "✓ Copied to python/feig_reader/"
endif

# Cross-platform directory creation
$(BUILD_DIR):
	@$(PYTHON) -c "import os; os.makedirs('$(BUILD_DIR)', exist_ok=True)"

clean:
	@echo "Cleaning build artifacts..."
	@$(PYTHON) -c "import os, glob; [os.remove(f) for f in glob.glob('$(BUILD_DIR)/*') if os.path.isfile(f)]"

distclean:
	@echo "Removing build directory..."
	@$(PYTHON) -c "import shutil; shutil.rmtree('$(BUILD_DIR)', ignore_errors=True)"

check:
	@echo "Checking environment..."
	@$(PYTHON) -c "import os,sys,shutil,importlib.util; f=[]; not shutil.which('$(CXX)') and f.append('$(CXX)'); importlib.util.find_spec('pybind11') is None and f.append('pybind11'); not os.path.isdir('$(FEIG_INCLUDE_DIR)') and f.append('FEIG_INCLUDE'); not os.path.isdir('$(FEIG_LIB_DIR)') and f.append('FEIG_LIB'); f and [print(f'✗ Missing: {x}') for x in f] or print('✓ Environment OK'); f and sys.exit(1)"

info:
	@echo "Platform: $(PLATFORM)"
	@echo "Python: $(PYTHON)"
	@echo "CXX: $(CXX)"
	@echo "Target: $(TARGET)"
	@echo "FEIG Include: $(FEIG_INCLUDE_DIR)"
	@echo "FEIG Lib: $(FEIG_LIB_DIR)"