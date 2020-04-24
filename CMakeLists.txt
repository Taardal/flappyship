cmake_minimum_required(VERSION 3.15)
project(flappyship)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/main.cpp src/GameLayer.cpp src/GameLayer.h src/Player.cpp src/Player.h src/Level.cpp src/Level.h src/SpikeManager.cpp src/SpikeManager.h src/ParticleEmitter.cpp src/ParticleEmitter.h)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(RES_DIR ${PROJECT_SOURCE_DIR}/res)

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})

set(STORYTIME_NAME storytime)
set(STORYTIME_DIR ${LIB_DIR}/storytime)
add_subdirectory(${STORYTIME_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${STORYTIME_DIR}/include)
target_link_libraries(${PROJECT_NAME} ${STORYTIME_NAME})

file(REMOVE_RECURSE ${PROJECT_BINARY_DIR}/res)
file(COPY ${RES_DIR} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${STORYTIME_DIR}/res DESTINATION ${PROJECT_BINARY_DIR})