# 
# External dependencies
# 

find_package(SFML 2.4 REQUIRED COMPONENTS audio graphics network window system)


# 
# Library name and options
# 

# Target name
set(target Core)

# Exit here if required dependencies are not met
message(STATUS "Lib ${target}")


# 
# Sources
# 

# Generate version file
configure_file(Version.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/${META_PROJECT_NAME}-Version.cpp)

set(headers
    IState.hpp
    InputManager.hpp
    Math.hpp
    Math.inl
    Profiler.hpp
    StateManager.hpp
    StateManager.inl
    Util.hpp
    Version.hpp
)

set(sources
    InputManager.cpp
    Profiler.cpp
    StateManager.cpp
    ${META_PROJECT_NAME}-Version.cpp
)


# Group source files
set(header_group "Header Files")
set(source_group "Source Files")
source_group_by_path(${CMAKE_CURRENT_SOURCE_DIR} "\\\\.h$|\\\\.hpp$|\\\\.inl$" 
    ${header_group} ${headers})
source_group_by_path(${CMAKE_CURRENT_SOURCE_DIR}  "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$|\\\\.inl$" 
    ${source_group} ${sources})


# 
# Create library
# 

# Build library
add_library(${target}
    ${sources}
    ${headers}
)

# Create namespaced alias
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})


# 
# Project options
# 

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
    VERSION ${META_VERSION}
    SOVERSION ${META_VERSION_MAJOR}
)


# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
    ${PROJECT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}

    PUBLIC
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${SFML_INCLUDE_DIR}

    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LIBRARIES}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES}

    INTERFACE
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE

    PUBLIC
    $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${target_id}_STATIC_DEFINE>
    ${DEFAULT_COMPILE_DEFINITIONS}

    INTERFACE
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_COMPILE_OPTIONS}

    INTERFACE
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LINKER_OPTIONS}

    INTERFACE
)
