TASView
==========

Shows stuff from BXT (`bxt_interprocess_enable 1`).

#Running
- Get some TTF font, name it **font.ttf**, put where the TASView executable is, run the TASView executable.
- Windows: you will also need **SDL2.dll**, **SDL2_ttf.dll**, **zlib1.dll** and **libfreetype-6.dll** in that place.

#Building
####Windows
- Get Visual Studio 2015.
- You will need SDL2 and SDL2_ttf development libraries.
- You will need the Boost headers.
- Create a build directory along the *src* directory.
- Run `cmake -DBOOST_ROOT=path\to\boost\base\dir -DSDL2_ROOT=path\to\SDL2\base\dir -DSDL2_TTF_ROOT=path\to\SDL2_ttf\base\dir ..` from the build directory.
- Open the resulting .sln file and build it.

####Linux
- You will need the Boost headers, SDL2 and SDL2_ttf.
- Create a build directory along the *src* directory.
- Run `cmake ..` from the build directory.
- Run `make` from the build directory.
