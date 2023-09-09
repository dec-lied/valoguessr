# ValoGuessr

ValoGuessr is an open source GeoGuessr-like windows desktop application for Valorant!

I made this game for my HL IB Computer Science Internal Assessment (although mostly for fun and experience) which was taking place in December of 2022.
At that time, there were only 8 maps, with Pearl being the most recent map. Unfortunately, I do not plan to add any additional maps because the code is a bit 🥴.

## Features
* This game is written entirely in C++, using barebones OpenGL and GLFW for graphics and features my custom (elementary) UI engine!
* All of your game data is saved in a local file, so all of your previous games, achievements, and stats are saved forever!
* You can share your previous games with friends or replay them yourself and compare the scores!

## Screenshots
These are just a few screenshots to show what the UI looks like. If you would like to see more screenshots, go to the [screenshots branch of this repo](https://github.com/declspecl/ValoGuessr/tree/screenshots).
<img src="../screenshots/mainMenu.png" width="650" height="370">
<img src="../screenshots/play.png" width="650" height="370">
<img src="../screenshots/settings.png" width="650" height="370">
<img src="../screenshots/roundResults.png" width="650" height="370">

## How to play
In the [releases page](https://github.com/declspecl/ValoGuessr/releases), download the newest release, navigate to the `/bin` folder, and run `valoguessr.exe`.

## Bulding from source
First, run ```git clone https://github.com/declspecl/ValoGuessr```.
Then, navigate inside the ValoGuessr folder and open the sln file in Visual Studio, allowing you to easily build the game from the source.

The Debug build mode should only be used for running the game from Visual Studio, as the paths for the assets are dependent on it.

The Release build mode is what should be used when compiling the standalone executable.
Create a `/bin` folder in `/valoguessr` and place the newly created valoguessr.exe into `/valoguessr/bin`.
Then place the appropriate dlls from the `/dlls` folder in the same directory as valoguessr.exe, which should be /valoguessr/bin.

## Warnings
In all my testing, the program has no memory leaks and (should) never crash. Though, if it does for you, I would be fascinated to learn why, so please let me know!

Also, the scaling method of the program is very naiva and is truly meant to be used in a 16:9 aspect ratio. So, for your best interest, I would try to keep it in it's default resolution or try to resize it to be in 16:9.

## Dependencies
This project uses numerous libraries and dependencies. Here is a list of them:
* [OpenGL](https://www.opengl.org/)
* [GLFW](https://www.glfw.org/)
* [GLAD](https://glad.dav1d.de/)
* [glm](https://github.com/g-truc/glm)
* [irrKlang](https://www.ambiera.com/irrklang/)
* [stb](https://github.com/nothings/stb)
* [FreeType](https://freetype.org/)

## Credit
The minimap images were edited by me but were all taken from mobalytics.gg.
* [Ascent](https://cdn.mobalytics.gg/assets/valorant/images/maps/levels/ascent_map.svg)
* [Bind](https://cdn.mobalytics.gg/assets/valorant/images/maps/levels/bind_map.svg)
* [Breeze](https://cdn.mobalytics.gg/assets/valorant/images/maps/levels/breeze_map.svg)
* [Fracture](https://cdn.mobalytics.gg/assets/valorant/images/maps/levels/fracture_map.svg)
* [Haven](https://cdn.mobalytics.gg/assets/valorant/images/maps/levels/haven_map.svg)
* [Icebox](https://cdn.mobalytics.gg/assets/valorant/images/maps/levels/icebox_map.svg)
* [Pearl](https://cdn.mobalytics.gg/assets/valorant/images/maps/levels/pearl_map.svg)
* [Split](https://cdn.mobalytics.gg/assets/valorant/images/maps/levels/split_map.svg)

The images of Valorant gameplay and splash art were all taken by me but all belong to Riot Games and were taken from their game Valorant.

The Valorant logo is used in various textures in ValoGuessr. All of these textures were created by me but contain assets belonging to Riot Games.

The font used in ValoGuessr was made by user [Brylark on dafont](https://www.dafont.com/valorant.font).

The following sounds were all recorded by me from Riot Games's game Valorant:
* valoclick.mp3
* valoclicky.mp3
* valohover.mp3
* valomenu.mp3
* valoplayclick.mp3
