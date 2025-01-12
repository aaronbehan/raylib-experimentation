#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct Timer {
    float Lifetime;
} Timer;

typedef struct Wall { 
    Vector3 position;
    int id; // this will be matched to the values derived from the layout array and will tell us which wall to render on the map
    int health;  // most walls should be destructible
    // texture. still have not touched this aspect of raylib
    // weight. in case i go to the more dynamic map generation using
    bool door;  // can it be opened to allow units through?
    Color colour;
} Wall;

// Player units and enemy units
typedef struct Unit {
    Vector3 position;  // X = East and West. Y = Up and Down. Z = North and South.
    Vector3 waypoint; // must always start with identical coords as unitPos
    Vector3 size;
    Vector2 screenPos; // strictly used for drawing text above unit. possibly not necessary
    float speed;
    float attackRate;  // should be a value like 0.75 (1 attack every 3 quarters a second)
    int maxHealth;
    int currentHealth;
    int attackValue;  
    bool selected;  // only applies to friendly units when selected with the mouse
    Vector3 *targeting;  // is this unit targetting another? If so, it is just storing a unitPos vector and following the point
    bool dead;  // make sure that a dead unit cannot move or perform actions. could be superfluous
    Timer ticker;
    Color colour;
} Unit;

#endif