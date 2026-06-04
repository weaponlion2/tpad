# RFID TagAccess - Linux Setup & Build Guide

## Quick Start

### System Requirements
- **OS**: Ubuntu 18.04 LTS or newer (Debian 10+)
- **Architecture**: x86_64 (ARM support possible with modifications)
- **Disk Space**: ~500MB for build tools
- **Memory**: 1GB minimum

### Step 1: Install Build Dependencies

```bash
# Update package manager
sudo apt-get update

# Install essential build tools
sudo apt-get install -y \
    build-essential \
    cmake \
    gcc \
    g++ \
    make \
    pkg-config \
    libusb-1.0-0-dev \
    git
```

**Verify installation:**
```bash
cmake --version
gcc --version
```

### Step 2: Prepare the Source Code

```bash
# Navigate to TagAccess directory
cd ~/path/to/samples/c++/TagAccess

# Make build script executable
chmod +x build_linux.sh
```

### Step 3: Build the Project

**Option A: Using the build script (recommended)**
```bash
./build_linux.sh
```

**Option B: Manual build**
```bash
mkdir -p build
cd build
cmake ..
make
cd ..
```

### Step 4: Test the Build

```bash
# Show help
./build/rfid_tool help

# Show version
./build/rfid_tool version

# List available commands
./build/rfid_tool
```

## Project Structure

```
TagAccess/
├── CMakeLists.txt                 # Build configuration
├── build_linux.sh                 # Build script
├── rfid_tool_linux.cpp            # Main Linux CLI implementation
│
├── Core Functions (Keep on Linux)
├── gFunctions.cpp/h               # Utility functions
├── AccessConditions.cpp/h         # Tag access logic
├── AsyncTaskExe.cpp/h             # Async operations
├── TagMb89r118c.cpp/h             # MB89R118C tag support
├── myinc.h, mytypes.h, myopt.h   # Type definitions
│
├── Windows-Specific (Remove)
├── Dlg*.cpp/h                     # MFC dialog classes
├── demo_m24rl64.cpp/h             # Windows MFC app
├── *.rc, *.aps                    # Resource files
└── ...
```

## Building Without Linux rfidlib (Current State)

Since the Linux version of rfidlib may not be available yet, the current build:
- ✅ Compiles successfully
- ✅ Includes stub implementations
- ⚠️ Does NOT connect to actual hardware

### Placeholder Output Example

```bash
$ ./rfid_tool inventory
[*] Starting RFID inventory scan...
[*] Scanning for ISO15693 and ISO14443A tags...

Found tags:
  UID: E00000000000000ABC123  Type: ISO15693  Signal: -45dBm
  UID: 04A1B2C3D4E5F6789ABC   Type: ISO14443A Signal: -52dBm

[!] NOTE: This is a placeholder output. Actual implementation requires rfidlib.
```

## Integrating with Real rfidlib

When you obtain Linux version of rfidlib:

### 1. Copy Library Files
```bash
# Copy library binaries
cp librfidlib.so /path/to/c++_lib/lib_linux_x86_64/
cp librfidlib.a  /path/to/c++_lib/lib_linux_x86_64/

# Copy/update header files
cp rfidlib*.h /path/to/c++_lib/inc/
```

### 2. Update CMakeLists.txt

```cmake
# Add to CMakeLists.txt in the Linux section:
set(RFIDLIB_PATH ${CPP_LIB_DIR}/lib_linux_x86_64)

# Link the rfidlib library
target_link_libraries(rfid_tool
    ${RFIDLIB_PATH}/librfidlib.so
    pthread
)

# Or for static linking:
# target_link_libraries(rfid_tool
#     ${RFIDLIB_PATH}/librfidlib.a
#     pthread
# )
```

### 3. Complete Implementation

Replace the placeholder functions in `rfid_tool_linux.cpp` with actual rfidlib calls:

```cpp
int cmd_inventory() {
    int ret;
    
    // Open reader connection
    unsigned int hReader = 0;
    ret = rfidlib_CreateReader("", "", &hReader);
    if (ret != 0) {
        std::cerr << "Failed to open reader\n";
        return 1;
    }
    
    // Enable antenna
    ret = rfidlib_EnableAntenna(hReader, 0, true);
    // ... rest of inventory logic
    
    rfidlib_ReleaseReader(&hReader);
    return 0;
}
```

## Troubleshooting

### CMake Not Found
```bash
sudo apt-get install cmake
```

### Build Fails: "missing dependency"
```bash
# Install missing package (example for pkg-config)
sudo apt-get install pkg-config
```

### libusb-1.0 Not Found (Optional Warning)
```bash
sudo apt-get install libusb-1.0-0-dev
```

### Compilation Errors with TCHAR or Windows Types
- Already handled in `rfid_tool_linux.cpp`
- You may need to update `stdafx.h` if including platform-specific headers

### "No such file or directory" Error
```bash
# Verify your working directory
pwd
# Should be in .../samples/c++/TagAccess/

# Check CMakeLists.txt path references
cat CMakeLists.txt | grep -i "include_directories"
```

### Permission Denied when Running rfid_tool
```bash
# Make it executable
chmod +x ./build/rfid_tool
```

## Platform-Specific Configuration

### Ubuntu/Debian 18.04
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake
./build_linux.sh
```

### Ubuntu/Debian 20.04+
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake g++
./build_linux.sh
```

### CentOS/RHEL (if porting to these)
```bash
sudo yum install -y gcc gcc-c++ cmake make
sudo yum install -y libusb-devel
./build_linux.sh
```

### Raspberry Pi / ARM64
Additional steps:
```bash
# Install ARM-specific tools
sudo apt-get install -y gcc-arm-linux-gnueabihf

# Build for ARM (advanced)
# Would require ARM cross-compilation configuration in CMakeLists.txt
```

## Docker Build (Optional)

Create a `Dockerfile` for consistent builds:

```dockerfile
FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
    build-essential cmake gcc g++ make pkg-config libusb-1.0-0-dev

WORKDIR /workspace
COPY . /workspace

RUN cd TagAccess && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make

ENTRYPOINT ["./TagAccess/build/rfid_tool"]
```

Build and run:
```bash
docker build -t rfid_tool .
docker run rfid_tool help
```

## Next Steps

1. ✅ Basic Linux build working
2. ⏳ Obtain Linux version of rfidlib from manufacturer
3. ⏳ Integrate rfidlib binaries
4. ⏳ Complete rfidlib wrapper functions
5. ⏳ Test with actual RFID reader hardware

## Support & Resources

- **CMake Documentation**: https://cmake.org/documentation/
- **libusb Documentation**: http://libusb.info/
- **Linux Development**: https://www.kernel.org/doc/html/latest/

## Common Commands

```bash
# Build
cd build && cmake .. && make && cd ..

# Clean build
rm -rf build && mkdir build && cd build && cmake .. && make

# Install (optional)
cd build && sudo make install && cd ..

# Run with verbose output
./build/rfid_tool -v

# Help
./build/rfid_tool help
```

---

**Status**: Basic Linux build ready. Awaiting Linux rfidlib binaries from manufacturer.
