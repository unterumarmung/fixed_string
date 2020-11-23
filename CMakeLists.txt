cmake_minimum_required(VERSION 3.16)
project(fixed_string VERSION "0.0.1" LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(IS_TOPLEVEL_PROJECT TRUE)
else()
    set(IS_TOPLEVEL_PROJECT FALSE)
endif()

option(FIXED_STRING_OPT_BUILD_EXAMPLES "Build fixed_string examples") #${IS_TOPLEVEL_PROJECT})
option(FIXED_STRING_OPT_BUILD_TESTS "Build and perform fixed_string tests" ${IS_TOPLEVEL_PROJECT})
option(FIXED_STRING_OPT_INSTALL "Generate and install fixed_string target" ${IS_TOPLEVEL_PROJECT})

if(FIXED_STRING_OPT_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(FIXED_STRING_OPT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

include(CMakePackageConfigHelpers)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

write_basic_package_version_file(${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion)

if(FIXED_STRING_OPT_INSTALL)
    install(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Config)

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
            DESTINATION lib/cmake/${PROJECT_NAME})

    install(EXPORT ${PROJECT_NAME}Config
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION lib/cmake/${PROJECT_NAME})

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
            DESTINATION .)

    export(EXPORT ${PROJECT_NAME}Config
            NAMESPACE ${PROJECT_NAME}::)
endif()
