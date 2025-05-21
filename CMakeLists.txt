cmake_minimum_required(VERSION 3.10)
project(ImaginedStudio VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    src/core/ProjectManager.cpp
    src/core/TalentManager.cpp
    src/core/ResourceAllocator.cpp
    src/main.cpp
)

# Add header files
set(HEADERS
    src/core/ProjectManager.hpp
    src/core/TalentManager.hpp
    src/core/ResourceAllocator.hpp
)

# Create library
add_library(imagined_studio STATIC ${SOURCES} ${HEADERS})

# Create executable
add_executable(imagined_studio_demo src/main.cpp)
target_link_libraries(imagined_studio_demo imagined_studio)

# Include directories
target_include_directories(imagined_studio
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core
)

target_include_directories(imagined_studio_demo
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add tests
enable_testing()
add_subdirectory(tests)

# Installation
install(TARGETS imagined_studio imagined_studio_demo
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY src/
    DESTINATION include/imagined_studio
    FILES_MATCHING PATTERN "*.hpp"
) 