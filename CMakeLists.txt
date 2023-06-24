cmake_minimum_required(VERSION 3.12)
project(MyFLTKApp)

find_package(FLTK REQUIRED)

file(GLOB SOURCES "src/*.cpp")

add_executable(MyApp ${SOURCES})

target_include_directories(MyApp PRIVATE ${FLTK_INCLUDE_DIRS})
target_link_libraries(MyApp PRIVATE ${FLTK_LIBRARIES})