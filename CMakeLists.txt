# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.28.3)

# Define variables for the project
set(MY_PROJECT_NAME "Velocirate")
set(MY_PROJECT_BIN_NAME "velocirate")
set(MY_PROJECT_VERSION "888.0")

# Define the project name and version
project(${MY_PROJECT_BIN_NAME} VERSION ${MY_PROJECT_VERSION} LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Ensure GCC version 11 or higher is used
find_program(CXX_COMPILER NAMES g++-11 g++-12 g++-13 g++-14 PATHS /usr/bin /usr/local/bin)

# Check if the compiler is found
if(NOT CXX_COMPILER)
    message(FATAL_ERROR "GCC 11.0 or higher is required")
endif()

# Extract the version number using the compiler output
execute_process(
    COMMAND ${CXX_COMPILER} --version
    OUTPUT_VARIABLE GCC_VERSION_OUTPUT
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Extract version number
string(REGEX MATCH "([0-9]+\\.[0-9]+)" GCC_VERSION ${GCC_VERSION_OUTPUT})

# Ensure that the version > than:
if(GCC_VERSION VERSION_LESS "11.0.0")
    message(FATAL_ERROR "GCC 11.0 or higher is required, found version: ${GCC_VERSION}")
endif()

# Set the C++ compiler explicitly
set(CMAKE_CXX_COMPILER ${CXX_COMPILER})


# COMPILER FLAGS START:

# Build Type Configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()

# Compiler Flags per Build Type
# Basic for all build modes
set(COMPILER_FLAGS_BASE "-Wall -Wextra -Wpedantic -fPIC -pthread")
set(COMPILER_FLAGS_DEBUG "-O0 -g")
# "-O3"	Final builds where maximum performance is required..
set(COMPILER_FLAGS_RELEASE "-O3")
# "-O2 -g" Builds requiring both reasonable optimization and debugging capability.
set(COMPILER_FLAGS_RELWITHDEBINFO "-O2 -g")
# "-Os"	Builds targeting size-constrained environments or improving memory usage.
set(COMPILER_FLAGS_MINSIZEREL "-Os")

# Apply Compiler Flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS_BASE} ${COMPILER_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS_BASE} ${COMPILER_FLAGS_RELEASE}")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS_BASE} ${COMPILER_FLAGS_RELWITHDEBINFO}")
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS_BASE} ${COMPILER_FLAGS_MINSIZEREL}")
endif()


# GERNERAL conditional flags: ...

# Option to Enable Dependency Generation
option(ENABLE_DEPENDENCY_GENERATION "Enable dependency generation compile flags" OFF)
if(ENABLE_DEPENDENCY_GENERATION)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MMD -MP")
endif()

# Option to Enable Sanitizers
option(ENABLE_SANITIZERS "Enable sanitizer compile flags for runtime debugging" ON)
if(ENABLE_SANITIZERS)
    set(SANITIZER_FLAGS "-fsanitize=undefined,shift,integer-divide-by-zero,bounds,float-divide-by-zero -fsanitize=shift -fsanitize=shift-exponent -fsanitize=shift-base -fsanitize=integer-divide-by-zero -fsanitize=vla-bound \
  -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=bounds-strict -fsanitize=alignment -fsanitize=object-size \
  -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fsanitize=nonnull-attribute -fsanitize=returns-nonnull-attribute -fsanitize=bool -fsanitize=enum \
  -fsanitize=vptr -fsanitize=pointer-overflow -fsanitize=builtin   ") # -fsanitize=thread  -fsanitize=leak -fsanitize=pointer-compare  -fsanitize=address
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")
endif()


# DEBUG conditional flags: ...

# SET ON / OFF - Conditionally enable CMAKE_EXPORT_COMPILE_COMMANDS based on the option
option(ENABLE_COMPILE_COMMANDS "Enable generation of compile_commands.json" ON)
if(ENABLE_COMPILE_COMMANDS)
    set(CMAKE_EXPORT_COMPILE_COMMANDS YES CACHE BOOL "Export compile commands" FORCE)
endif()

# SET ON / OFF - Enable verbose makefile output
option(ENABLE_VERBOSE_MAKEFILES "Enable verbose makefiles" ON)
if(ENABLE_VERBOSE_MAKEFILES)
    set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "Enable verbose makefile output" FORCE)
