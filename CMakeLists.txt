cmake_minimum_required (VERSION 3.1)
project (TASView)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -mtune=native -m32 -Ofast -Wall -Wextra -Wno-deprecated-declarations")

find_package (Boost 1.57.0 REQUIRED COMPONENTS chrono date_time system thread)
include_directories (${Boost_INCLUDE_DIR})

set (LIBRARY_OUTPUT_DIRECTORY ".")
set (SOURCE_FILES
	src/Renderer.cpp
	src/Window.cpp
	src/Texture.cpp
	src/Block.cpp
	src/MessageThread.cpp
	src/Application.cpp
	src/TASView.cpp
	src/Blocks/Time.cpp
	src/Blocks/Clips.cpp
	src/Blocks/Framerate.cpp
	src/Blocks/Water.cpp)

add_executable (TASView ${SOURCE_FILES})
target_link_libraries (TASView ${Boost_LIBRARIES} pthread rt SDL2 SDL2main SDL2_ttf)
