#include "math.h"  // for fabs(), floor() and ceil()

// For tile based movement. Evaluates whether the coordinate (eg. 45.342396) is closer to 45 or 46 and rounds up or down accordingly 
Vector3 lockToTile(Vector3 unitPos) 
{
    int xLower = floor(unitPos.x);
    int xUpper = ceil(unitPos.x);

    int zLower = floor(unitPos.z);
    int zUpper = ceil(unitPos.z);

    if (fabs(unitPos.x - xLower) < fabs(xUpper - unitPos.x)) {
        unitPos.x = (float)xLower;
    } else {
        unitPos.x = (float)xUpper;
    }

    if (fabs(unitPos.z - zLower) < fabs(zUpper - unitPos.z)) {
        unitPos.z = (float)zLower;
    } else {
        unitPos.z = (float)zUpper;
    }

    return unitPos;
}
