# ValoGuessr

ValoGuessr is a windows desktop application inspired by GeoGuessr for Valorant!

It is written entirely in C++, using OpenGL and GLFW for graphics and feature my custom (elementary) framework engine!

## How to run
In the releases tab, download the newest release, navigate to the /bin folder, and run valoguessr.exe.

## Bulding from source
Opening the sln file in Visual Studio will allow you to easily build the game from the source code.

The Debug build mode should only be used for running the game from Visual Studio, as the paths for the assets are dependent on it.

The Release build mode is what should be used when compiling the executable. Create a /bin folder in /valoguessr and place the newly created valoguessr.exe into /valoguessr/bin.
Then place the appropriate dlls from the /dlls folder in the same directory as valoguessr.exe, which should be /valoguessr/bin.

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

The Valorant logo is used in various textures in ValoGuessr. All of these textures were made by me but belong to Riot Games.

The font used in ValoGuessr was made by user Bryan T on dafont, the website from where I downloaded the font.

The following sounds were all recorded by me from Riot Games's game Valorant:
* valoclick.mp3
* valohover.mp3
* valomenu.mp3
* valoplayclick.mp3
