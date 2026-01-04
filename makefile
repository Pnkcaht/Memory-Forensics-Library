# ============================
# Toolchain
# ============================
CC      := gcc
AR      := ar
ARFLAGS := rcs

# ============================
# Flags
# ============================
CFLAGS := -Wall -Wextra -Werror -g \
          -Iapi/include \
          -Isrc

# ============================
# Library
# ============================
LIB_NAME := libmemorycore.a

# ============================
# Core sources (PUBLIC)
# ============================
CORE_SRC := \
    src/core/context.c \
    src/core/process.c \
    src/core/snapshot.c \
    src/core/diff.c

# ============================
# Logs
# ============================
LOG_SRC := \
    src/logs/log.c

# ============================
# Utils
# ============================
UTILS_SRC := \
    $(wildcard src/utils/*.c)

# ============================
# Linux backend
# ============================
LINUX_SRC := \
    $(wildcard src/os/linux/*.c)

# ============================
# Final source list
# ============================
SRC := $(CORE_SRC) $(LOG_SRC) $(UTILS_SRC) $(LINUX_SRC)

OBJ := $(SRC:.c=.o)

# ============================
# Targets
# ============================
all: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(LIB_NAME)

.PHONY: all clean
