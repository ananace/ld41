# 
# External dependencies
# 

# find_package(THIRDPARTY REQUIRED)


# 
# Sources
# 

set(sources
    Application.hpp
    Application.cpp

    main.cpp
)


# 
# Create executable
# 

# Build executable
add_executable(${META_PROJECT_NAME}
    MACOSX_BUNDLE
    ${sources}
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
    ${PROJECT_BINARY_DIR}/source/include
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
