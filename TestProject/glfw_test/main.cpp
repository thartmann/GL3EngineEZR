/*!
 *\brief    small testproject to develop an OpenGL3+ framework
 *
 *\author   Dominik Ospelt, Hans-Christian Wollert, Tobias Hartmann
 *\date     01-07-2011
 */

#include "glfw_test.h"

/*!
 *\brief    main function
 *
 *\return   0 on success
 */
int main(int argc, char** argv)
{
    glfw_test *myTest = new glfw_test();

    delete myTest;

    return EXIT_SUCCESS;
}
