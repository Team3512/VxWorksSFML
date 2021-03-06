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

/* !!! THIS IS AN EXTREMELY ALTERED AND PURPOSE-BUILT VERSION OF SFML !!!
 * This distribution is designed to possess only a limited subset of the
 * original library's functionality and to only build on VxWorks 6.3.
 * The original distribution of this software has many more features and
 * supports more platforms.
 */

#ifndef SFML_THREADLOCAL_HPP
#define SFML_THREADLOCAL_HPP

#include "NonCopyable.hpp"
#include <pthread.h>
#include <cstdlib>


namespace sf
{

// Defines variables with thread-local storage
class ThreadLocal : NonCopyable {
public:
    ThreadLocal(void* value = NULL);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~ThreadLocal();

    ////////////////////////////////////////////////////////////
    /// \brief Set the thread-specific value of the variable
    ///
    /// \param value Value of the variable for the current thread
    ///
    ////////////////////////////////////////////////////////////
    void setValue(void* value);

    ////////////////////////////////////////////////////////////
    /// \brief Retrieve the thread-specific value of the variable
    ///
    /// \return Value of the variable for the current thread
    ///
    ////////////////////////////////////////////////////////////
    void* getValue() const;

private:
    pthread_key_t m_key; ///< Index of our thread-local storage slot
};

} // namespace sf


#endif // SFML_THREADLOCAL_HPP
