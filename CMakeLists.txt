cmake_minimum_required(VERSION 3.18)
project(cppnology VERSION 0.1.0)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")

# include(CTest)
# enable_testing()

# Dependencies
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ext/cpr")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ext/fmt")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ext/CxxUrl")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ext/json")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ext/refl-cpp")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ext/magic_enum")

set(CPPNOLOGY_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cppnology/exception/CppnologyException.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cppnology/QuickConnect.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cppnology/Attributes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cppnology/CprSerializer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cppnology/Request.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cppnology/Instance.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cppnology/API/Auth/login.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cppnology/API/Info/query.h"
)

set(CPPNOLOGY_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/CppnologyException.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/QuickConnect.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Instance.cpp"
)

set(CPPNOLOGY_INTERNAL_HEADERS
    # currently none, but they should be placed in src/cppnology_internal
)

include(CMakePackageConfigHelpers)

add_library(cppnology ${CPPNOLOGY_HEADERS} ${CPPNOLOGY_INTERNAL_HEADERS} ${CPPNOLOGY_SRC})
add_library(cppnology::cppnology ALIAS cppnology)

set_target_properties(cppnology PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${CPPNOLOGY_HEADERS}"
)

target_include_directories(cppnology PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ext/CxxUrl")
target_include_directories(cppnology PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(cppnology PUBLIC refl-cpp::refl-cpp magic_enum::magic_enum cpr::cpr fmt::fmt nlohmann_json::nlohmann_json CxxUrl)


install(TARGETS cppnology
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY include/cppnology DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
