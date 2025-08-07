cmake_minimum_required(VERSION 3.31)
project(Physics)

set(CMAKE_CXX_STANDARD 26)

file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

add_executable(Physics src/main.cpp
        src/Box.cpp
        src/Box.h
        src/GameEngine.cpp
        src/GameEngine.h
        src/Constants.h
        "src/Ball.cpp"
        "src/Ball.h"
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
        src/Weapon.h
        src/Shield.cpp
        src/Shield.h
        src/Bow.cpp
        src/Bow.h
        src/Scythe.cpp
        src/Scythe.h
        src/Mace.cpp
        src/Mace.h
        src/Staff.cpp
        src/Staff.h
        src/raylib-nuklear.h
        src/gui_ballEditor.h
        src/Wrench.cpp
        src/Wrench.h
        src/Shuriken.cpp
        src/Shuriken.h
)

#find_package(box2d REQUIRED)
#target_link_libraries(Physics PRIVATE box2d::box2d)
add_subdirectory(third_party/box2d)  # Add Box2D source build here
target_link_libraries(Physics PRIVATE box2d)

#find_package(glfw3 REQUIRED)
#target_link_libraries(Physics PRIVATE glfw)
#set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_WIN32 OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#add_subdirectory(third_party/glfw3)
#target_link_libraries(Physics PRIVATE glfw3)

#find_package(NanoSVG REQUIRED)
#target_link_libraries(Physics PRIVATE NanoSVG::nanosvg NanoSVG::nanosvgrast)
#add_subdirectory(third_party/NanoSVG)
#target_link_libraries(Physics PRIVATE NanoSVG)

#find_package(raylib REQUIRED)
#target_link_libraries(Physics PRIVATE raylib)
if(EMSCRIPTEN)
    # For Emscripten
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(PLATFORM_EMSCRIPTEN ON CACHE BOOL "" FORCE)
    set(PLATFORM_DESKTOP OFF CACHE BOOL "" FORCE)
    set(PLATFORM_RPI OFF CACHE BOOL "" FORCE)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1 --preload-file ${CMAKE_SOURCE_DIR}/resources@resources")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.

    add_subdirectory(third_party/raylib)
    target_link_libraries(Physics PRIVATE raylib)

    # Do NOT find or link glfw or other desktop libs here

else()
    find_package(raylib REQUIRED)
    target_link_libraries(Physics PRIVATE raylib)
endif()

#add_subdirectory(third_party/stb)
#target_include_directories(Physics PRIVATE ${CMAKE_SOURCE_DIR}/third_party/stb)

#add_subdirectory(third_party/raygui)
target_include_directories(Physics PRIVATE ${CMAKE_SOURCE_DIR}/third_party/raygui/src)

if(EMSCRIPTEN)
    # Output directory for the web build
    set(WEB_DIST_DIR ${CMAKE_BINARY_DIR}/web_dist)
    file(MAKE_DIRECTORY ${WEB_DIST_DIR})

    # Copy Physics.js, Physics.wasm
    add_custom_command(TARGET Physics POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/Physics.js ${WEB_DIST_DIR}/Physics.js
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/Physics.wasm ${WEB_DIST_DIR}/Physics.wasm
    )

    # Copy resources folder
    add_custom_command(TARGET Physics POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${WEB_DIST_DIR}/resources
    )

    add_custom_command(TARGET Physics POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/Physics.js ${WEB_DIST_DIR}/Physics.js
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/Physics.wasm ${WEB_DIST_DIR}/Physics.wasm
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/Physics.data ${WEB_DIST_DIR}/Physics.data
    )

    # Generate index.html
    file(WRITE ${WEB_DIST_DIR}/index.html
            "<!DOCTYPE html>\n"
            "<html lang=\"en\">\n"
            "<head>\n"
            "  <meta charset=\"UTF-8\">\n"
            "  <title>Physics Game</title>\n"
            "  <style>\n"
            "    body {\n"
            "      margin: 0;\n"
            "      background: #000;\n"
            "      display: flex;\n"
            "      justify-content: center;\n"
            "      align-items: center;\n"
            "      height: 100vh;\n"
            "    }\n"
            "    canvas {\n"
            "      max-width: 100%;\n"
            "      max-height: 100%;\n"
            "      width: auto;\n"
            "      height: auto;\n"
            "      display: block;\n"
            "    }\n"
            "  </style>\n"
            "</head>\n"
            "<body>\n"
            "  <canvas id=\"canvas\" oncontextmenu=\"event.preventDefault()\" tabindex=\"0\"></canvas>\n"
            "  <script>\n"
            "    var Module = {\n"
            "      canvas: (() => document.getElementById('canvas'))()\n"
            "    };\n"
            "  </script>\n"
            "  <script src=\"Physics.js\"></script>\n"
            "</body>\n"
            "</html>\n"
    )
endif()



