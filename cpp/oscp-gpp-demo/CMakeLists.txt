cmake_minimum_required(VERSION 3.11)
project(oscp-gpp-demo LANGUAGES CXX)

# Release build by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)




# Dependencies
find_package(nlohmann_json REQUIRED)
if(nlohmann_json_FOUND)
    message(STATUS "Found nlohmann_json")
endif()

# OpenSSL is required for httplib
find_package(OpenSSL REQUIRED)
if(OpenSSL_FOUND)
    message(STATUS "Found OpenSSL: ${OPENSSL_VERSION}, ${OPENSSL_LIBRARIES}, ${OPENSSL_INCLUDE_DIR}")
endif()

# httplib (only required for the client)
find_package(httplib)
if(httplib_FOUND)
    message(STATUS "Found httplib: ${httplib_VERSION}")
endif()

# stduuid (only required for the client). This library requires C++17 standard
find_package(stduuid)
if(stduuid_FOUND)
    message(STATUS "Found stduuid: ${stduuid_VERSION}")
endif()

find_package(OpenCV)
if(stduuid_FOUND)
    message(STATUS "Found OpenCV: ${stduuid_VERSION}")
endif()

find_package(oscp-gpp REQUIRED)
if(oscp-gpp_FOUND)
    message(STATUS "Found oscp-gpp: ${oscp-gpp_VERSION}")
endif()


add_executable(oscp-gpp-client main_client.cpp)
target_link_libraries(oscp-gpp-client PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(oscp-gpp-client PRIVATE oscp-gpp)
target_link_libraries(oscp-gpp-client PRIVATE ${OPENSSL_LIBRARIES})
target_include_directories(oscp-gpp-client PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(oscp-gpp-client PRIVATE stduuid)
if (HTTPLIB_IS_COMPILED)
    target_link_libraries(oscp-gpp-client PRIVATE httplib::httplib)
endif()
target_link_libraries(oscp-gpp-client PRIVATE ${OpenCV_LIBRARIES})


add_executable(oscp-gpp-server main_server.cpp)
target_link_libraries(oscp-gpp-server PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(oscp-gpp-server PRIVATE oscp-gpp)
target_link_libraries(oscp-gpp-server PRIVATE ${OPENSSL_LIBRARIES})
target_include_directories(oscp-gpp-server PRIVATE ${OPENSSL_INCLUDE_DIR})
if (HTTPLIB_IS_COMPILED)
    target_link_libraries(oscp-gpp-server PRIVATE httplib::httplib)
endif()

install(TARGETS oscp-gpp-client oscp-gpp-server
    RUNTIME DESTINATION bin COMPONENT Runtime
)
