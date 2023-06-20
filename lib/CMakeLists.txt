cmake_minimum_required(VERSION 3.9)
set(PROJECT_NAME "ck_pathcobbler_bindings")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MINGW32)
	set(CMAKE_SYSTEM_NAME Windows)
	set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc-posix)
	set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++-posix)
	set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
	set(CMAKE_RANLIB x86_64-w64-mingw32-ranlib)
	set(CMAKE_SHARED_LIBRARY_SUFFIX .dll)
else()
	set(CMAKE_SHARED_LIBRARY_SUFFIX .so)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB_RECURSE ckutils_executable_sources CONFIGURE_DEPENDS
    # "${PROJECT_SOURCE_DIR}/ck_utilities/src/*.cpp"
	"${PROJECT_SOURCE_DIR}/ck_utilities/src/team254_geometry/*.cpp"
	"${PROJECT_SOURCE_DIR}/ck_utilities/src/trajectory/*.cpp"
)

set(INT_LIB_NAME "ck_utilities")
add_library(${INT_LIB_NAME} STATIC ${ckutils_executable_sources})
set_property(TARGET ${INT_LIB_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(${INT_LIB_NAME} PUBLIC "ck_utilities/include")
target_link_libraries(${INT_LIB_NAME} Threads::Threads)

file(GLOB_RECURSE executable_sources CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${executable_sources} path_lib.def)
target_include_directories(${PROJECT_NAME} PUBLIC "include" "ck_utilities/include")
target_link_libraries(${PROJECT_NAME} Threads::Threads ${INT_LIB_NAME})
#add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS})

# include_directories("ck_utilities/include")
# include_directories("include")
# add_executable(main ${executable_sources} ${ckutils_executable_sources})
# add_executable(main "${PROJECT_SOURCE_DIR}/src/main.cpp")