cmake_minimum_required(VERSION 3.4.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(async VERSION ${PROJECT_VERSION})

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME all_test COMMAND async_test)