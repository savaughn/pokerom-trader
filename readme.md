# Pokerom Trader - Pokémon Save File Trading GUI

![main_menu](https://github.com/savaughn/pokerom-trader/assets/25937456/78ef12ba-b04c-471d-a1b2-36bc55ab731e)

#
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/savaughn/pokerom-trader)](https://github.com/savaughn/pokerom-trader/releases) [![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/savaughn/pokerom-trader/build-macos.yml?branch=main)](https://github.com/savaughn/pokerom-trader/actions) [![GitHub issues](https://img.shields.io/github/issues/savaughn/pokerom-trader)](https://github.com/savaughn/pokerom-trader/issues) [![GitHub](https://img.shields.io/github/license/savaughn/pokerom-trader)](https://github.com/savaughn/pokerom-trader/blob/main/LICENSE) [![GitHub all releases](https://img.shields.io/github/downloads/savaughn/pokerom-trader/total)](https://github.com/savaughn/pokerom-trader/releases)

## Overview

Pokerom Trader is an open-source project that simplifies the process of trading Pokémon between two save files using the [PKSav](https://github.com/savaughn/pksav) C library. This graphical user interface (GUI) provides an intuitive way for Pokémon enthusiasts to transfer Pokémon between different save files or Pokémon game versions. This is not another save file editor. This replicates the in-game trading experience resulting in legal pokémon.
There is no backup system implemented yet. Make backup saves before using this on your own personal files, because this is still very early stage and has not been thoroughly tested. 
### Features
- Trade - Allows a user to trade Pokémon between save files
  - Trade with NPCs - Allows a user to trade Pokémon with NPCs such as gym leaders or Red (future feature)
- Evolve - A shortcut for evolving Pokémon that only evolve through trading
  - This replicates a trade, the evolution, and a trade back to OT
- Bill's PC - Allows a user to view and manage their Pokémon boxes and party (in progress)

### Settings 
![settings_menu](https://github.com/savaughn/pokerom-trader/assets/25937456/72e15dca-461a-40eb-a095-cc8d079976c4)
- Change save files folder with absolute path to folder
  - Default is ~/Library/PokeromTrader/saves (MacOS)
  - Default is ~/.pokeromtrader/ (Linux)
  - Default is \<UserProfile\>\\Documents\\PokeromTrader\\saves (Windows)
- Disable Random DVs on trade (default off) when on will retain the dvs of the Pokémon being traded or evolved.
  - The in-game experience always randomizes DVs on trade. This is a bypass of the official experience.
- Reset to defaults - Resets all settings to default values

### Deep Dive
- DV randomization
  - Random function is 1:1 with the in-game Random call converted from assembly to C which generates an add byte for both generations.
  - The add byte is anded with 0xF to get the lower 4 bits of the random byte (i.e. 0 to 15).
  - The DV for HP is calculated by taking the least significant bit of each DV (attack, defense, speed, special) and concatenating them together into a single byte.
- Stats calculations
  - Calculated using formula from [bulbapedia](https://bulbapedia.bulbagarden.net/wiki/Stats).
  - Based off of the Pokémon's base stats, level, DVs (stored or generated), and EVs all of which are read from the save file sram.

## What's working

- Same-gen trading  (Gen 1 <-> Gen 1, Gen 2 <-> Gen 2)
- Cross-gen trading (Gen 1 <-> Gen 2)
- Gen 1 trade evolutions
- Gen 2 trade evolutions

*Perfect Crystal save files seem to work.

## What's not working
- See [issues tab](https://github.com/savaughn/pokerom-trader/issues) for current bugs 
- No JP Region support

## Discord
https://discord.gg/JUzzegS3AP

## Getting Started

| Platform          | Compatibility   | default save folder |
|-------------------|-----------------|---------|
| Windows           | ✅ | \<UserProfile\>\\Documents\\PokeromTrader\\saves
| Linux amd64       | ✅ |~/.pokeromtrader/saves
| Linux arm64       | ✅ |~/.pokeromtrader/saves
| MacOS x86_64      | ✅  |~/Library/Application Support/PokeromTrader/saves 
| MacOS arm64       | ✅  |~/Library/Application Support/PokeromTrader/saves 
| iOS               | ❌  |
| Android           | ❌  |


Download the latest release from the [releases page](https://github.com/savaughn/pokerom-trader/releases).

### MacOS
To execute, right click PokeromTrader.app and select "Open" to bypass the MacOS security warning. This is only required the first time you open the app.

## Contributing
 
 [Contributing Guide](CONTRIBUTING.md)

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- [PKSav](https://github.com/ncorgan/pksav): Thank you to the creators and maintainers of PKSav for providing the library that makes this project possible.
- [Raylib](www.github.com/raysan5/raylib): Thank you to the creators and maintainers of Raylib for providing the library that makes this project possible.
- [Bulbapedia](https://bulbapedia.bulbagarden.net/wiki/Main_Page): Thank you to the creators and maintainers of Bulbapedia for providing the information that makes this project possible.
- [TextStudio](https://www.textstudio.com/logo/pokemon-3d-text-318): Used their text generator for stylized text.
- [Cazwolf](https://cazwolf.itch.io/caz-pixel-consoles): Used their pixel art for the consoles and catridges.
- [Miguel PM Romeu](https://miguel-pm-romeu.itch.io/pack-of-pokeballs): Modified their pokeball sprites.

## Disclaimer
Pokerom Trader is an unofficial application and is not affiliated with or endorsed by Nintendo, Game Freak, Creatures, The Pokémon Company, or any related entities. Pokémon and Pokémon character names are trademarks of Nintendo, Game Freak, Creatures, and The Pokémon Company. All trademarks, character names, and other intellectual property used in this application are used for identification and informational purposes only. The use of these names and marks is believed to qualify as fair use under trademark law. Pokerom Trader is not endorsed by or affiliated with any of the aforementioned entities. Pokerom Trader is provided "as is" without warranty of any kind, and the developers make no warranties, express or implied, regarding the accuracy or completeness of the content provided in this application.

## Demos
### Trade

![trade_demo](https://github.com/savaughn/pokerom-trader/assets/25937456/d33ebe5f-d3ab-4b58-b67e-9cc4a8c7633f)

### Evolve

![evolve_demo](https://github.com/savaughn/pokerom-trader/assets/25937456/4e97170c-d3ed-44ee-9fe5-19c5c25792ee)

