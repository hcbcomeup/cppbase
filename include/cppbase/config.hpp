#pragma once
#ifndef CPPBASE_CONFIG_HPP
#define CPPBASE_CONFIG_HPP

/**
 * @brief cppbase global compile configuration header file
 * @details Core cross-platform config, compiler/OS detection, C++ standard check
 * @note All macros are preprocessor definitions, compile-time constants, zero runtime cost
 * @note This is the first included header for all cppbase components, compile foundation
 */

#if __cplusplus < 201703L
#error "cppbase requires C++17 or higher compiler standard!"
#endif

/**
 * @brief OS identification macros, value: 0=unknown,1=Windows,2=Linux,3=macOS
 * @note Cross-platform core macro, used for conditional compilation of OS-specific code
 */
#define CPPBASE_OS_UNKNOWN 0
#define CPPBASE_OS_WINDOWS 1
#define CPPBASE_OS_LINUX   2
#define CPPBASE_OS_MACOS   3

/**
 * @brief Compiler identification macros, value:0=unknown,1=MSVC,2=GCC,3=Clang
 * @note Used for conditional compilation of compiler-specific code/fixes
 */
#define CPPBASE_COMPILER_UNKNOWN 0
#define CPPBASE_COMPILER_MSVC    1
#define CPPBASE_COMPILER_GCC     2
#define CPPBASE_COMPILER_CLANG   3

/**
 * @brief Auto detect current OS, set CPPBASE_OS to corresponding value
 * @note _WIN32: match both Windows 32/64 bit; __linux__: Linux; __APPLE__: macOS/iOS
 */
#ifdef _WIN32
#   define CPPBASE_OS CPPBASE_OS_WINDOWS
#elif __linux__
#   define CPPBASE_OS CPPBASE_OS_LINUX
#elif __APPLE__
#   define CPPBASE_OS CPPBASE_OS_MACOS
#else
#   define CPPBASE_OS CPPBASE_OS_UNKNOWN
#endif

/**
 * @brief Auto detect current compiler, set CPPBASE_COMPILER to corresponding value
 * @note _MSC_VER: MSVC compiler; __GNUC__: GCC compiler; __clang__: Clang compiler
 */
#ifdef _MSC_VER
#   define CPPBASE_COMPILER CPPBASE_COMPILER_MSVC
#elif __GNUC__
#   define CPPBASE_COMPILER CPPBASE_COMPILER_GCC
#elif __clang__
#   define CPPBASE_COMPILER CPPBASE_COMPILER_CLANG
#else
#   define CPPBASE_COMPILER CPPBASE_COMPILER_UNKNOWN
#endif

/**
 * @brief Simplified OS check macros (boolean), easy to use in code
 * @note Return true/false, recommended for conditional compilation (if/ifdef)
 */
#define CPPBASE_IS_WINDOWS     (CPPBASE_OS == CPPBASE_OS_WINDOWS)
#define CPPBASE_IS_LINUX       (CPPBASE_OS == CPPBASE_OS_LINUX)
#define CPPBASE_IS_MACOS       (CPPBASE_OS == CPPBASE_OS_MACOS)

/**
 * @brief Simplified compiler check macros (boolean), easy to use in code
 * @note CPPBASE_IS_GNU: GCC + Clang (GNU family compilers), common for Unix-like systems
 */
#define CPPBASE_IS_MSVC     (CPPBASE_COMPILER == CPPBASE_COMPILER_MSVC)
#define CPPBASE_IS_GCC      (CPPBASE_COMPILER == CPPBASE_COMPILER_GCC)
#define CPPBASE_IS_CLANG    (CPPBASE_COMPILER == CPPBASE_COMPILER_CLANG)
#define CPPBASE_IS_GNU      (CPPBASE_IS_GCC || CPPBASE_IS_CLANG) 

#endif // CPPBASE_CONFIG_HPP