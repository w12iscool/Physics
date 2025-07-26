cmake_minimum_required(VERSION 3.31)
project(Physics)

set(CMAKE_CXX_STANDARD 26)

add_executable(Physics src/main.cpp
        src/Box.cpp
        src/Box.h
        src/GameEngine.cpp
        src/GameEngine.h
        src/Constants.h
        src/Ball.cpp
        src/Ball.h
        src/Item.cpp
        src/Item.h
        src/spear.cpp
        src/spear.h
        src/Random.h
        src/RaylibTimer.h
        src/Sword.cpp
        src/Sword.h
        src/Dagger.cpp
        src/Dagger.h
)

find_package(box2d REQUIRED)
target_link_libraries(Physics PRIVATE box2d::box2d)
find_package(glfw3 REQUIRED)
target_link_libraries(Physics PRIVATE glfw)
find_package(NanoSVG REQUIRED)
target_link_libraries(Physics PRIVATE NanoSVG::nanosvg NanoSVG::nanosvgrast)
find_package(raylib REQUIRED)
target_link_libraries(Physics PRIVATE raylib)
find_package(Stb REQUIRED)
target_include_directories(Physics PRIVATE ${Stb_INCLUDE_DIR})


