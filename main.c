#include <stdio.h>
#include <stdlib.h>
#include "main.h"


// Main function
int main()
{
  
  start(); // Initialize your graphical environment
  loadobj();
  while (run)
  {                             // Main application loop
    Events();                   // Handle events (like user inputs)
    fill_screen(255, 255, 255); // Clear screen to white
    rotate_obj();
    Render(); // Render everything on screen
  }
  
  return 0;
}

