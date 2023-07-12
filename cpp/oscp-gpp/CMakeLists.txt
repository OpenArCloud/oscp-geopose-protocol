cmake_minimum_required(VERSION 3.11)
project(oscp-gpp LANGUAGES CXX)

# Release build by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON) # exceptions are used


# Sources
file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/oscp/*.h)
add_library(oscp-gpp ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Dependencies
find_package(nlohmann_json REQUIRED)
if(nlohmann_json_FOUND)
    message(STATUS "Found nlohmann_json")
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)

# Install
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/oscp-gppConfigVersion.cmake"
    VERSION 0.1
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/oscp-gppConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/oscp-gppConfig.cmake"
    INSTALL_DESTINATION lib/cmake/oscp-gpp
)
install(EXPORT ${PROJECT_NAME}Targets DESTINATION lib/cmake/oscp-gpp)
install(FILES "${PROJECT_BINARY_DIR}/oscp-gppConfigVersion.cmake"
    "${PROJECT_BINARY_DIR}/oscp-gppConfig.cmake"
    DESTINATION lib/cmake/oscp-gpp)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib COMPONENT Runtime
    ARCHIVE DESTINATION lib COMPONENT Development
    RUNTIME DESTINATION bin COMPONENT Runtime
    PUBLIC_HEADER DESTINATION include COMPONENT Development
    BUNDLE DESTINATION bin COMPONENT Runtime
)
