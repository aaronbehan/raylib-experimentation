#ifndef FUNCTIONS_H
#define FUNCTIONS_H

BoundingBox createBoundingBox(Vector3 startPoint, Vector3 endPoint) // Creates a bounding box using its two furthest corners
{
    BoundingBox boundingBox = { 0 };

    if (startPoint.x <= endPoint.x) boundingBox.min.x = startPoint.x, boundingBox.max.x = endPoint.x;  // Expected mouse drag direction
    else if (startPoint.x > endPoint.x) boundingBox.min.x = endPoint.x, boundingBox.max.x = startPoint.x;  // Unexpected 
    
    if (startPoint.z <= endPoint.z) boundingBox.min.z = startPoint.z, boundingBox.max.z = endPoint.z;  // Expected mouse drag direction
    else if (startPoint.z > endPoint.z) boundingBox.min.z = endPoint.z, boundingBox.max.z = startPoint.z;  // Unexpected

    boundingBox.min.y = 0.0f;
    boundingBox.max.y = 0.5f; // Arbitrary height value 
    
    return boundingBox;
}

Unit InitialiseUnit(void)  // random coords at the moment 
{
    Unit unit = { 0 };

    unit.size = (Vector3){1.0f, 2.0f, 1.0f};
    unit.position = (Vector3){GetRandomValue(-5, 5), unit.size.y/2, GetRandomValue(-5, 5)};  // Unit overlap currently possible
    unit.waypoint = (Vector3){unit.position.x, unit.position.y, unit.position.z};
    // Screen position correctly set to 0, 0
    unit.speed = 0.05;
    unit.attackRate = 0.75;
    unit.maxHealth = 50;
    unit.currentHealth = unit.maxHealth;
    unit.attackValue = 2;
    // Selected correctly set to 0
    // Targeting correctly??? set to 0. Would have liked NULL but having difficulties with defining it in this separate file
    // Ticker correctly set to 0
    unit.colour = PURPLE;

    return unit;
}

#endif