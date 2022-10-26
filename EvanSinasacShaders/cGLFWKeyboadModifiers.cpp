#include "globalThings.h"
//Stuff done in class with Michael Feeney

bool cGFLWKeyboardModifiers::areAllModsUp(GLFWwindow* pWindow)
{
    if (cGFLWKeyboardModifiers::isShiftKeyDown(pWindow)) { return false; }
    if (cGFLWKeyboardModifiers::isControlKeyDown(pWindow)) { return false; }
    if (cGFLWKeyboardModifiers::isAltKeyDown(pWindow)) { return false; }
    // everything is up
    return true;
}

bool cGFLWKeyboardModifiers::isLeftShiftKeyDown(GLFWwindow* pWindow)
{
    if (glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        return true;
    }
    return false;
}
bool cGFLWKeyboardModifiers::isRightShiftKeyDown(GLFWwindow* pWindow)
{
    if (glfwGetKey(pWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        return true;
    }
    return false;
}

// Either down
bool cGFLWKeyboardModifiers::isShiftKeyDown(GLFWwindow* pWindow)
{
    if (cGFLWKeyboardModifiers::isLeftShiftKeyDown(pWindow))
    {
        return true;
    }
    if (cGFLWKeyboardModifiers::isRightShiftKeyDown(pWindow))
    {
        return true;
    }
    return false;
}

bool cGFLWKeyboardModifiers::isLeftControlKeyDown(GLFWwindow* pWindow)
{
    if (glfwGetKey(pWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        return true;
    }
    return false;
}

bool cGFLWKeyboardModifiers::isRightControlKeyDown(GLFWwindow* pWindow)
{
    if (glfwGetKey(pWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        return true;
    }
    return false;
}

// Either down
bool cGFLWKeyboardModifiers::isControlKeyDown(GLFWwindow* pWindow)
{
    if (cGFLWKeyboardModifiers::isLeftControlKeyDown(pWindow))
    {
        return true;
    }
    if (cGFLWKeyboardModifiers::isRightControlKeyDown(pWindow))
    {
        return true;
    }
    return false;
}

bool cGFLWKeyboardModifiers::isLeftAltKeyDown(GLFWwindow* pWindow)
{
    if (glfwGetKey(pWindow, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        return true;
    }
    return false;
}

bool cGFLWKeyboardModifiers::isRightAltKeyDown(GLFWwindow* pWindow)
{
    if (glfwGetKey(pWindow, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
    {
        return true;
    }
    return false;
}

// Either down
bool cGFLWKeyboardModifiers::isAltKeyDown(GLFWwindow* pWindow)
{
    if (cGFLWKeyboardModifiers::isLeftAltKeyDown(pWindow))
    {
        return true;
    }
    if (cGFLWKeyboardModifiers::isRightAltKeyDown(pWindow))
    {
        return true;
    }
    return false;
}

// Returns true if this pattern matches
// example: (true, false, false) returns true if ONLY shift is down
bool cGFLWKeyboardModifiers::isModifierDown(GLFWwindow* pWindow, bool bShift, bool bControl, bool bAlt)
{
    if (isShiftKeyDown(pWindow) != bShift)
    {
        return false;
    }
    if (isControlKeyDown(pWindow) != bControl)
    {
        return false;
    }
    if (isAltKeyDown(pWindow) != bAlt)
    {
        return false;
    }
    // All good
    return true;
}

