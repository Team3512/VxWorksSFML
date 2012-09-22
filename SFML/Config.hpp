////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef SFML_CONFIG_HPP
#define SFML_CONFIG_HPP


////////////////////////////////////////////////////////////
// Define the SFML version
////////////////////////////////////////////////////////////
#define SFML_VERSION_MAJOR 2
#define SFML_VERSION_MINOR 0


////////////////////////////////////////////////////////////
// Identify the operating system
////////////////////////////////////////////////////////////
    // Linux
    #define SFML_SYSTEM_LINUX


////////////////////////////////////////////////////////////
// Identify the endianness
////////////////////////////////////////////////////////////
    // Big endian
    #define SFML_ENDIAN_BIG


////////////////////////////////////////////////////////////
// Define a portable debug macro
////////////////////////////////////////////////////////////
#if !defined(NDEBUG)

    #define SFML_DEBUG

#endif


////////////////////////////////////////////////////////////
// Define helpers to create portable import / export macros for each module
////////////////////////////////////////////////////////////
#if !defined(SFML_STATIC)
        #if __GNUC__ >= 4

            // GCC 4 has special keywords for showing/hidding symbols,
            // the same keyword is used for both importing and exporting
            #define SFML_API_EXPORT __attribute__ ((__visibility__ ("default")))
            #define SFML_API_IMPORT __attribute__ ((__visibility__ ("default")))

        #else

            // GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
            #define SFML_API_EXPORT
            #define SFML_API_IMPORT

        #endif

#else

    // Static build doesn't need import/export macros
    #define SFML_API_EXPORT
    #define SFML_API_IMPORT

#endif


////////////////////////////////////////////////////////////
// Define portable fixed-size types
////////////////////////////////////////////////////////////
namespace sf
{
    // All "common" platforms use the same size for char, short and int
    // (basically there are 3 types for 3 sizes, so no other match is possible),
    // we can use them without doing any kind of check

    // 8 bits integer types
    typedef signed   char Int8;
    typedef unsigned char Uint8;

    // 16 bits integer types
    typedef signed   short Int16;
    typedef unsigned short Uint16;

    // 32 bits integer types
    typedef signed   int Int32;
    typedef unsigned int Uint32;

    // 64 bits integer types
    typedef signed   long long Int64;
    typedef unsigned long long Uint64;

} // namespace sf


#endif // SFML_CONFIG_HPP