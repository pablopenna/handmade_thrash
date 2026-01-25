cmake_minimum_required(VERSION 3.16)
project(handmade_thrash)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Add debug symbols
set(CMAKE_BUILD_TYPE Debug)

# Generate compile_commands.json file so that IDEs like Zed can properly detect SDL libraries
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Define files to compile
file(GLOB SOURCES "src/*.cpp" "src/**/*.cpp")
file(GLOB HEADERS "src/*.h" "src/**/*.h")

# Create your game executable target as usual
add_executable(handmade_thrash WIN32
    ${SOURCES}
    ${HEADERS}
)

# Link to the actual SDL library.
target_link_libraries(handmade_thrash PRIVATE SDL2::SDL2)
