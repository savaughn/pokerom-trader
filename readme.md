# Pokerom Trader - Pokémon Save File Trading GUI


![Pokerom Trader](https://user-images.githubusercontent.com/25937456/270499867-b2119796-1e0e-47f3-9358-96836b64c0df.png)

## Overview

Pokerom Trader is an open-source project that simplifies the process of trading Pokémon between two save files using the PKSav C library. This graphical user interface (GUI) provides an intuitive way for Pokémon enthusiasts to transfer Pokémon and items between different save files or Pokémon game versions.

## Discord
https://discord.gg/JUzzegS3AP

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

- PKSav: Thank you to the creators and maintainers of PKSav for providing the library that makes this project possible.
