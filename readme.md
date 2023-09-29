# Pokerom Trader - Pokémon Save File Trading GUI

![file select screen](https://github.com/savaughn/pokerom-trader/assets/25937456/77095008-ae24-4a1c-9045-5a818d0046ec)
![Pokerom Trader](https://github.com/savaughn/pokerom-trader/assets/25937456/c55aa92e-24ef-4ddf-931c-44c58d384621)

## Overview

Pokerom Trader is an open-source project that simplifies the process of trading Pokémon between two save files using the PKSav C library. This graphical user interface (GUI) provides an intuitive way for Pokémon enthusiasts to transfer Pokémon and items between different save files or Pokémon game versions.

## Discord
https://discord.gg/JUzzegS3AP

## Getting Started
Currently this only runs on MacOS M1s.

You'll need to make the executable executable by running the following command in the directory where the executable is located:
```bash
chmod +x pokeromtrader
```
You'll need to allow the executable to run on your computer by going to System Preferences > Security & Privacy or by right clicking the executable and selecting "Open" for the first time for both the executable and the pksav library (libpksav.dylib). 

Default save files folder is located in the "saves" folder. You can change it by going to settings and giving the path to your save files folder relative to the executable.


## Contributing
If you wish to build this project from source or contribute to the project, then follow the instructions below.
### Installation

   ```bash
   git clone https://github.com/savaughn/pokerom-trader.git
   cd pokerom-trader
   ```

   #### Build pksav dependency

   ```bash
   cd deps/lpksav
   cmake .
   make
   cd ..
   ```

   #### Build raylib dependency

   ```bash
   cd raylib/src
   make PLATFORM=PLATFORM_DESKTOP
   cd ../..
   ```

   #### Build Pokerom Trader

   ```bash
   make
   ```


## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- [PKSav](https://github.com/ncorgan/pksav): Thank you to the creators and maintainers of PKSav for providing the library that makes this project possible.
- [Raylib](www.github.com/raysan5/raylib): Thank you to the creators and maintainers of Raylib for providing the library that makes this project possible.
