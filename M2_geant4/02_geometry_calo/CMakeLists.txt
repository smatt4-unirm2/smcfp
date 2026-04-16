cmake_minimum_required(VERSION 3.16)
project(CaloCourseGeometry)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(calo main.cc ${sources} ${headers})
target_link_libraries(calo ${Geant4_LIBRARIES})

file(COPY macro DESTINATION ${PROJECT_BINARY_DIR})
