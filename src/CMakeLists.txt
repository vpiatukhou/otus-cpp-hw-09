add_library(async SHARED
                  async.cpp 
                  AsyncCommandWriter.cpp
                  CommandProcessingFacade.cpp
                  CommandProcessor.cpp 
                  ConsoleCommandWriter.cpp 
                  FileCommandWriter.cpp 
                  Util.cpp)
add_executable(async_cli main.cpp)

#rename the executable to "async"
set_target_properties(async_cli PROPERTIES OUTPUT_NAME async PUBLIC_HEADER async.h)

find_package(Threads REQUIRED)
target_link_libraries(async_cli PRIVATE async Threads::Threads)

#BEGIN release
install(TARGETS async_cli async)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
#END
