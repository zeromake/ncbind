
cmake_minimum_required(VERSION 3.12)
project(ncbind_krkrsdl2)

set(PLUGIN_NAME testbind)
set(PLUGIN_DEPS ncbind)

if((NOT DEFINED CMAKE_BUILD_TYPE) OR (NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL ""))
if(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
set(CMAKE_BUILD_TYPE MinSizeRel)
else()
set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()
endif()

set(PLUGIN_SRC testbind.cpp)
set(PLUGIN_INCLUDES include)

include(FetchContent)

FetchContent_Declare(
	krkrsdl2-tp_stub
	URL https://github.com/krkrsdl2/krkrsdl2/releases/download/latest/krkrsdl2-tp_stub.tar.gz
)

FetchContent_MakeAvailable(krkrsdl2-tp_stub)

add_library(ncbind STATIC)
set_target_properties(ncbind PROPERTIES CXX_STANDARD 11)
set_target_properties(ncbind PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_sources(ncbind PRIVATE ncbind.cpp)
target_link_libraries(ncbind tp_stub)
target_include_directories(ncbind PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} include)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
target_compile_options(ncbind PRIVATE
	-sDISABLE_EXCEPTION_CATCHING=0
	-fPIC
)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
add_executable(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES SUFFIX ".so")
else()
add_library(${PLUGIN_NAME} MODULE)
endif()
set_target_properties(${PLUGIN_NAME} PROPERTIES CXX_STANDARD 11)
set_target_properties(${PLUGIN_NAME} PROPERTIES PREFIX "")
target_sources(${PLUGIN_NAME} PRIVATE ${PLUGIN_SRC})
target_include_directories(${PLUGIN_NAME} PRIVATE ${PLUGIN_INCLUDES})
target_link_libraries(${PLUGIN_NAME} ${PLUGIN_DEPS})
if(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
target_link_options(${PLUGIN_NAME} PRIVATE
	-sDISABLE_EXCEPTION_CATCHING=0
	-fPIC
	-sSIDE_MODULE=1
)
target_compile_options(${PLUGIN_NAME} PRIVATE
	-sDISABLE_EXCEPTION_CATCHING=0
	-fPIC
)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
target_link_options(${PLUGIN_NAME} PRIVATE
	-O3
)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
target_link_options(${PLUGIN_NAME} PRIVATE
	-O2
)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
target_link_options(${PLUGIN_NAME} PRIVATE
	-Os
)
endif()
endif()

if(FALSE)
cmake_minimum_required(VERSION 3.16)

# UTF-8 source
add_compile_options("$<$<AND:$<C_COMPILER_ID:MSVC>,$<COMPILE_LANGUAGE:C>>:/utf-8>")
add_compile_options("$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<COMPILE_LANGUAGE:CXX>>:/utf-8>")
add_compile_options("$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<COMPILE_LANGUAGE:CXX>>:/Zc:__cplusplus>")

set(PROJECT_NAME ncbind)
set(PROJECT_VERSION 1.00)

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

add_subdirectory(../tp_stub ${CMAKE_CURRENT_BINARY_DIR}/tp_stub)

add_library(${PROJECT_NAME} STATIC
    ncbind.cpp
    ncbind.hpp
    ncb_foreach.h
    ncb_invoke.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    tp_stub
)
endif()

