#pragma once

#include "gcard.h"
#include "ay.h"

#include <string>

class Model : public GCard
{
public:
    AY_API Model(const std::string&);
    AY_API ~Model() = default;
};