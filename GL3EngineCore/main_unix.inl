/*!
 *\brief    UNIX main
 *
 *\author   Dominik Ospelt, Hans-Christian Wollert, Tobias Hartmann
 *\date     01-07-2011
 */

#include "LPVApp.h"

#include <ctime>
#include <GL/glfw.h>
#include "Timer.h"
LPVApp *myLPVApp;

bool m_running;

int m_windowWidth;
int m_windowHeight;

int filling = 1;
Timer timer;
float tslf = 0.0f;
bool mousedown = false;
int mousex = 0;
int mousey = 0;
int deltax = 0 ;
int deltay = 0;

/*!
 *\brief glfw callback for window resizing
 *
 *\param width - new window width
 *\param height - new window height
 */
void GLFWCALL WindowSizeHandler(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	myLPVApp->SetScreenSize(m_windowWidth, m_windowHeight);
}


/*!
 *\brief glfw callback for keyboard events
 *
 *\param key - key triggered
 *\param action - action that triggered the key
 */
void GLFWCALL KeyHandler(int key, int action)
{
    if (action != GLFW_PRESS)
    {
        return;
    }

    switch (key)
    {
		case GLFW_KEY_ESC:
			m_running = false;
			break;
		case GLFW_KEY_F1:
		case GLFW_KEY_F2:
		case GLFW_KEY_F3:
		case GLFW_KEY_F4:
		case GLFW_KEY_F17:
		case GLFW_KEY_F18:
		case GLFW_KEY_F19:
		case GLFW_KEY_F20:
		case GLFW_KEY_F21:
		case GLFW_KEY_F22:
		case GLFW_KEY_F23:
		case GLFW_KEY_F24:
		case GLFW_KEY_F25:
		case GLFW_KEY_UP:
		case GLFW_KEY_DOWN:
		case GLFW_KEY_LEFT:
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_LSHIFT:
		case GLFW_KEY_RSHIFT:
		case GLFW_KEY_LCTRL:
		case GLFW_KEY_RCTRL:
		case GLFW_KEY_LALT:
		case GLFW_KEY_RALT:
		case GLFW_KEY_TAB:
		case GLFW_KEY_ENTER:
		case GLFW_KEY_BACKSPACE:
		case GLFW_KEY_INSERT:
		case GLFW_KEY_DEL:
		case GLFW_KEY_PAGEUP:
		case GLFW_KEY_PAGEDOWN:
		case GLFW_KEY_HOME:
		case GLFW_KEY_END:
		case GLFW_KEY_KP_0:
			{
				myLPVApp->showRenderTarget( 0 );
				break;
			}
		case GLFW_KEY_KP_1:
			{
				myLPVApp->showRenderTarget( 1 );
				break;
			}
		case GLFW_KEY_KP_2:
			{
				myLPVApp->showRenderTarget( 2 );
				break;
			}
		case GLFW_KEY_KP_3:
			{
				myLPVApp->showRenderTarget( 3 );
				break;
			}
		case GLFW_KEY_KP_4:
			{
				myLPVApp->showRenderTarget( 4 );
				break;
			}
		case GLFW_KEY_KP_5:
		case GLFW_KEY_KP_6:
		case GLFW_KEY_KP_7:
		case GLFW_KEY_KP_8:
		case GLFW_KEY_KP_9:
		case GLFW_KEY_KP_DIVIDE:
		case GLFW_KEY_KP_MULTIPLY:
		case GLFW_KEY_KP_SUBTRACT:
		case GLFW_KEY_KP_ADD:
		case GLFW_KEY_KP_DECIMAL:
		case GLFW_KEY_KP_EQUAL:
		case GLFW_KEY_KP_ENTER:
		case GLFW_KEY_SPACE:
			break;
		case 'W':
			{
				myLPVApp->m_Camera.camMove(glm::vec3(0.0f, 0.0f , 1.0f) * tslf, TS_LOCAL);
				break;
			}
		case 'S':
			{
				myLPVApp->m_Camera.camMove(glm::vec3(0.0f, 0.0f , -1.0f) * tslf, TS_LOCAL);
				break;
			}
		case 'A':
			{
				myLPVApp->m_Camera.camMove(glm::vec3(1.0f, 0.0f , 0.0f) * tslf, TS_LOCAL);
				break;
			}
		case 'D':
			{
				myLPVApp->m_Camera.camMove(glm::vec3(-1.0f, 0.0f , 0.0f) * tslf, TS_LOCAL);
				break;
			}
		case 'C':
			break;
		case 'E':
			break;
		case 'Q':
			break;
		default:
			break;
    }
}

