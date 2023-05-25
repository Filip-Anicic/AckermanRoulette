<img align="left" width="80" height="80" src="https://media.githubusercontent.com/media/Filip-Anicic/AckermanRoulette/main/Resources/Icon128.png?token=AFRKIWTQC7IW6MKGUMXLH4TEDORWI" alt="Ackerman's Roulette Plugin">

# Ackerman's Roulette Plugin
Unreal Editor tool that spawns a random mesh in the world, from a customizable list of static meshes.

Built with C++ and Slate. Tested with UE 5.0, 5.1

## Demo
https://user-images.githubusercontent.com/23241818/227052633-9af2e8b6-a206-417d-ae66-a33c3694c790.mp4

## How to install
> Note: Repository uses Git LFS, so download ZIP option on GitHub would not download the entire repository content.

1. Create an Unreal Engine 5.0+ project.
2. Open root of the project in the file browser.
3. Add folder called **Plugins**.
4. Clone repository with Git and place into Plugins folder.
5. Launch project and rebuild module when prompted by Unreal Engine.
6. If no roulette toolbar icon is present, open the Plugins window, find and enable the Ackerman's Roulette plugin.

## How to use
1. When enabled, open Ackerman's Roulette using the toolbar button or **Window->Ackerman's Roulette**.
2. Use default plugin settings or change the data asset, spawn folder and spawn location.
3. Press "Spin roulette"

## How to create custom data
1. Open content browser.
2. Create a new Miscellaneous/DataAsset of class **RouletteDataAsset**.
3. Open the created data asset and fill out the Roulette Mesh array with Static Meshes.


## Known bugs
- Changes to plugin settings do not persist after editor session.
