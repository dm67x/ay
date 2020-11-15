#pragma once

#include "math.hpp"

struct Color {
    float r;
    float g;
    float b;
    float a;

    ///
    /// @brief New color from (r, g, b, a)
    /// @param r Red
    /// @param g Green
    /// @param b Blue
    /// @param a Alpha
    ///  
    Color(float r, float g, float b, float a = 1.f) : r(r), g(g), b(b), a(a) {}
    
    ///
    /// @brief New color from hex
    /// @param hex Hexadecimal value
    ///  
    Color(unsigned int hex) {
        r = static_cast<float>((hex & 0xff000000) >> 24) / 255.f;
        g = static_cast<float>((hex & 0x00ff0000) >> 16) / 255.f;
        b = static_cast<float>((hex & 0x0000ff00) >> 8) / 255.f;
        a = static_cast<float>((hex & 0x000000ff)) / 255.f;
    }

    ///
    /// @brief Convert color to a Vec4
    /// @return Vec4 Vec4
    ///  
    inline Vec4 toVec() const {
        return Vec4(r, g, b, a);
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
    friend Color operator*(const Color& c1, float n) {
        return Color(c1.r * n, c1.g * n, c1.b * n, c1.a);
    }
};