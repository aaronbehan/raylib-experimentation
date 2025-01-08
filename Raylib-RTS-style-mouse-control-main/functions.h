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

#endif