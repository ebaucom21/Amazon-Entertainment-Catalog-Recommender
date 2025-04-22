cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,16777216")

project(Amazon-Entertainment-Catalog-Recommender)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## CHANGE DIRECTORY HERE
##              ||
##              ||
##              \/
set(SFML_DIR "C:/Users/scien/CLionProjects/SMFL seperate/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)
add_executable(Amazon-Entertainment-Catalog-Recommender
        main.cpp
        Balanced-KD.cpp
        Balanced-RB.cpp
        CSV-Extractor.cpp
)
##testing purposes only, uncomment to test
#add_executable(Tester
#        Balanced-KD.cpp
#        CSV-Extractor.cpp
#        KD-Tester.cpp
#)

target_link_libraries(Amazon-Entertainment-Catalog-Recommender sfml-system sfml-window sfml-graphics sfml-audio)
