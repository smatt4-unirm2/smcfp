cmake_minimum_required(VERSION 3.16)
project(CaloCourseScoring)

find_package(Geant4 REQUIRED ui_all vis_all)
find_package(ROOT REQUIRED COMPONENTS Core RIO Tree)

include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${ROOT_INCLUDE_DIRS})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(calo main.cc ${sources} ${headers})
target_link_libraries(calo ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

file(COPY macro DESTINATION ${PROJECT_BINARY_DIR})
