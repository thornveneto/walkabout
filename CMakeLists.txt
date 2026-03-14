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

add_executable(main src/main.cpp "src/math/Matrix2D.h"
"src/WorldRenderer.h" "src/entities/MovingEntity.h" "src/terrain/Terrain.h" 
"src/terrain/Cell.h" "src/terrain/Wall.h" "src/GameWorld.h" "src/math/LineSegment.h"
"src/effects/Explosion.h" "src/effects/Effect.h" "src/GameWorld.cpp" 
"src/entities/MovingEntity.cpp" "src/entities/Unit.h" "src/entities/Unit.cpp"
"src/entities/Projectile.h" "src/entities/Projectile.cpp" "src/State.h" 
"src/StateMachine.h" "src/interface_components/CellCube.h" 
"src/terrain/Cell.cpp" "src/ui_state/UIState.h" 
"src/ui_state/DummyCommandState.h" "src/ui_state/UnitSelectionState.h" 
"src/ui_state/UnitSelectedState.h" "src/ui_state/UIState.cpp" "src/ui_state/UnitSelectedState.cpp"
"src/ui_state/UnitSelectionState.cpp" "src/terrain/Terrain.cpp"
"src/WorldRenderer.cpp" "src/UI_InputEvent.h" "src/math/XY.h"
"src/interface_components/CellAttackCube.h" "src/terrain/Wall.cpp"
"src/math/IJ.h"  "src/effects/Explosion.cpp" "src/ui_state/UnitCommandExecutionState.h"
"src/ui_state/UnitCommandExecutionState.cpp" "src/interface_components/CellAttackCube.cpp"
"src/interface_components/CellCube.cpp" "src/ui_state/DummyCommandState.cpp" "src/effects/Effect.cpp"
"src/physics/CollisionData.h" "src/physics/ICollideable.h" "src/math/LineSegment.cpp" 
"src/_types.h" "src/EllipseShape.h" "src/interface_components/HUD.h" 
"src/interface_components/HUD.cpp" "src/interface_components/Button.h" "src/math/Rectangle.h"
"src/GameCommand.h" "src/CommandQueue.h" "src/entities/Weapon.h" "src/entities/Weapon.cpp" "src/math/IJ.cpp" 
"src/interface_components/Button.cpp" "src/terrain/TileDrawing.h" "src/ResourceManager.h" "src/interface_components/PlayField.cpp" "src/palette.h")

#added here
if(MSVC)
    target_compile_options(main PRIVATE 
        /W4           # Set Warning Level to 4
        /permissive-  # Enable Standards Conformance (Strict Mode)
        /EHsc         # Standard C++ Exception Handling
    )
endif()

target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main PRIVATE SFML::Graphics)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)