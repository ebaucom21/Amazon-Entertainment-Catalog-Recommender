cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,16777216")

project(Amazon-Entertainment-Catalog-Recommender)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
##                    ||
##                    ||
##CHANGE TARGET HERE  \/
set(SFML_DIR "C:/Users/scien/CLionProjects/SMFL seperate/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)
add_executable(Amazon-Entertainment-Catalog-Recommender
        main.cpp
        Balanced-KD.cpp
        CSV-Extractor.cpp
)
##testing purposes only
add_executable(Tester
        Balanced-KD.cpp
        CSV-Extractor.cpp
        KD-Tester.cpp
)

target_link_libraries(Amazon-Entertainment-Catalog-Recommender sfml-system sfml-window sfml-graphics sfml-audio)
