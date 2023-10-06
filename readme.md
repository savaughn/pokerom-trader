# Pokerom Trader - Pokémon Save File Trading GUI

![file select screen](https://github.com/savaughn/pokerom-trader/assets/25937456/77095008-ae24-4a1c-9045-5a818d0046ec)
![Pokerom Trader](https://github.com/savaughn/pokerom-trader/assets/25937456/c55aa92e-24ef-4ddf-931c-44c58d384621)

## Overview

Pokerom Trader is an open-source project that simplifies the process of trading Pokémon between two save files using the PKSav C library. This graphical user interface (GUI) provides an intuitive way for Pokémon enthusiasts to transfer Pokémon and items between different save files or Pokémon game versions.

There is no backup system implemented yet. Make backup saves before using this on your own personal files, because this is still very early stage and has not been thoroughly tested. 

Perfect Crystal save files seem to work.

## What's working
<style>
  .green {
    color: green;
  }

  .red {
    color: red;
  }
</style>

|      | Red  | Blue | Yellow | Gold | Silver | Crystal |
|------|------|------|--------|------|--------|---------|
| Red  | ✅  | ✅  | ✅      | ❌  | ❌     | ❌      |
| Blue | ✅  | ✅   | ✅      | ❌  | ❌     | ❌     |
| Yellow | ✅  | ✅   | ✅      | ❌  | ❌    | ❌     |
| Gold | ❌  | ❌   | ❌     | ✅  | ✅     | ✅      |
| Silver | ❌  | ❌   | ❌      | ✅  | ✅     | ✅      |
| Crystal | ❌  | ❌   | ❌      | ✅  | ✅     | ✅      |


## What's not working
- See [issues tab](https://github.com/savaughn/pokerom-trader/issues) for current bugs 

## Discord
https://discord.gg/JUzzegS3AP

## Getting Started

<style>
  .red {
    color: red;
  }

  .green {
    color: green;
  }
</style>

| Platform          | Compatibility   |
|-------------------|-----------------|
| Windows           | <span class="red">&#10007;</span>  |
| Linux             | <span class="red">&#10007;</span>   |
| MacOS x86_64      | <span class="green">&#10003;</span>   |
| MacOS arm64       | <span class="green">&#10003;</span>   |
| iOS               | <span class="red">&#10007;</span>  |
| Android           | <span class="red">&#10007;</span>  |
| Nintendo Switch   | <span class="red">&#10007;</span>  |


Download the latest release from the [releases page](https://github.com/savaughn/pokerom-trader/releases).

Put your folder containing your save somewhere with a short path such as your desktop, home folder, or documents folder.
In the app, click settings on the main menu and select 'Change save directory'. Input the full path to the folder containing your save files.
e.g. /Users/username/Documents/saves

To execute, right click PokeromTrader.app and select "Open" to bypass the MacOS security warning.

## Contributing
If you wish to build this project from source or contribute to the project, then follow the instructions below. Tip: You can just copy/paste the entire block of commands into your terminal.
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
