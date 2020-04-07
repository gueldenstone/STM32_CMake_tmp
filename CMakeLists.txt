# ---------------------------- STM32 Project Properties -------------------- #
set(STM32_CHIP STM32F303CC)
set(BINARY_POSTFIX ".elf")
set(CMAKE_BUILD_TYPE Debug)
set(CLONE_DIR "/Users/lukas/Library/Mobile Documents/com~apple~CloudDocs/05_DEV/VSARM/Drivers")
# -------------------------------------------------------------------------- #

# ---------------------------------- Paths --------------------------------- #
set(TOOLCHAIN_PREFIX "/usr/local")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
MESSAGE(STATUS "CMake Module Path is " ${CMAKE_MODULE_PATH})
# -------------------------------------------------------------------------- #

# ------------------------- CMakeLists.txt --------------------------------- #
cmake_minimum_required(VERSION 3.17)
if(${CMAKE_VERSION} VERSION_LESS 3.17)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
# -------------------------------------------------------------------------- #

# ---------------------------- Get STM32 Drivers --------------------------- #
include(cmake/gcc_stm32.cmake)
IF(NOT STM32_FAMILY)
    MESSAGE(STATUS "No STM32_FAMILY specified, trying to get it from STM32_CHIP")
    IF(NOT STM32_CHIP)
        SET(STM32_FAMILY "F1" CACHE INTERNAL "stm32 family")
        MESSAGE(STATUS "Neither STM32_FAMILY nor STM32_CHIP specified, using default STM32_FAMILY: ${STM32_FAMILY}")
    ELSE()
        STRING(REGEX REPLACE "^[sS][tT][mM]32(([fF][0-47])|([hH]7)|([lL][0-14])|([tT])|([wW])).+$" "\\1" STM32_FAMILY ${STM32_CHIP})
        STRING(TOUPPER ${STM32_FAMILY} STM32_FAMILY)
        MESSAGE(STATUS "Selected STM32 family: ${STM32_FAMILY}")
    ENDIF()
ENDIF()
STRING(REGEX REPLACE "^[sS][tT][mM]32([fF][0-47][0-47][0-47]).+$" "\\1" STM32_SVD ${STM32_CHIP})
MESSAGE(STATUS "Selected SVD File: ${STM32_SVD}")
include(cmake/GitUtils.cmake)
git_clone(
    PROJECT_NAME    STM32Cube${STM32_FAMILY}
    GIT_URL         https://github.com/STMicroelectronics/STM32Cube${STM32_FAMILY}.git
    GIT_BRANCH      master
    DIRECTORY       ${CLONE_DIR}
    TO_CLONE_DIRS   "Drivers/"

)
set(STM32Cube_DIR ${CLONE_DIR}/STM32Cube${STM32_FAMILY})
find_package(CMSIS REQUIRED)
find_package(STM32HAL REQUIRED)
git_clone(
    PROJECT_NAME    CMSIS_SVD
    GIT_URL         https://github.com/posborne/cmsis-svd.git
    GIT_BRANCH      master
    DIRECTORY       ${CLONE_DIR}
    TO_CLONE_DIRS   "data/STMicro/"
)
# -------------------------------------------------------------------------- #


# ------------------------------ Project Settings --------------------------- #
get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} VERSION 1.0
                  DESCRIPTION "<description>"
                  LANGUAGES C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
enable_language(ASM)
# -------------------------------------------------------------------------- #

# -------------------------------- Includes -------------------------------- #

set(INCLUDES 
    "inc"
    ${CMSIS_INCLUDE_DIRS}
    ${STM32HAL_INCLUDE_DIR}
)
string(REPLACE ";" "\",\n\"" C_CPP_INCLUDES "${INCLUDES}")


MESSAGE(STATUS "The includes are " ${INCLUDES})
include_directories(${INCLUDES})
# -------------------------------------------------------------------------- #

# ------------------------------ Source Files ------------------------------ #
file(GLOB_RECURSE USER_SOURCES "src/*.c" "src/*.cpp")
# -------------------------------------------------------------------------- #

# --------------------------------- Linker --------------------------------- #
set(STM32_LINKER_SCRIPT ${CMSIS_LINKER_SCRIPT})

# ------------------------- Executables and Targets ------------------------ #
add_executable(${PROJECT_NAME} ${USER_SOURCES} ${CMSIS_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}${BINARY_POSTFIX}")

STM32_SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME})
STM32_ADD_HEX_BIN_TARGETS(${CMAKE_PROJECT_NAME})
STM32_PRINT_SIZE_OF_TARGETS(${CMAKE_PROJECT_NAME})

# -------------------- Configure tasks launch and c/cpp -------------------- #
configure_file (
    "${CMAKE_SOURCE_DIR}/cmake/vscode_templates/c_cpp_properties.json.in"
    "${PROJECT_SOURCE_DIR}/.vscode/c_cpp_properties.json"
)
configure_file (
    "${CMAKE_SOURCE_DIR}/cmake/vscode_templates/launch.json.in"
    "${PROJECT_SOURCE_DIR}/.vscode/launch.json"
)
configure_file (
    "${CMAKE_SOURCE_DIR}/cmake/vscode_templates/tasks.json.in"
    "${PROJECT_SOURCE_DIR}/.vscode/tasks.json"
    @ONLY
)
# -------------------------------------------------------------------------- #
