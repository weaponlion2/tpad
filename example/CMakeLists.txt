cmake_minimum_required(VERSION 3.12)
project(feig_reader)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================
# FEIG SDK Configuration
# ==============================

set(FEIG_SDK_ROOT "." CACHE STRING "FEIG SDK root directory")
set(FEIG_INCLUDE_DIR "${FEIG_SDK_ROOT}/include" CACHE PATH "FEIG SDK include directory")
set(FEIG_LIB_DIR "${FEIG_SDK_ROOT}/x64/lib/release" CACHE PATH "FEIG SDK library directory")
message(STATUS "FEIG_LIB_DIR = ${FEIG_LIB_DIR}")
# ==============================
# Find Python and pybind11 (Optional)
# ==============================

option(BUILD_PYTHON_EXTENSION "Build Python extension module" ON)

if(BUILD_PYTHON_EXTENSION)
    find_package(Python COMPONENTS Interpreter Development)
    
    if(Python_FOUND)
        # Get pybind11 include directory from Python module
        execute_process(
            COMMAND ${Python_EXECUTABLE} -m pybind11 --includes
            OUTPUT_VARIABLE PYBIND11_INCLUDES
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )

        if(PYBIND11_INCLUDES AND EXISTS "${CMAKE_SOURCE_DIR}/src/bindings.cpp")
            # Extract include paths from pybind11 output (removes -I prefix)
            string(REPLACE "-I" ";" PYBIND11_INCLUDES "${PYBIND11_INCLUDES}")

            # Get pybind11 compile definitions
            execute_process(
                COMMAND ${Python_EXECUTABLE} -m pybind11 --cxxflags
                OUTPUT_VARIABLE PYBIND11_CXXFLAGS
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET
            )

            message(STATUS "pybind11 includes: ${PYBIND11_INCLUDES}")
            message(STATUS "pybind11 cxxflags: ${PYBIND11_CXXFLAGS}")

            # ==============================
            # Source Files for Python Extension
            # ==============================

            set(PYTHON_SOURCES
                src/bindings.cpp
                src/reader_manager.cpp
            )

            # ==============================
            # Create Python Extension Module
            # ==============================

            add_library(feig_reader SHARED ${PYTHON_SOURCES})

            # Set the output name and extension for Python modules
            set_target_properties(feig_reader PROPERTIES
                PREFIX ""
                SUFFIX ".pyd"
            )

            # Add pybind11 includes
            target_include_directories(feig_reader PRIVATE ${PYBIND11_INCLUDES})
            # ... (rest of Python config)
        else()
            message(WARNING "Skipping Python extension: src/bindings.cpp missing or pybind11 not fully functional")
            set(BUILD_PYTHON_EXTENSION OFF)
        endif()
    endif()
endif()

if(BUILD_PYTHON_EXTENSION)
    # Add pybind11 compile flags
    target_compile_options(feig_reader PRIVATE ${PYBIND11_CXXFLAGS})

    # Add FEIG SDK includes
    target_include_directories(feig_reader PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${FEIG_INCLUDE_DIR}
        ${Python_INCLUDE_DIRS}
    )
endif()

# ==============================
# Platform-Specific Configuration
# ==============================

if(WIN32)
    # Windows MSVC Configuration
    message(STATUS "Configuring for Windows (MSVC)")
    
    if(BUILD_PYTHON_EXTENSION)
        # Compiler flags for Windows
        target_compile_options(feig_reader PRIVATE
            /W3          # Warning level 3
            /O2          # Optimize for speed
            /EHsc        # Enable exception handling
            /MD          # Multi-threaded DLL runtime
        )
        
        # Linker directories
        target_link_directories(feig_reader PRIVATE 
            ${FEIG_LIB_DIR}
        )
        
        # Link FEIG SDK libraries
        target_link_libraries(feig_reader PRIVATE
            fedm
            fecom
            feisp
            feusb
            feudp
            fetls
            feble
            fedm-funit
            fedm-service
            ${Python_LIBRARIES}
        )
        
        # Copy .pyd to python/feig_reader directory
        add_custom_command(TARGET feig_reader POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                    $<TARGET_FILE:feig_reader>
                    ${CMAKE_SOURCE_DIR}/python/feig_reader/$<TARGET_FILE_NAME:feig_reader>
            COMMENT "Copying .pyd to python/feig_reader/"
        )
        
        # Copy FEIG SDK DLLs to python/feig_reader directory for runtime loading
        add_custom_command(TARGET feig_reader POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${CMAKE_SOURCE_DIR}/x64/bin/release"
                    "${CMAKE_SOURCE_DIR}/python/feig_reader"
            COMMENT "Copying FEIG SDK DLLs to python/feig_reader/"
        )
    endif()

