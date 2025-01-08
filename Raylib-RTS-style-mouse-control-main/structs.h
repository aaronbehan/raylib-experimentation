#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct Wall { 
    int id; // this will be matched to the values derived from the layout array and will tell us which wall to render on the map
    float height;  // can units fire over the top of it with minor cover protection?
    int health;  // all walls should be destructible
    // texture. still have not touched this aspect of raylib
    // weight. in case i go to the more dynamic map generation using 
    bool door;  // can it be opened to allow units through?
    Color color;
} Wall;

// Player units and enemy units
typedef struct Unit {
    Vector3 unitPos;  // X = East and West. Y = Up and Down. Z = North and South.
    Vector3 waypoint; 
    Vector3 unitSize;
    float unitSpeed;
    int health;
    int attackRate;  // set up a timer. then we can base animations off of this rate. opposing units that face each other attack 
    int damage;  
    bool selected;  // only applies to friendly units when performing an action with them
    //int targetting;  // is this unit targetting another. also make a unique id to target?
    bool targeted;  // this should apply mainly to enemies upon being right clicked so that units continue to target them. it should also apply to friendly units when being healed. could show health upon targeting 
    bool dead;  // make sure that a dead unit cannot move or perform actions
    Color color;
} Unit;

#endif