endif()

# SET ON / OFF-  Option to Treat Warnings as Errors
option(ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
if(ENABLE_WARNINGS_AS_ERRORS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()


# SET ON / OFF clang-tidy (e.g apt clang-tidy) (easily complains, leave off unless needed)
option(ENABLE_CLANG_TIDY "Run clang-tidy during compilation" OFF)
if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)
    if(CLANG_TIDY_EXE)
       # set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
       # set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};--checks=-*,modernize-*,-clang-analyzer-cplusplus.NewDeleteLeaks")
    else()
        message(WARNING "clang-tidy requested but not found!")
    endif()
endif()

# Pass preprocessor definitions for conditional logging in the code.
option(ENABLE_LOGGING "Enable detailed logging" ON)
if(ENABLE_LOGGING)
    add_definitions(-DENABLE_LOGGING)
else()
    add_definitions(-DDISABLE_LOGGING)
endif()

# Include testing libraries or frameworks
#option(ENABLE_TESTS "Build and run tests" OFF)
#if(ENABLE_TESTS)
#    enable_testing()
#    add_subdirectory(tests) # Assuming a 'tests' subdirectory exists
#endif()


# COMPILER FLAGS END


# Library flags (from your original Makefile)
set(LIBS cpprest boost_system sqlite3 z ssl crypto pthread)

# Define directories
set(SRC_DIR src)
set(BUILD_DIR ${CMAKE_BINARY_DIR}/build)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)
set(DATABASE_DIR ${SRC_DIR}/database)

# Define source files (find all .cpp files in src and subdirectories)
file(GLOB_RECURSE SRCS ${SRC_DIR}/*.cpp)  # GLOB_RECURSE only matches files in the current directory (depth 1)
# file(GLOB_RECURSE SRCS ${SRC_DIR}/**/*.cpp)  # GLOB_RECURSE with '**' matches .cpp files at any depth of subdirectories

# Define include directories
include_directories(/usr/include ${DATABASE_DIR})

# Add the executable
add_executable(${MY_PROJECT_BIN_NAME} ${SRCS})

# Specify the output directory for the executable
set_target_properties(${MY_PROJECT_BIN_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}  # Ensure it outputs to the root bin directory
)

# Link external libraries (from your original Makefile)
target_link_libraries(${MY_PROJECT_BIN_NAME} ${LIBS})

# Create necessary directories for the build
file(MAKE_DIRECTORY ${BUILD_DIR})
file(MAKE_DIRECTORY ${BIN_DIR})  # Create bin in the project root if it doesn't exist

# Define custom targets for cleaning and running
add_custom_target(my_clean
    COMMAND rm -rf ${BUILD_DIR} ${BIN_DIR}
    COMMENT "Clean build and bin directories"
)

add_custom_target(run
    COMMAND ${BIN_DIR}/${MY_PROJECT_BIN_NAME}
    DEPENDS edgar_fetch
    COMMENT "Run the program"
)

# Make the custom targets available to the user
add_dependencies(run ${MY_PROJECT_BIN_NAME})


# Summary of Configuration Settings
message(STATUS "------------------------------------------------------------")
message(STATUS "Project: ${MY_PROJECT_BIN_NAME} (Version: ${MY_PROJECT_VERSION})")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
endif()

message(STATUS "CMake generator: ${CMAKE_GENERATOR}")
message(STATUS "Compiler: ${CXX_COMPILER}")
message(STATUS "Compiler Flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Dependency Generation: ${ENABLE_DEPENDENCY_GENERATION}")
message(STATUS "Enable sanitizer compile flags for runtime debugging: ${ENABLE_SANITIZERS}")
message(STATUS "Warnings as Errors: ${ENABLE_WARNINGS_AS_ERRORS}")
message(STATUS "Verbose Makefiles: ${ENABLE_VERBOSE_MAKEFILES}")
message(STATUS "Compile Commands Export: ${ENABLE_COMPILE_COMMANDS}")
message(STATUS "------------------------------------------------------------")