else()
    # Linux/Unix Configuration
    message(STATUS "Configuring for Linux/Unix")
    
    if(BUILD_PYTHON_EXTENSION)
        # Compiler flags for Linux
        target_compile_options(feig_reader PRIVATE
            -Wall
            -Wextra
            -O2
            -fPIC
        )
        
        # Linker directories
        target_link_directories(feig_reader PRIVATE ${FEIG_LIB_DIR})
        
        # Libraries for Linux
        target_link_libraries(feig_reader PRIVATE
            fedm
            fecom
            feisp
            usb-1.0
            pthread
            dl
            ${Python_LIBRARIES}
        )
        
        # Set RPATH for runtime library loading
        set_target_properties(feig_reader PROPERTIES
            BUILD_RPATH "${FEIG_LIB_DIR}"
            INSTALL_RPATH "${FEIG_LIB_DIR};$ORIGIN/libs"
            INSTALL_RPATH_USE_LINK_PATH TRUE
        )
        
        # Copy .so to python/feig_reader directory
        add_custom_command(TARGET feig_reader POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                    $<TARGET_FILE:feig_reader>
                    ${CMAKE_SOURCE_DIR}/python/feig_reader/$<TARGET_FILE_NAME:feig_reader>
            COMMENT "Copying .so to python/feig_reader/"
        )
    endif()

endif()

# ==============================
# Create C++ CLI Executable
# ==============================

add_executable(feig_reader_cli 
    src/main.cpp
    src/reader_manager.cpp
)

# Add FEIG SDK includes to CLI executable
target_include_directories(feig_reader_cli PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${FEIG_INCLUDE_DIR}
)

if(WIN32)
    # Windows MSVC Configuration for CLI
    message(STATUS "Configuring CLI executable for Windows (MSVC)")
    
    # Compiler flags
    target_compile_options(feig_reader_cli PRIVATE
        /W3          # Warning level 3
        /O2          # Optimize for speed
        /EHsc        # Enable exception handling
        /MD          # Multi-threaded DLL runtime
    )
    
    # Linker directories
    target_link_directories(feig_reader_cli PRIVATE 
        ${FEIG_LIB_DIR}
    )
    
    # Link FEIG SDK libraries
    target_link_libraries(feig_reader_cli PRIVATE
        fedm
        fecom
        feisp
        feusb
        feudp
        fetls
        feble
        fedm-funit
        fedm-service
    )
    
    # Copy FEIG SDK DLLs to Release directory for runtime loading
    add_custom_command(TARGET feig_reader_cli POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/x64/bin/release"
                "${CMAKE_BINARY_DIR}/Release"
        COMMENT "Copying FEIG SDK DLLs to Release/"
    )

else()
    # Linux/Unix Configuration for CLI
    message(STATUS "Configuring CLI executable for Linux/Unix")
    
    # Compiler flags
    target_compile_options(feig_reader_cli PRIVATE
        -Wall
        -Wextra
        -O2
    )
    
    # Linker directories
    target_link_directories(feig_reader_cli PRIVATE ${FEIG_LIB_DIR})
    
    # Link FEIG SDK libraries
    target_link_libraries(feig_reader_cli PRIVATE
        fedm
        fecom
        feisp
        pthread
        dl
    )
    
    # Set RPATH for runtime library loading
    set_target_properties(feig_reader_cli PROPERTIES
        BUILD_RPATH "${FEIG_LIB_DIR}"
        INSTALL_RPATH "${FEIG_LIB_DIR};$ORIGIN"
        INSTALL_RPATH_USE_LINK_PATH TRUE
    )

endif()

# ==============================
# Build Info
# ==============================

add_custom_target(info
    COMMAND ${CMAKE_COMMAND} -E echo "=== Build Configuration ==="
    COMMAND ${CMAKE_COMMAND} -E echo "FEIG_INCLUDE_DIR: ${FEIG_INCLUDE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "FEIG_LIB_DIR: ${FEIG_LIB_DIR}"
)

# ==============================
# Clean Target
# ==============================

add_custom_target(distclean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Removing build directory"
)
