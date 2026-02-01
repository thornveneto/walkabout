cmake_minimum_required(VERSION 3.28)
project(walkabout LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp "src/Matrix2D.h" "src/WorldRenderer.h" "src/MovingEntity.h" "src/Terrain.h" "src/Cell.h" "src/Wall.h" "src/GameWorld.h" "src/LineSegment.h" "src/Explosion.h" "src/Effect.h" "src/GameWorld.cpp" "src/MovingEntity.cpp" "src/Unit.h" "src/Unit.cpp" "src/Projectile.h" "src/Projectile.cpp" "src/State.h" "src/StateMachine.h" "src/CellCube.h" "src/Cell.cpp" "src/UIState.h" "src/DummyCommandState.h" "src/UnitCommandSelectionState.h" "src/UnitCommandExecutionState.h" "src/UIState.cpp" "src/UnitCommandExecutionState.cpp" "src/UnitCommandSelectionState.cpp" "src/Terrain.cpp" "src/WorldRenderer.cpp" "src/UI_InputEvent.h" "src/XY.h" "src/CellAttackCube.h" "src/Wall.cpp" "src/IJ.h"  "src/Explosion.cpp")
target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main PRIVATE SFML::Graphics)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)