/*!
 *\brief glfw callback for mouseMovement
 *
 *\param x - x coordinate of the mouse cursor
 *\param y - y coordinate of the mouse cursor
 */
void GLFWCALL MouseMovementHandler(int x, int y)
{
	deltax = x - mousex;
	deltay = y - mousey;
}

/*!
 *\brief glfw callback for mouseMovement
 *
 *\param button - mouse button pressed
 *\param action - action triggered
 */
void GLFWCALL MouseButtonHandler(int button, int action)
{
	if (action != GLFW_PRESS)
	{
		mousedown = false;
		return;
	}

	switch (button)
	{
		case GLFW_MOUSE_BUTTON_1:
			{
				if (!mousedown)
				{
					mousedown = true;
					glfwGetMousePos(&mousex, &mousey);
				}
				break;
			}
		case GLFW_MOUSE_BUTTON_2:
			{
				break;
			}
		case GLFW_MOUSE_BUTTON_3:
			{
				break;
			}
		case GLFW_MOUSE_BUTTON_4:
			{
				break;
			}
		case GLFW_MOUSE_BUTTON_5:
			{
				break;
			}
		case GLFW_MOUSE_BUTTON_6:
			{
				break;
			}
		case GLFW_MOUSE_BUTTON_7:
			{
				break;
			}
		case GLFW_MOUSE_BUTTON_8:
			{
				break;
			}
		default:
			{
				break;
			}
	}
}

/*!
 *\brief glfw callback for mouseMovement
 *
 *\param pos - position of wheel
 */
void GLFWCALL MouseWheelHandler(int pos)
{
	return;
}

/*!
 *\brief initializes the window and LPVApp
 *
 *\return 0 on success
 */
int InitApp()
{
	timer = Timer();
 	myLPVApp = new LPVApp();

	m_windowWidth = 800;
    m_windowHeight = 600;

    myLPVApp->SetScreenSize(m_windowWidth, m_windowHeight);

	if (!glfwInit())
    {
        std::cerr << "Initialization of GLFW failed!" << std::endl;

        return EXIT_FAILURE;
    }

    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!glfwOpenWindow( m_windowWidth, m_windowHeight, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
    {
        glfwTerminate();

        std::cerr << "GLFW opening of window failed!" << std::endl;

        return EXIT_FAILURE;
    }

    glfwSetWindowTitle("LPV Beauty");

//    //turn vsync on
//    glfwSwapInterval(1);

    //set glfw window size callback
    glfwSetWindowSizeCallback(WindowSizeHandler);

    //set glfw keyboard callback
    glfwSetKeyCallback(KeyHandler);
    glfwEnable(GLFW_KEY_REPEAT);

    glfwSetMousePosCallback(MouseMovementHandler);
    glfwSetMouseButtonCallback(MouseButtonHandler);
    glfwSetMouseWheelCallback(MouseWheelHandler);

    glfwEnable(GLFW_AUTO_POLL_EVENTS);

	std::cout << "OpenGL window context version: " << glfwGetWindowParam(GLFW_OPENGL_PROFILE) << std::endl;

	myLPVApp->Initialize();

    return EXIT_SUCCESS;
}

/*!
 *\brief main loop
 *
 *\return 0 on success
 */
int MainLoop()
{
   m_running = true;

    while(m_running)
    {
    	//time prototype
      timer.updateFPS();
		tslf = timer.getSPF() *0.01;
    glfwPollEvents();

		if(mousedown)
		{
			myLPVApp->m_Camera.camRotate(deltax * 0.5 * tslf, glm::vec3(0.0,1.0,0.0));
			myLPVApp->m_Camera.camRotate(deltay * 0.5 * tslf, glm::vec3(1.0,0.0,0.0),TS_LOCAL);
		}

		myLPVApp->RenderScene();

        glfwSwapBuffers();

		m_running = m_running && glfwGetWindowParam(GLFW_OPENED);
    }

    glfwCloseWindow();

    glfwTerminate();

    return EXIT_SUCCESS;
}

/*!
 *\brief cleanup
 *
 *\return 0 on success
 */
int ShutDown()
{
	delete myLPVApp;

	return EXIT_SUCCESS;
}


/*!
 *\brief    main function
 *
 *\return   0 on success
 */
int main(int argc, char** argv)
{
	InitApp();

	MainLoop();

	ShutDown();

    return EXIT_SUCCESS;
}
