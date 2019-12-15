#pragma once

#include "GCard.hpp"
#include "Ay.hpp"

#include <string>

class Model : public GCard
{
public:
    AY_API Model(const std::string&);
    AY_API ~Model() = default;
};