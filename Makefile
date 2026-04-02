# ===============================
# Minimal Windows 32-bit Driver Makefile
# Uses MinGW-w64, pure C, multiple headers
# ===============================

# Driver configuration
DRIVER_NAME   = ${target}
DRIVER_SOURCES = ${target}.c
DRIVER_OBJECTS = $(DRIVER_SOURCES:.c=.o)
DRIVER_TARGET  = $(DRIVER_NAME).sys

# Compiler and linker
CC = x86_64-w64-mingw32-gcc
LD = x86_64-w64-mingw32-gcc

# Include directories (add as many as needed)
INCLUDES = -I/usr/x86_64-w64-mingw32/include/ddk \
           -I/usr/x86_64-w64-mingw32/include \
           -I./include

# Compiler flags
CFLAGS = -D_KERNEL_MODE -m64 -fno-asynchronous-unwind-tables -fno-exceptions

# Linker flags

#LDFLAGS = -nostdlib \
#          -Wl,--subsystem,native \
#          -Wl,--image-base,0x0000000140000000 \
#          -Wl,--dynamicbase \
#          -Wl,--nxcompat \
#          -Wl,-e,DriverEntry

LDFLAGS = -nostdlib \
          -Wl,--subsystem,native \
          -Wl,--image-base,0x0000000140000000 \
          -Wl,--dynamicbase \
          -Wl,--nxcompat \
          -Wl,-e,DriverEntry \
          -lntoskrnl


# ===============================
# Rules
# ===============================

# Compile each C source to object file
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# Link object files into .sys driver
$(DRIVER_TARGET): $(DRIVER_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(DRIVER_OBJECTS) -lntoskrnl

# Clean up build artifacts
.PHONY: clean
clean:
	rm -f $(DRIVER_OBJECTS) $(DRIVER_TARGET)



