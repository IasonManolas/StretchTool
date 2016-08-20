// Enumeration for menu options
enum MENU_OPTIONS
{
	A,
	B,
	A1,
	A1_DOUBLE_POINT_RADIUS,
	A1_HALF_POINT_RADIUS,
	A2,
	A2_COLOURS,
	A2_HARDNESS,
	A2_RADIUS,
	A2_PERMEABILITY,
	A2_CLEAR,
	A2_SAVE,
	A2_LOAD,
	B1_ADD_WEIGHTS,
	B1_CHANGE_PERM,
	B1_CHANGE_RADIUS,
	B2,
	B2_ADD_ANCHOR,
	B2_ADD_DRAG_POINT,
	B2_CLEAR_ANCHOR_LIST,
	B2_DOUBLE_RADIUS,
	B2_HALF_RADIUS,
	B3_DEFORM_MESH,
	B3_KEEP_CURRENT_MESH,
	B4_DEFORM_MESH,
	CAMERA_MOVEMENT,
	RESET_MESH,
	SAVE_DEFORMED_MODEL,
	LOAD_DEFORMED_MODEL,
    BOTH,
    NONE
};

/*******************************************************************************
  Visual Functions
 ******************************************************************************/

// Set up the OpenGL state machine and create a light source
void setup();

// The function responsible for drawing everything in the
// OpenGL context associated to a window.
void render();

// Handle the window size changes and define the world coordinate
// system and projection type
void resize(int w, int h);

// Idle function
void idle();

// Function for menu select events
void menuSelect(int choice);

// Function for handling keyboard events.
void keyboardDown(unsigned char key, int x, int y);

// Function for handling keyboard events.
void keyboardUp(unsigned char key, int x, int y);

// Function for handling keyboard events.
void keyboardSpecialDown(int key, int x, int y);

// Function for handling keyboard events.
void keyboardSpecialUp(int key, int x, int y);

// Function for handling mouse events
void mouseClick(int button, int state, int x, int y);

// Function for handling mouse events
void mouseMotion(int x, int y);