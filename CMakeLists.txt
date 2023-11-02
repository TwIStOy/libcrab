cmake_minimum_required(VERSION 3.26)
project(crab CXX)

if(NOT DEFINED CRAB_MASTER_PROJECT)
    set(CRAB_MASTER_PROJECT OFF)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(CRAB_MASTER_PROJECT ON)
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(PEDANTIC_COMPILE_FLAGS
        -Wformat=2
        -Wstrict-aliasing
        -Wall
        -Wextra
        -Wtrigraphs
        -Wuninitialized
        -Wwrite-strings
        -Wpointer-arith
        -Wredundant-decls
        -Wunused
        -Wmissing-include-dirs
        -Wno-missing-field-initializers
    )

    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.6)
        set(PEDANTIC_COMPILE_FLAGS
            ${PEDANTIC_COMPILE_FLAGS}
            -Wno-dangling-else
            -Wno-unused-local-typedefs
        )
    endif()
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
        set(PEDANTIC_COMPILE_FLAGS
            ${PEDANTIC_COMPILE_FLAGS}
            -Wdouble-promotion
            -Wtrampolines
            -Wzero-as-null-pointer-constant
            -Wuseless-cast
            -Wvector-operation-performance
            -Wsized-deallocation
            -Wshadow
        )
    endif()
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0)
        set(PEDANTIC_COMPILE_FLAGS
            ${PEDANTIC_COMPILE_FLAGS}
            -Wshift-overflow=2
            -Wnull-dereference
            -Wduplicated-cond
        )
    endif()

    set(WERROR_FLAG -Werror)
endif()

add_library(crab)
add_library(crab::crab ALIAS crab)

target_compile_features(crab PUBLIC cxx_std_20)
set_target_properties(crab PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(crab PUBLIC ${PEDANTIC_COMPILE_FLAGS} ${WERROR_FLAG})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(crab PUBLIC -fcoroutines)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(crab PUBLIC -fcoroutines-ts)
endif()

target_include_directories(
    crab
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

file(GLOB_RECURSE GRAB_SOURCES CONFIGURE_DEPENDS src/*.cc)

target_sources(crab PRIVATE ${GRAB_SOURCES})
