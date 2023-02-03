set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_executable(async_cli main.cpp)
add_library(async SHARED
                  async.cpp 
                  CommandProcessor.cpp 
                  CommandQueue.cpp 
                  ConsoleCommandWriter.cpp 
                  BaseCommandWriter.cpp 
                  FileCommandWriter.cpp)

#rename the executable to "async"
set_target_properties(async_cli PROPERTIES OUTPUT_NAME async)

target_link_libraries(async_cli PRIVATE async)

#BEGIN release
install(TARGETS async RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
#END
