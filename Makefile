# Windows Shell: PS
ifeq ($(OS),Windows_NT)
SHELL := powershell.exe
.SHELLFLAGS := -NoProfile -Command
endif

# Compiler
CC = gcc
CFLAGS = -Wall -Iinclude/ -export-dynamic 

# Directories
DIR_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
DIR_BIN = $(DIR_ROOT)bin
DIR_OBJ = $(DIR_ROOT)obj
DIR_SRC = $(DIR_ROOT)src
DIR_DATA = $(DIR_ROOT)data
DIR_LOGS = $(DIR_ROOT)logs
DIR_TMP = $(DIR_ROOT)tmp
DIR_OUT = $(DIR_ROOT)out

# Target name
ifeq ($(OS),Windows_NT)
TARGET = $(DIR_BIN)/huffman.exe
else
TARGET = $(DIR_BIN)/huffman.out
endif

# List of object files
OBJ_FILES := $(patsubst $(DIR_SRC)/%.c, $(DIR_OBJ)/%.o, $(wildcard $(DIR_SRC)/*.c))

# ------------------------------------------------------------------------------

# Default target
all: clean $(TARGET)

# ------------------------------------------------------------------------------

# Linking rule
$(TARGET): $(OBJ_FILES)
	@echo " ****************************************************** "
ifeq ($(OS),Windows_NT)
	@if ( -not ( Test-Path "$(DIR_BIN)" ) ) { mkdir "$(DIR_BIN)" }
	@Write-Host -NoNewLine " [*] Linking object files into binary file..."
else
	@if [ ! -d "$(DIR_BIN)" ]; then mkdir $(DIR_BIN); fi
	@echo -n " [*] Linking object files into binary file..."
endif
	@$(CC) $(CFLAGS) $(OBJ_FILES) -o $@
	@echo " OK"
	@echo " [*] Binary file created: $(TARGET)"

# ------------------------------------------------------------------------------

# Source to Object build rules
$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
ifeq ($(OS),Windows_NT)
	@if ( -not ( Test-Path "$(DIR_OBJ)" ) ) { mkdir "$(DIR_OBJ)" }
	@Write-Host -NoNewLine "     - Compiling $< into $@ ..."
else
	@if [ ! -d "$(DIR_OBJ)" ]; then mkdir $(DIR_OBJ); fi
	@echo -n "     - Compiling $< into $@ ..."
endif
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo " OK"

# ------------------------------------------------------------------------------

# Clean up rule
clean: clean_tmp clean_out
ifeq ($(OS),Windows_NT)
	@Write-Host " [*] Cleaning up binaries and objects..."
	@Write-Host -NoNewLine "   - Removing binary target file..."
	@if ( Test-Path "$(TARGET)" ) { Remove-Item "$(TARGET)"; Remove-Item "$(DIR_BIN)" }
	@Write-Host " OK"

	@Write-Host -NoNewLine "   - Removing objects directory..."
	@if ( Test-Path "$(DIR_OBJ)" ) { Remove-Item "$(DIR_OBJ)\*.o"; Remove-Item "$(DIR_OBJ)" }
	@Write-Host " OK"

	@Write-Host " [*] Clean up done."
	@Write-Host " ****************************************************** "
else
	@echo " [*] Cleaning up binaries and objects..."
	@echo -n "   - Removing binary target file..."
	@if [ -f "$(TARGET)" ]; then rm -f "$(TARGET)"; fi
	@echo " OK"

	@echo -n "   - Removing objects directory..."
	@if [ -d "$(DIR_OBJ)" ]; then rm -fr $(DIR_OBJ); fi
	@echo " OK"

	@echo " [*] Clean up done."
	@echo " ****************************************************** "
endif

clean_tmp:
ifeq ($(OS),Windows_NT)
	@Write-Host " [*] Cleaning up temporary files..."
	@if ( Test-Path "$(DIR_TMP)" ) { Remove-Item -Recurse -Force "$(DIR_TMP)\*" }
	@echo " OK"
else
	@echo " [*] Cleaning up temporary files..."
	@Write-Host -NoNewLine "   - Removing temporary files and directory..."
	@if [ -d "$(DIR_TMP)" ]; then rm -fr "$(DIR_TMP)"; fi
	@echo " OK"
endif

clean_out:
ifeq ($(OS),Windows_NT)
	@Write-Host " [*] Cleaning up output files..."
	@Write-Host -NoNewLine "   - Removing output files and directory..."
	@if ( Test-Path "$(DIR_OUT)" ) { Remove-Item -Recurse -Force "$(DIR_OUT)\*" }
	@echo " OK"
else
	@echo " [*] Cleaning up output files..."
	@echo -n "   - Removing output files and directory..."
	@if [ -d "$(DIR_OUT)" ]; then rm -fr "$(DIR_OUT)"; fi
	@echo " OK"
endif

clean_logs:
ifeq ($(OS),Windows_NT)
	@Write-Host " [*] Cleaning up logs..."
	@Write-Host -NoNewLine "   - Removing log files and directory..."
	@if ( Test-Path "$(DIR_LOGS)" ) { Remove-Item "$(DIR_LOGS)\*.log"; Remove-Item -Recurse -Force "$(DIR_LOGS)" }
	@echo " OK"
else
	@echo " [*] Cleaning up logs..."
	@echo -n "   - Removing log files..."
	@if [ -d "$(DIR_LOGS)" ]; then rm -fr "$(DIR_LOGS)"; fi
	@echo " OK"
endif