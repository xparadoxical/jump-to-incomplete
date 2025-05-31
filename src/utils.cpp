#include "utils.h"
#include <cmath>

int jti::utils::floorMod(double a, double n)
{
    return a - n * std::floor(a / n);
}