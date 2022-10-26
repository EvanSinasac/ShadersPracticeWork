#include "globalThings.h"
//Stuff done in class with Michael Feeney
#include <stdio.h>

/*static*/ void GLFW_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error: %s\n", description);

    // Print to console, too
    printf("GLFW Error: %s\n", description);

    return;
}
