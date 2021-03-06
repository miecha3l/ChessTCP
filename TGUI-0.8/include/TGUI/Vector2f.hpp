/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2019 Bruno Van de Velde (vdv_b@tgui.eu)
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_VECTOR2F_HPP
#define TGUI_VECTOR2F_HPP

#include <TGUI/Global.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Err.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    class TGUI_API Vector2f
    {
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_CONSTEXPR Vector2f() = default;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor to create from X and Y values
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_CONSTEXPR Vector2f(float xValue, float yValue) :
            x{xValue},
            y{yValue}
        {
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Copy constructor to create from an sf::Vector2f
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Vector2f(const sf::Vector2f& vec) :
            x{vec.x},
            y{vec.y}
        {
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor to create from a string
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Vector2f(const char* str) :
            Vector2f{std::string(str)}
        {
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor to create from a string
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Vector2f(std::string str)
        {
            if (str.empty())
            {
                TGUI_PRINT_WARNING("Failed to parse Vector2f. String was empty.");
                return;
            }

            // Remove the brackets around the value
            if (((str.front() == '(') && (str.back() == ')')) || ((str.front() == '{') && (str.back() == '}')))
                str = str.substr(1, str.length() - 2);

            if (str.empty())
            {
                x = 0;
                y = 0;
                return;
            }

            auto commaPos = str.find(',');
            if (commaPos == std::string::npos)
            {
                TGUI_PRINT_WARNING("Failed to parse Vector2f '" + str + "'. Expected numbers separated with a comma.");
                return;
            }

            x = tgui::stof(trim(str.substr(0, commaPos)));
            y = tgui::stof(trim(str.substr(commaPos + 1)));
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Converts this object into an sf::Vector2f object
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        operator sf::Vector2f() const
        {
            return sf::Vector2f{x, y};
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public:

        float x = 0; ///< X coordinate of the vector
        float y = 0; ///< Y coordinate of the vector

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Overload of unary operator -
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline TGUI_CONSTEXPR Vector2f operator-(const Vector2f& right)
    {
        return {-right.x, -right.y};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Overload of binary operator +=
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline TGUI_CONSTEXPR Vector2f& operator+=(Vector2f& left, const Vector2f& right)
    {
        left.x += right.x;
        left.y += right.y;
        return left;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Overload of binary operator -=
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline TGUI_CONSTEXPR Vector2f& operator-=(Vector2f& left, const Vector2f& right)
    {
        left.x -= right.x;
        left.y -= right.y;
        return left;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Overload of binary operator +
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline TGUI_CONSTEXPR Vector2f operator+(const Vector2f& left, const Vector2f& right)
    {
        return {left.x + right.x, left.y + right.y};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Overload of binary operator -
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline TGUI_CONSTEXPR Vector2f operator-(const Vector2f& left, const Vector2f& right)
    {
        return {left.x - right.x, left.y - right.y};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Overload of binary operator *
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline TGUI_CONSTEXPR Vector2f operator*(const Vector2f& left, float right)
    {
        return {left.x * right, left.y * right};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Overload of binary operator *
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline TGUI_CONSTEXPR Vector2f operator*(float left, const Vector2f& right)
    {
        return {left * right.x, left * right.y};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Overload of binary operator *=
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline TGUI_CONSTEXPR Vector2f& operator*=(Vector2f& left, float right)
    {
        left.x *= right;
        left.y *= right;
        return left;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Overload of binary operator /
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline TGUI_CONSTEXPR Vector2f operator/(const Vector2f& left, float right)
    {
        return {left.x / right, left.y / right};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Overload of binary operator /=
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline TGUI_CONSTEXPR Vector2f& operator/=(Vector2f& left, float right)
    {
        left.x /= right;
        left.y /= right;
        return left;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Overload of binary operator ==
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline TGUI_CONSTEXPR bool operator==(const Vector2f& left, const Vector2f& right)
    {
        return (left.x == right.x) && (left.y == right.y);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Overload of binary operator ==
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline TGUI_CONSTEXPR bool operator!=(const Vector2f& left, const Vector2f& right)
    {
        return !(left == right);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_VECTOR2F_HPP
