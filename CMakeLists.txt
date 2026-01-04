cmake_minimum_required(VERSION 3.16)

project(memory_forensics
    VERSION 0.1.0
    LANGUAGES C
)

# ============================
# Options
# ============================
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ============================
# Include directories
# ============================
include_directories(
    api/include
    src
)

# ============================
# Source files
# ============================
file(GLOB_RECURSE MEMORY_FORENSICS_SOURCES
    src/core/*.c
    src/logs/*.c
    src/utils/*.c
    src/os/linux/*.c
)

# ============================
# Library target
# ============================
add_library(memory_forensics STATIC
    ${MEMORY_FORENSICS_SOURCES}
)

target_compile_options(memory_forensics PRIVATE
    -Wall
    -Wextra
    -Werror
    -g
)

# ============================
# Install rules (opção 3 friendly)
# ============================
install(TARGETS memory_forensics
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(
    DIRECTORY api/include/
    DESTINATION include/memory_forensics
)



