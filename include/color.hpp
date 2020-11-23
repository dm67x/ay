#pragma once

#include "types.hpp"
#include <glm/glm.hpp>

namespace ay
{
    class Color {
    public:
        ///
        /// @brief New color from (r, g, b, a)
        /// @param r Red
        /// @param g Green
        /// @param b Blue
        /// @param a Alpha
        ///  
        Color(f32 r, f32 g, f32 b, f32 a = 1.f)
            : r(r), g(g), b(b), a(a)
        {
        }

        ///
        /// @brief New color from hex
        /// @param hex Hexadecimal value
        ///  
        Color(u32 hex) {
            r = static_cast<f32>((hex & 0xff000000) >> 24) / 255.f;
            g = static_cast<f32>((hex & 0x00ff0000) >> 16) / 255.f;
            b = static_cast<f32>((hex & 0x0000ff00) >> 8) / 255.f;
            a = static_cast<f32>((hex & 0x000000ff)) / 255.f;
        }

        /// 
        /// @brief Comparaison operator
        /// @param c Color
        /// 
        inline bool operator==(const Color& c) {
            return r == c.r && g == c.g && b == c.b && a == c.a;
        }

        ///
        /// @brief Convert to a pointer
        /// @return Float pointer
        ///  
        inline const f32* toPtr() const {
            return &this->r;
        }

        ///
        /// @brief Convert to a pointer
        /// @return Float pointer
        ///  
        inline f32* toPtr() {
            return &this->r;
        }

        ///
        /// @brief Convert color to a Vec4
        /// @return Vec4 Vec4
        ///  
        inline glm::vec4 toVec() const {
            return glm::vec4(r, g, b, a);
        }

        ///
        /// @brief White color
        /// @return Color color
        ///
        inline static Color white() {
            return Color(1.f, 1.f, 1.f);
        }

        ///
        /// @brief Black color
        /// @return Color color
        ///
        inline static Color black() {
            return Color(0.f, 0.f, 0.f);
        }

        ///
        /// @brief Red color
        /// @return Color color
        ///
        inline static Color red() {
            return Color(1.f, 0.f, 0.f);
        }

        ///
        /// @brief Green color
        /// @return Color color
        ///
        inline static Color green() {
            return Color(0.f, 1.f, 0.f);
        }

        ///
        /// @brief Blue color
        /// @return Color color
        ///
        inline static Color blue() {
            return Color(0.f, 0.f, 1.f);
        }

        ///
        /// @brief Yellow color
        /// @return Color color
        ///
        inline static Color yellow() {
            return Color(1.f, 1.f, 0.f);
        }

        ///
        /// @brief Mix two colors
        /// @param c1 Color
        /// @param c2 Color
        /// @return Color
        ///
        friend Color operator*(const Color& c1, const Color& c2) {
            float a = 1.f - (1.f - c2.a) * (1.f - c1.a);
            float r = std::round(c2.r * c2.a / a + c1.r * c1.a * (1.f - c2.a) / a);
            float g = std::round(c2.g * c2.a / a + c1.g * c1.a * (1.f - c2.a) / a);
            float b = std::round(c2.b * c2.a / a + c1.b * c1.a * (1.f - c2.a) / a);
            return Color(r, g, b, a);
        }

        ///
        /// @brief Multiply color with a number
        /// @param c1 Color
        /// @param n Number
        /// @return Color
        ///
        friend Color operator*(const Color& c1, f32 n) {
            return Color(c1.r * n, c1.g * n, c1.b * n, c1.a);
        }

    public:
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };
}