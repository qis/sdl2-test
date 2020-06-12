cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(test VERSION 0.1.0 LANGUAGES CXX)

# Sources
file(GLOB_RECURSE sources src/*.[hc]pp)

# Executable
add_executable(${PROJECT_NAME} ${sources} src/main.manifest)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/main.hpp)
target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Dependencies
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

# Install
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

if(WIN32 AND VCPKG_LIBRARY_LINKAGE STREQUAL "dynamic")
  install(CODE "
    file(GLOB libraries \"$<TARGET_FILE_DIR:${PROJECT_NAME}>/*.dll\")
    file(INSTALL $\{libraries} DESTINATION \"${CMAKE_INSTALL_PREFIX}/bin\")
  ")
endif()
