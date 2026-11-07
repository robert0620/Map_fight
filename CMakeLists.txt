cmake_minimum_required(VERSION 3.28)

set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}_main")
set(MAIN_LIBRARY_NAME "${MAIN_PROJECT_NAME}")
set(TESTS_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}_test")

option(CMAKE_EXPORT_COMPILE_COMMANDS "Export compilation database" ON)

project(${MAIN_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

include(CTest)
include(cmake/RegisterTests.cmake)

include(FetchContent)

set(FETCHCONTENT_QUIET OFF CACHE INTERNAL "Override verbose output" FORCE)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE INTERNAL "Override update strategy" FORCE)

FetchContent_Declare(
        fmt
        SYSTEM
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        11.1.2
        GIT_SHALLOW    1
        GIT_PROGRESS   ON
)


FetchContent_Declare(
        nlohmann_json
        SYSTEM
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.3
        GIT_SHALLOW    1
        GIT_PROGRESS   ON
)

if(MSVC AND USE_ASAN)
    add_compile_options("$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:/fsanitize=address>")
    add_link_options("$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:/fsanitize=address>")
endif()

FetchContent_MakeAvailable(fmt nlohmann_json)

add_library(${MAIN_LIBRARY_NAME} STATIC
        include/Punct2D.h
        include/Vreme.h
        include/Unitate.h
        include/Armata.h
        include/Harta.h
        include/Joc.h
        src/Vreme.cpp
        src/Unitate.cpp
        src/Armata.cpp
        src/Harta.cpp
        src/Joc.cpp
        include/Mag.h
        src/Mag.cpp
        include/Tehnologie.h
        include/ArboreTehnologic.h
        src/ArboreTehnologic.cpp
        src/Tehnologie.cpp
)

add_executable(${MAIN_EXECUTABLE_NAME}
        app/main.cpp
)

set_compiler_flags(RUN_SANITIZERS ${USE_ASAN} TARGET_NAMES ${MAIN_EXECUTABLE_NAME} ${MAIN_LIBRARY_NAME})

target_include_directories(${MAIN_LIBRARY_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_include_directories(${MAIN_LIBRARY_NAME} PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_include_directories(${MAIN_LIBRARY_NAME} SYSTEM PUBLIC
        ${EXT_PUBLIC_DIR}
)
target_include_directories(${MAIN_LIBRARY_NAME} SYSTEM PRIVATE
        ${EXT_PRIVATE_DIR}
)

target_link_libraries(${MAIN_LIBRARY_NAME} PUBLIC fmt::fmt nlohmann_json::nlohmann_json)
target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE ${MAIN_LIBRARY_NAME})

install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
install(TARGETS ${MAIN_LIBRARY_NAME} ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION include
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
)
if(APPLE)
    install(FILES ${CMAKE_SOURCE_DIR}/launcher.command DESTINATION ${DESTINATION_DIR})
endif()

copy_files(FILES tastatura.txt DIRECTORY assets COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()