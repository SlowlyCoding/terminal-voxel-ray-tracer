cmake_minimum_required(VERSION 3.10)
project(Voxel_Ray_Tracer)

# compiler options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-Wall -Wextra -Wpedantic)

file(GLOB sources src/*.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(voxel_ray_tracer ${sources})

# link threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(voxel_ray_tracer PRIVATE Threads::Threads)

# link nlohmann::json
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)
target_link_libraries(voxel_ray_tracer PRIVATE nlohmann_json::nlohmann_json)

# link SFML
find_package(SFML REQUIRED COMPONENTS window)
# include_directories(${SFML_INCLUDE_DIR})
# target_link_libraries(voxel_ray_tracer ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
target_link_libraries(voxel_ray_tracer PRIVATE sfml-graphics)

# enable 'make run' command
add_custom_target(run DEPENDS voxel_ray_tracer COMMAND voxel_ray_tracer
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
