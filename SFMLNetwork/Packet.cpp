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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "../SFML/Network/Packet.hpp"
#include "Unix/SocketImpl.hpp"
#include "../SFML/System/String.hpp"
#include <cstring>

uint64_t htonll( uint64_t value ) {
	// The answer is 42
	static const int num = 42;

	// Check the endianness
	if ( *reinterpret_cast<const char*>(&num) == num ) { // if little endian
		const uint32_t high_part = htonl( static_cast<uint32_t>(value >> 32) );
		const uint32_t low_part = htonl( static_cast<uint32_t>(value & 0xFFFFFFFFLL) );

		return ( static_cast<uint64_t>(low_part) << 32 ) | high_part;
	}
	else
		return value;
}

uint64_t ntohll( uint64_t value ) {
	// The answer is 42
	static const int num = 42;

	// Check the endianness
	if ( *reinterpret_cast<const char*>(&num) == num ) // if little endian
		return value;
	else {
		const uint32_t high_part = ntohl( static_cast<uint32_t>(value << 32) );
		const uint32_t low_part = ntohl( static_cast<uint32_t>(value & (0xFFFFFFFFLL) ) );

		return ( static_cast<uint64_t>(low_part) >> 32 ) | high_part;
	}
}

namespace sf
{
////////////////////////////////////////////////////////////
Packet::Packet() :
m_readPos(0),
m_isValid(true)
{

}


////////////////////////////////////////////////////////////
Packet::~Packet()
{

}


////////////////////////////////////////////////////////////
void Packet::append(const void* data, std::size_t sizeInBytes)
{
    if (data && (sizeInBytes > 0))
    {
        std::size_t start = m_packetData.size();
        m_packetData.resize(start + sizeInBytes);
        std::memcpy(&m_packetData[start], data, sizeInBytes);
    }
}


////////////////////////////////////////////////////////////
void Packet::clear()
{
    m_packetData.clear();
    m_readPos = 0;
    m_isValid = true;
}


////////////////////////////////////////////////////////////
const void* Packet::getData() const
{
    return !m_packetData.empty() ? &m_packetData[0] : NULL;
}


////////////////////////////////////////////////////////////
std::size_t Packet::getDataSize() const
{
    return m_packetData.size();
}


////////////////////////////////////////////////////////////
bool Packet::endOfPacket() const
{
    return m_readPos >= m_packetData.size();
}


////////////////////////////////////////////////////////////
Packet::operator BoolType() const
{
    return m_isValid ? &Packet::checkSize : NULL;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(bool& data)
{
    Uint8 value;
    if (*this >> value)
        data = (value != 0);

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(Int8& data)
{
    if (checkSize(sizeof(data)))
    {
        data = *reinterpret_cast<const Int8*>(&m_packetData[m_readPos]);
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(Uint8& data)
{
    if (checkSize(sizeof(data)))
    {
        data = *reinterpret_cast<const Uint8*>(&m_packetData[m_readPos]);
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(Int16& data)
{
    if (checkSize(sizeof(data)))
    {
        data = ntohs(*reinterpret_cast<const Int16*>(&m_packetData[m_readPos]));
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(Uint16& data)
{
    if (checkSize(sizeof(data)))
    {
        data = ntohs(*reinterpret_cast<const Uint16*>(&m_packetData[m_readPos]));
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(Int32& data)
{
    if (checkSize(sizeof(data)))
    {
        data = ntohl(*reinterpret_cast<const Int32*>(&m_packetData[m_readPos]));
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(Uint32& data)
{
    if (checkSize(sizeof(data)))
    {
        data = ntohl(*reinterpret_cast<const Uint32*>(&m_packetData[m_readPos]));
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(Int64& data)
{
	if(checkSize(sizeof(data)))
	{
		data = ntohll(*reinterpret_cast<const Int64*>(&m_packetData[m_readPos]));
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(Uint64& data)
{
	if(checkSize(sizeof(data)))
	{
		data = ntohll(*reinterpret_cast<const Uint64*>(&m_packetData[m_readPos]));
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(float& data)
{
    if (checkSize(sizeof(data)))
    {
        data = *reinterpret_cast<const float*>(&m_packetData[m_readPos]);
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(double& data)
{
    if (checkSize(sizeof(data)))
    {
        data = *reinterpret_cast<const double*>(&m_packetData[m_readPos]);
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(char* data)
{
    // First extract string length
    Uint32 length = 0;
    *this >> length;

    if ((length > 0) && checkSize(length))
    {
        // Then extract characters
        std::memcpy(data, &m_packetData[m_readPos], length);
        data[length] = '\0';

        // Update reading position
        m_readPos += length;
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(std::string& data)
{
    // First extract string length
    Uint32 length = 0;
    *this >> length;

    data.clear();
    if ((length > 0) && checkSize(length))
    {
        // Then extract characters
        data.assign(&m_packetData[m_readPos], length);

        // Update reading position
        m_readPos += length;
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(wchar_t* data)
{
    // First extract string length
    Uint32 length = 0;
    *this >> length;

    if ((length > 0) && checkSize(length * sizeof(Uint32)))
    {
        // Then extract characters
        for (Uint32 i = 0; i < length; ++i)
        {
            Uint32 character = 0;
            *this >> character;
            data[i] = static_cast<wchar_t>(character);
        }
        data[length] = L'\0';
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(std::wstring& data)
{
    // First extract string length
    Uint32 length = 0;
    *this >> length;

    data.clear();
    if ((length > 0) && checkSize(length * sizeof(Uint32)))
    {
        // Then extract characters
        for (Uint32 i = 0; i < length; ++i)
        {
            Uint32 character = 0;
            *this >> character;
            data += static_cast<wchar_t>(character);
        }
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(String& data)
{
    // First extract the string length
    Uint32 length = 0;
    *this >> length;

    data.clear();
    if ((length > 0) && checkSize(length * sizeof(Uint32)))
    {
        // Then extract characters
        for (Uint32 i = 0; i < length; ++i)
        {
            Uint32 character = 0;
            *this >> character;
            data += character;
        }
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(bool data)
{
    *this << static_cast<Uint8>(data);
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(Int8 data)
{
    append(&data, sizeof(data));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(Uint8 data)
{
    append(&data, sizeof(data));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(Int16 data)
{
    Int16 toWrite = htons(data);
    append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(Uint16 data)
{
    Uint16 toWrite = htons(data);
    append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(Int32 data)
{
    Int32 toWrite = htonl(data);
    append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(Uint32 data)
{
    Uint32 toWrite = htonl(data);
    append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(Int64 data)
{
    Int64 toWrite = htonll(data);
    append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(Uint64 data)
{
    Uint64 toWrite = htonll(data);
    append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(float data)
{
    append(&data, sizeof(data));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(double data)
{
    append(&data, sizeof(data));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(const char* data)
{
    // First insert string length
    Uint32 length = 0;
    for (const char* c = data; *c != '\0'; ++c)
        ++length;
    *this << length;

    // Then insert characters
    append(data, length * sizeof(char));

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(const std::string& data)
{
    // First insert string length
    Uint32 length = static_cast<Uint32>(data.size());
    *this << length;

    // Then insert characters
    if (length > 0)
    {
        append(data.c_str(), length * sizeof(std::string::value_type));
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(const wchar_t* data)
{
    // First insert string length
    Uint32 length = 0;
    for (const wchar_t* c = data; *c != L'\0'; ++c)
        ++length;
    *this << length;

    // Then insert characters
    for (const wchar_t* c = data; *c != L'\0'; ++c)
        *this << static_cast<Uint32>(*c);

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(const std::wstring& data)
{
    // First insert string length
    Uint32 length = static_cast<Uint32>(data.size());
    *this << length;

    // Then insert characters
    if (length > 0)
    {
        for (std::wstring::const_iterator c = data.begin(); c != data.end(); ++c)
            *this << static_cast<Uint32>(*c);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(const String& data)
{
    // First insert the string length
    Uint32 length = static_cast<Uint32>(data.getSize());
    *this << length;

    // Then insert characters
    if (length > 0)
    {
        for (String::ConstIterator c = data.begin(); c != data.end(); ++c)
            *this << *c;
    }

    return *this;
}


////////////////////////////////////////////////////////////
bool Packet::checkSize(std::size_t size)
{
    m_isValid = m_isValid && (m_readPos + size <= m_packetData.size());

    return m_isValid;
}


////////////////////////////////////////////////////////////
const void* Packet::onSend(std::size_t& size)
{
    size = getDataSize();
    return getData();
}


////////////////////////////////////////////////////////////
void Packet::onReceive(const void* data, std::size_t size)
{
    append(data, size);
}

} // namespace sf
