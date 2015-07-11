TASView
==========

Shows stuff from BXT (`bxt_interprocess_enable 1`).

#Running
- Get some TTF font, name it **font.ttf**, put where the TASView executable is, run the TASView executable.

#Building
####Linux
- You will need **32-bit** Boost libraries **chrono**, **date_time**, **system** and **thread**.
- Create a build directory along the *src* directory.
- Run `cmake ..` from the build directory. You might need to specify `-DBOOST_ROOT=path/to/boost/base/dir` if you're running a 64-bit linux and have built the 32-bit Boost libraries separately.
- Run `make` from the build directory.
