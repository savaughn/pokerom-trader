# Pokerom Trader - Pokémon Save File Trading GUI

## Overview

Pokerom Trader is an open-source project that simplifies the process of trading Pokémon between two save files using the PKSav C library. This graphical user interface (GUI) provides an intuitive way for Pokémon enthusiasts to transfer Pokémon between different save files or Pokémon game versions. This is not another save file editor. This replicates the in-game trading experience resulting in legal pokémon.
There is no backup system implemented yet. Make backup saves before using this on your own personal files, because this is still very early stage and has not been thoroughly tested. 
### Features
- Trade - Allows a user to trade Pokémon between save files (currently only within the same generation)
  - Trade with NPCs - Allows a user to trade Pokémon with NPCs such as gym leaders or Red (future feature)
- Evolve - A shortcut for evolving Pokémon that only evolve through trading
  - This replicates a trade, the evolution, and a trade back to OT
- Bill's PC - Allows a user to view and manage their Pokémon boxes and party (in progress)

### Settings 
- Change save files folder with absolute path to folder
  - Default is ~/Library/PokeromTrader/saves (MacOS)
- Disable Random DVs on trade (default off) when on will retain the dvs of the Pokémon being traded or evolved.
  - The in-game experience always randomizes DVs on trade. This is a bypass of the official experience.

### Deep Dive
- DV randomization
  - Random function is 1:1 with the in-game Random call converted from assembly to C which generates an add byte for both generations.
  - The add byte is anded with 0xF to get the lower 4 bits of the random byte (i.e. 0 to 15).
  - The DV for HP is calculated by taking the least significant bit of each DV (attack, defense, speed, special) and concatenating them together into a single byte.
- Stats calculations
  - Calculated using formula from [bulbapedia](https://bulbapedia.bulbagarden.net/wiki/Stats).
  - Based off of the Pokémon's base stats, level, DVs (stored or generated), and EVs all of which are read from the save file sram.

## What's working

- Same-gen trading
- Cross-gen trading
- Gen 1 trade evolutions
- Gen 2 trade evolutions

*Perfect Crystal save files seem to work.

## What's not working
- See [issues tab](https://github.com/savaughn/pokerom-trader/issues) for current bugs 

## Discord
https://discord.gg/JUzzegS3AP

## Getting Started

| Platform          | Compatibility   |
|-------------------|-----------------|
| Windows           | ❌ |
| Linux (built with ubuntu)             | ✅ |
| MacOS x86_64      | ✅  |
| MacOS arm64       | ✅  |
| iOS               | ❌  |
| Android           | ❌  |
| Nintendo Switch   | ❌  |


Download the latest release from the [releases page](https://github.com/savaughn/pokerom-trader/releases).

Default saves folder is located ~/Library/PokeromTrader/saves on OSX and ~/.pokeromtrader/ in linux. You can change this in the settings menu.

To execute, right click PokeromTrader.app and select "Open" to bypass the MacOS security warning. This is only required the first time you open the app.

## Contributing
If you wish to build this project from source or contribute to the project, then follow the instructions below.
### Installation

   ```bash
   git clone https://github.com/savaughn/pokerom-trader.git
   cd pokerom-trader
   ```

   #### Build pksav dependency

   ```bash
   cd deps/pksav
   mkdir build && cd build
   cmake ..
   cmake --build .
   cd ../..
   ```

   #### Build raylib dependency

   ```bash
   cd raylib/src
   make PLATFORM=PLATFORM_DESKTOP
   cd ../../..
   ```

   #### Build Pokerom Trader

   ```bash
   make
   ```

   #### Build and Run Pokerom Trader

   ```bash
   make launch
   ```

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- [PKSav](https://github.com/ncorgan/pksav): Thank you to the creators and maintainers of PKSav for providing the library that makes this project possible.
- [Raylib](www.github.com/raysan5/raylib): Thank you to the creators and maintainers of Raylib for providing the library that makes this project possible.

## Disclaimer
Pokerom Trader is an unofficial application and is not affiliated with or endorsed by Nintendo, Game Freak, Creatures, The Pokémon Company, or any related entities. Pokémon and Pokémon character names are trademarks of Nintendo, Game Freak, Creatures, and The Pokémon Company. All trademarks, character names, and other intellectual property used in this application are used for identification and informational purposes only. The use of these names and marks is believed to qualify as fair use under trademark law. Pokerom Trader is not endorsed by or affiliated with any of the aforementioned entities. Pokerom Trader is provided "as is" without warranty of any kind, and the developers make no warranties, express or implied, regarding the accuracy or completeness of the content provided in this application.
