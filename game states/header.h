#ifndef HEADER_H
#define HEADER_H

// Types and structure definitions
typedef enum GameScreen { TITLE, GAMEPLAY } GameScreen;
// make temperament (one of the members within the unit struct into a typedef enum so that 0, 1, 2, etc is easier to understand)


// Global variables
extern GameScreen currentScreen; // simply declaring that this variable exists. we define it in main.c


// title.c
int return_six();
void UpdateTitleScreen(void);



// gameplay.c
int return_five();
Camera3D* initCamera(void);
void UpdateGameplayScreen(void);




#endif