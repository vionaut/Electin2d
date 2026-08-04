cmake_minimum_required(VERSION 3.22)
include(CMakeDependentOption)

project(box2d
	VERSION 3.2.0
	DESCRIPTION "A 2D physics engine for games"
	HOMEPAGE_URL "https://box2d.org"
	LANGUAGES C CXX
)

# stuff to help debug cmake
# message(STATUS "cmake tool chain: ${CMAKE_TOOLCHAIN_FILE}")
# message(STATUS "cmake source dir: ${CMAKE_SOURCE_DIR}")
# message(STATUS "library postfix: ${CMAKE_DEBUG_POSTFIX}")
message(STATUS "CMake C compiler: ${CMAKE_C_COMPILER_ID}")
message(STATUS "CMake C++ compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMake system name: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMake host system processor: ${CMAKE_HOST_SYSTEM_PROCESSOR}")

# static link to make leak checking easier (_crtBreakAlloc)
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if (MSVC OR APPLE OR (UNIX AND NOT APPLE))
	option(BOX2D_SANITIZE "Enable sanitizers for some builds" OFF)
	if(BOX2D_SANITIZE)
		message(STATUS "Box2D Sanitize")
		# sanitizers need to apply to all compiled libraries to work well
		if(MSVC)
			# address sanitizer only in the debug build
			add_compile_options("$<$<CONFIG:Debug>:/fsanitize=address>")
			add_link_options("$<$<CONFIG:Debug>:/INCREMENTAL:NO>")
		elseif(APPLE)
			add_compile_options(-fsanitize=address -fsanitize-address-use-after-scope -fsanitize=undefined)
			add_link_options(-fsanitize=address -fsanitize-address-use-after-scope -fsanitize=undefined)
		endif()
	else()
		if(MSVC AND CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND PROJECT_IS_TOP_LEVEL)
			# enable hot reloading
			add_compile_options("$<$<CONFIG:Debug>:/ZI>")
			add_link_options("$<$<CONFIG:Debug>:/INCREMENTAL>")
		endif()
	endif()
endif()

# Deterministic math
# https://box2d.org/posts/2024/08/determinism/
if (MINGW OR UNIX)
	add_compile_options(-ffp-contract=off)
elseif (MSVC AND CMAKE_C_COMPILER_ID STREQUAL "Clang")
	# clang-cl contracts multiply add into FMA under AVX2, diverging from non FMA builds.
	# The plain driver flag is dropped, so force it through to the frontend.
	add_compile_options(/clang:-ffp-contract=off)
endif()

option(BOX2D_DISABLE_SIMD "Disable SIMD math (slower)" OFF)
option(BOX2D_COMPILE_WARNING_AS_ERROR "Compile warnings as errors" OFF)
option(BOX2D_DOUBLE_PRECISION "Enable double precision for large worlds" OFF)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
	cmake_dependent_option(BOX2D_AVX2 "Enable AVX2" OFF "NOT BOX2D_DISABLE_SIMD" OFF)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Leaving this here for testing
# set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(src)