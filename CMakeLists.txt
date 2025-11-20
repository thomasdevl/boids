cmake_minimum_required(VERSION 3.10)
project(ant_war_direct)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Find SDL2_gfx
find_path(SDL2_GFX_INCLUDE_DIR SDL2/SDL2_gfxPrimitives.h)
find_library(SDL2_GFX_LIBRARY NAMES SDL2_gfx)

include_directories(${SDL2_GFX_INCLUDE_DIR})

add_executable(ant_war_direct src/main.cxx)

# Link SDL2 + SDL2_gfx
target_link_libraries(ant_war_direct ${SDL2_LIBRARIES} ${SDL2_GFX_LIBRARY})
