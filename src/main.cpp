//----------------------------------------------------------------------------//
//    Learning OpenGL: Εισαγωγικά Μαθήματα OpenGL                             //
//                                                                            //
//    Πλατφόρμα ανάπτυξης παραδειγμάτων                                       //
//                                                                            //
//    Εργαστήριο 4ο                                                           //
//----------------------------------------------------------------------------//

#include <stdlib.h>	  // Just for some standard functions
#include <stdio.h>    // - Just for some ASCII messages
#include <glut.h>     // - An interface and windows management library
#include "visuals.h"  // Header file for our OpenGL functions

/*******************************************************************************
  Main Program code
 ******************************************************************************/

int main(int argc, char* argv[])
{
    // initialize GLUT library state
    glutInit(&argc, argv);

    // Set up the display using the GLUT functions to
    // get rid of the window setup details:
    // - Use true RGB colour mode ( and transparency )
    // - Enable double buffering for faster window update
    // - Allocate a Depth-Buffer in the system memory or
    //   in the video memory if 3D acceleration available
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    // Define the main window size and initial position
    // ( upper left corner, boundaries included )
    glutInitWindowSize(480, 480);
    glutInitWindowPosition(50, 50);

    // Create and label the main window
    glutCreateWindow("Computer Graphics Project");

    // Configure various properties of the OpenGL rendering context
    setup();

    // Callbacks for the GL and GLUT events:
    // Set the rendering function
    glutDisplayFunc(render);
    // Set the resize function
    glutReshapeFunc(resize);
    // Set the idle function
    glutIdleFunc(idle);
  
    //-------------Task2------------------
    // Set function for menu
	// create a sub menu 
	
	int A1menu = glutCreateMenu(menuSelect);
	glutAddMenuEntry("Ray Cast Picking", A1);
	glutAddMenuEntry("Double Radius", A1_DOUBLE_POINT_RADIUS);
	glutAddMenuEntry("Half Radius", A1_HALF_POINT_RADIUS);
	int A2menu = glutCreateMenu(menuSelect);
	glutAddMenuEntry("Draw", A2);
	glutAddMenuEntry("Set Colour", A2_COLOURS);
	glutAddMenuEntry("Set Hardness", A2_HARDNESS);
	glutAddMenuEntry("Set Radius", A2_RADIUS);
	glutAddMenuEntry("Set Permeability", A2_PERMEABILITY);
	//glutAddMenuEntry("Clear Model", A2_CLEAR);
	
	int A3menu = glutCreateMenu(menuSelect);
	glutAddMenuEntry("Save Model", A2_SAVE);
	glutAddMenuEntry("Load Model", A2_LOAD);

	int Amenu = glutCreateMenu(menuSelect);
	glutAddSubMenu("Ai", A1menu);
	glutAddSubMenu("Aii", A2menu);
	glutAddSubMenu("Aiii", A3menu);

	int B1menu = glutCreateMenu(menuSelect);
	glutAddMenuEntry("Add weights", B1_ADD_WEIGHTS);
	glutAddMenuEntry("Change Radius", B1_CHANGE_RADIUS);
	glutAddMenuEntry("Change Permeability", B1_CHANGE_PERM);

	int B2menu = glutCreateMenu(menuSelect);
	glutAddMenuEntry("Add Anchor Point", B2_ADD_ANCHOR);
	glutAddMenuEntry("Add Drag Point", B2_ADD_DRAG_POINT);
	glutAddMenuEntry("Double Anchor Point Radius", B2_DOUBLE_RADIUS);
	glutAddMenuEntry("Half Anchor Point Radius", B2_HALF_RADIUS);

	int B3menu = glutCreateMenu(menuSelect);
	glutAddMenuEntry("Drag Point", B3_DEFORM_MESH);
	glutAddMenuEntry("Keep Current Mesh", B3_KEEP_CURRENT_MESH);
	
	int B4menu = glutCreateMenu(menuSelect);
	glutAddMenuEntry("Drag Point", B4_DEFORM_MESH);
	//glutAddMenuEntry("Keep Current Mesh", B4_KEEP_CURRENT_MESH);

	int BMenu = glutCreateMenu(menuSelect);
	glutAddSubMenu("B1", B1menu);
	glutAddSubMenu("B2", B2menu);
	glutAddSubMenu("B3", B3menu);
	glutAddSubMenu("B4", B4menu);
	glutAddMenuEntry("Save Deformed Mesh", SAVE_DEFORMED_MODEL);
	glutAddMenuEntry("Load Deformed Mesh", LOAD_DEFORMED_MODEL);



	// create main "right click" menu
	glutCreateMenu(menuSelect);
	glutAddSubMenu("A", Amenu);
	glutAddSubMenu("B", BMenu);
	glutAddMenuEntry("Turn on/off Camera Movement", CAMERA_MOVEMENT);
	glutAddMenuEntry("Reset Mesh", RESET_MESH);
	

	
	/*int subMenu = glutCreateMenu(menuSelect);
	glutAddMenuEntry("Set Colours",)
    int menu=glutCreateMenu(menuSelect);
	glutAddSubMenu("submenu", menu);
    glutAddMenuEntry("A1", A1);
	glutAddMenuEntry("A2", A2);
    glutAddMenuEntry("Both", BOTH);
    glutAddMenuEntry("None", NONE);*/
    // attach the menu to the right button
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //-------------Task3------------------
    // Set functions for keyboard
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(keyboardSpecialDown);
    glutSpecialUpFunc(keyboardSpecialUp);

    //-------------Task4------------------
    // Set functions for mouse
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);

    //Enter main event handling loop
    glutMainLoop();
    return 0;
}

