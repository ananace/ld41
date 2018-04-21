# 
# External dependencies
# 

# find_package(THIRDPARTY REQUIRED)


# 
# Sources
# 

set(headers
    Application.hpp
    Inputs.hpp

    Asteroids/Asteroid.hpp
    Asteroids/Game.hpp
    Asteroids/Level.hpp
    Asteroids/Player.hpp

    Minesweeper/Game.hpp
    Minesweeper/Level.hpp

    States/IntroState.hpp
    States/MenuState.hpp
    States/GameState.hpp
)

set(sources
    Application.cpp

    Asteroids/Asteroid.cpp
    Asteroids/Game.cpp
    Asteroids/Level.cpp
    Asteroids/Player.cpp

    Minesweeper/Game.hpp
    Minesweeper/Level.cpp

    States/IntroState.cpp
    States/MenuState.cpp
    States/GameState.cpp

    main.cpp
)

# Group source files
set(header_group "Header Files")
set(source_group "Source Files")
source_group_by_path(${CMAKE_CURRENT_SOURCE_DIR} "\\\\.h$|\\\\.hpp$|\\\\.inl$" 
    ${header_group} ${headers})
source_group_by_path(${CMAKE_CURRENT_SOURCE_DIR}  "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$|\\\\.inl$" 
    ${source_group} ${sources})

# 
# Create executable
# 

# Build executable
add_executable(${META_PROJECT_NAME}
    MACOSX_BUNDLE
    ${sources}
    ${headers}
)


# 
# Project options
# 

set_target_properties(${META_PROJECT_NAME}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
)


# 
# Include directories
# 

target_include_directories(${META_PROJECT_NAME}
    PRIVATE
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${CMAKE_CURRENT_SOURCE_DIR}
)


# 
# Libraries
# 

target_link_libraries(${META_PROJECT_NAME}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    ${META_PROJECT_NAME}::Core
)


# 
# Compile definitions
# 

target_compile_definitions(${META_PROJECT_NAME}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
)


# 
# Compile options
# 

target_compile_options(${META_PROJECT_NAME}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)


# 
# Linker options
# 

target_link_libraries(${META_PROJECT_NAME}
    PRIVATE
    ${DEFAULT_LINKER_OPTIONS}
)
