# Define compiler and flags
CC = gcc
CFLAGS = -Ilibpksav/include -Llibpksav/lib -lpksav

# Define source file and executable name
SOURCE = src/main.c
EXECUTABLE = src/pokeromtrader.out
BUILD_DIR = build/macos

# Define App Properties
APP_NAME = PokeromTrader
APP_VERSION = 0.1.0
APP_BUNDLE_ID = com.yourcompany.pokeromtrader
APP_BUNDLE_NAME = PokeromTrader
APP_BUNDLE_EXECUTABLE = pokeromtrader

# Define macOS bundle directory
MACOS_BUNDLE = PokeromTrader.app
MACOS_DESTINATION = $(BUILD_DIR)/$(MACOS_BUNDLE)/Contents/MacOS
# Define macOS bundle directory
INFO_PLIST = $(BUILD_DIR)/$(MACOS_BUNDLE)/Contents/Info.plist

# Define mGBA path and save file
ROM_PATH = $(realpath rom)
ROM_FILE = pk-crystal.gbc


.PHONY: all run launch clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE)
	@$(CC) $(SOURCE) -o $(EXECUTABLE) $(CFLAGS)

run: $(EXECUTABLE)
	@cd src && ./pokeromtrader.out && cd ..

launch: all run

clean-exe:
	@rm -f $(EXECUTABLE)

# Clean build folder
clean-build:
	@rm -rf build

## clean all
clean: clean-exe clean-build

bundle: all
	@# Create the build directory if it doesn't exist
	@mkdir -p "$(BUILD_DIR)"
	
	@# Create the directory structure
	@mkdir -p "$(MACOS_DESTINATION)"
	@mkdir -p "$(MACOS_BUNDLE)/Contents/Resources"
	
	@# Copy the executable to the .app bundle
	@cp "$(EXECUTABLE)" "$(MACOS_DESTINATION)/$(APP_BUNDLE_EXECUTABLE)"
	
	@# Create Info.plist file
	@echo '<?xml version="1.0" encoding="UTF-8"?>' > "$(INFO_PLIST)"
	@echo '<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">' >> "$(INFO_PLIST)"
	@echo '<plist version="1.0">' >> "$(INFO_PLIST)"
	@echo '<dict>' >> "$(INFO_PLIST)"
	@echo '    <key>CFBundleExecutable</key>' >> "$(INFO_PLIST)"
	@echo '    <string>$(APP_BUNDLE_EXECUTABLE)</string>' >> "$(INFO_PLIST)"
	@echo '    <key>CFBundleIdentifier</key>' >> "$(INFO_PLIST)"
	@echo '    <string>$(APP_BUNDLE_ID)</string>' >> "$(INFO_PLIST)"
	@echo '    <key>CFBundleName</key>' >> "$(INFO_PLIST)"
	@echo '    <string>$(APP_BUNDLE_NAME)</string>' >> "$(INFO_PLIST)"
	@echo '</dict>' >> "$(INFO_PLIST)"
	@echo '</plist>' >> "$(INFO_PLIST)"
	
	@# Make the executable executable
	@chmod +x "$(MACOS_DESTINATION)/$(APP_BUNDLE_EXECUTABLE)"
	
	@echo "$(MACOS_BUNDLE) bundle created"

.DEFAULT:
	@echo "Nothing to do. Please specify a target (e.g., 'make run')."

mgba:
	@mgba "$(ROM_PATH)/$(ROM_FILE)"

