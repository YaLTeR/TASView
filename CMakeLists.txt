cmake_minimum_required (VERSION 3.1)
project (TASView)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

if (NOT MSVC)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -mtune=native -Ofast -Wall -Wextra -Wno-deprecated-declarations -Wno-unused-parameter")
endif ()

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
set (Boost_USE_STATIC_LIBS ON)
find_package (Boost 1.57.0 REQUIRED)
include_directories (
	${Boost_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR}
	${SDL2_TTF_INCLUDE_DIR}
)

set (LIBRARY_OUTPUT_DIRECTORY ".")
set (WINDOWS_FILES
	src/platform/windows/IPC.cpp
	src/platform/windows/MessageQueue.cpp
)
set (POSIX_FILES
	src/platform/posix/IPC.cpp
)
set (SOURCE_FILES
	src/Renderer.cpp
	src/Window.cpp
	src/Texture.cpp
	src/Block.cpp
	src/Application.cpp
	src/TASView.cpp
	src/Blocks/Time.cpp
	src/Blocks/Clips.cpp
	src/Blocks/Framerate.cpp
	src/Blocks/Water.cpp
	src/Blocks/Remainder.cpp
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set (SOURCE_FILES ${SOURCE_FILES} ${WINDOWS_FILES})
else ()
	set (SOURCE_FILES ${SOURCE_FILES} ${POSIX_FILES})
endif ()

add_executable (TASView ${SOURCE_FILES})
target_link_libraries (TASView
	${SDL2_LIBRARY}
	${SDL2_TTF_LIBRARY}
	${Boost_LIBRARIES}
)

if (MSVC)
	set_target_properties (TASView PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
else ()
	target_link_libraries (TASView pthread rt)
endif ()
