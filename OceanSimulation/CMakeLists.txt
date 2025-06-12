cmake_minimum_required(VERSION 3.16)
project(LiveOcean VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC     CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.hpp)

add_executable(ocean ${SRC} ${HEADERS})
target_include_directories(ocean PRIVATE include)

# ─── SFML 3  →  fallback 2.x ─────────────────────────────────────────
find_package(SFML 3 QUIET COMPONENTS Graphics Window System)
if (NOT SFML_FOUND)
    message(STATUS "SFML-3 не найден, пробуем 2.x …")
    find_package(SFML 2 REQUIRED COMPONENTS graphics window system)
    target_link_libraries(ocean PRIVATE
        sfml-graphics sfml-window sfml-system)
else()
    target_link_libraries(ocean PRIVATE
        SFML::Graphics SFML::Window SFML::System)
endif()

# ─── macOS: добавление системных фреймворков ───────────────────────────
if (APPLE)
    target_link_libraries(ocean PRIVATE "-framework Cocoa" "-framework IOKit")
endif()
