cmake_minimum_required(VERSION 3.10)
project(HF_TagAccess_Linux)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Platform detection
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "Building for Linux")
    set(LINUX_BUILD ON)
else()
    message(STATUS "Building for Windows")
    set(WINDOWS_BUILD ON)
endif()

# Find optional packages
find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(LIBUSB IMPORTED_TARGET libusb-1.0)
endif()

# Source directories
set(TAGACCESS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CPP_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../c++_lib)

# Include directories
include_directories(
    ${TAGACCESS_SRC_DIR}
    ${CPP_LIB_DIR}/inc
    ${LIBUSB_INCLUDE_DIRS}
)

# Core source files (platform-independent)
set(CORE_SOURCES
    gFunctions.cpp
    AccessConditions.cpp
    AsyncTaskExe.cpp
    TagMb89r118c.cpp
)

# Linux-only build
if(LINUX_BUILD)
    message(STATUS "Configuring for Linux platform")
    
    # Add Linux-specific implementation
    list(APPEND CORE_SOURCES rfid_tool_linux.cpp)
    
    # Create executable
    add_executable(rfid_tool
        ${CORE_SOURCES}
    )
    
    # Link libraries
    target_link_libraries(rfid_tool
        pthread
    )
    
    # Link libusb if available
    if(LIBUSB_FOUND)
        target_link_libraries(rfid_tool ${LIBUSB_LIBRARIES})
        message(STATUS "libusb found: ${LIBUSB_LIBRARIES}")
    else()
        message(WARNING "libusb-1.0-dev not found. Install with: sudo apt-get install libusb-1.0-0-dev")
    endif()
    
    # Add compiler flags
    target_compile_options(rfid_tool PRIVATE
        -Wall
        -Wextra
        -DLINUX_BUILD=1
    )
    
    # Manually specify rfidlib location if needed
    # Uncomment and update path if you have Linux rfidlib:
    # target_link_libraries(rfid_tool /path/to/librfidlib.so)
    # target_link_directories(rfid_tool PRIVATE /path/to/lib)
    
    # Installation
    install(TARGETS rfid_tool DESTINATION bin)
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "===== Build Configuration =====")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source Dir: ${TAGACCESS_SRC_DIR}")
message(STATUS "CPP Lib Dir: ${CPP_LIB_DIR}")
if(LIBUSB_FOUND)
    message(STATUS "libusb: Found")
else()
    message(STATUS "libusb: Not found (optional)")
endif()
message(STATUS "===============================")
message(STATUS "")
message(STATUS "Build with: cmake .. && make")
message(STATUS "")
