CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

TARGET_DIR = bin
OBJ_DIR = obj
SRC_DIR = src

EXECUTABLE = googit
TARGET = $(TARGET_DIR)/$(EXECUTABLE)

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Build Complete: $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

install: all
	@echo "Installing $(EXECUTABLE) to /usr/local/bin..."
	@sudo cp $(TARGET) /usr/local/bin
	@sudo chmod +x /usr/local/bin/$(EXECUTABLE)
	@echo "$(EXECUTABLE) installed succesfully."

uninstall:
	@echo "uninstalling $(EXECUTABLE) from /usr/local/bin..."
	@sudo rm -f /usr/local/bin/$(EXECUTABLE)
	@echo "$(EXECUTABLE) uninstalled."

clean:
	@echo "Cleaning up build files..."
	@rm -rf $(TARGET_DIR) $(OBJ_DIR)
	@rm -rf .googit xv6-public

.PHONY: all clean install uninstall