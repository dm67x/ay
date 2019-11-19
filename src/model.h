#pragma once

#include "gcard.h"

#include <string>

class Model : public GCard
{
public:
    Model(const std::string&);
    ~Model() = default;
};