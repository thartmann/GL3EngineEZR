/*!
 *\brief    Light Propagation Volumes!
 *
 *\author   Dominik Ospelt, Hans-Christian Wollert, Tobias Hartmann
 *\date     01-07-2011
 */

#include "LPVApp.h"

#include <iostream>
#include <ctime>

bool KeyDown[256];
bool SpecialKeyDown[256];

using namespace std;

int m_windowWidth=800;
int m_windowHeight=600;
int winID = 0;
LPVApp *myLPVApp;
int filling = 1;
clock_t time_start;
clock_t time_current;
clock_t time_diff;
clock_t time_last;
float tslf = 0.0f;
bool mousedown = false;
int mousex = 0;
int mousey = 0;
int deltax = 0 ;
int deltay = 0;

void InitGL() {
	glGetError();
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
}

void OnRender() {

	//time prototype
	time_current = clock();
	time_diff = time_current - time_last;
	time_last = time_current;
	tslf = float(time_diff *0.001);/// CLOCKS_PER_SEC;// timesincelastframe in milliseconds

	//switch render Targets
	if(KeyDown['1'])myLPVApp->showRenderTarget(0); //show final rendering
	if(KeyDown['2'])myLPVApp->showRenderTarget(1); //show colorMap
	if(KeyDown['3'])myLPVApp->showRenderTarget(2); //show positionMap
	if(KeyDown['4'])myLPVApp->showRenderTarget(3); //show normalMap
	if(KeyDown['5'])myLPVApp->showRenderTarget(4); //show fluxMap
	if(KeyDown['6'])myLPVApp->showRenderTarget(5); //show depthMap
	if(KeyDown['7'])myLPVApp->showRenderTarget(6); //show ssao


	//input handling
	if(KeyDown['w'])myLPVApp->m_Camera.camMove(glm::vec3(0.0f, 0.0f , 1.0f) * tslf, TS_LOCAL);
	if(KeyDown['s'])myLPVApp->m_Camera.camMove(glm::vec3(0.0f, 0.0f , -1.0f) * tslf, TS_LOCAL);
	if(KeyDown['a'])myLPVApp->m_Camera.camMove(glm::vec3(1.0f, 0.0f , 0.0f) * tslf, TS_LOCAL);
	if(KeyDown['d'])myLPVApp->m_Camera.camMove(glm::vec3(-1.0f, 0.0f , 0.0f) * tslf, TS_LOCAL);

	/*if(SpecialKeyDown[GLUT_KEY_LEFT])myLPVApp->m_ls.updatePositionVector(glm::vec3(0.01, 0.0, 0.0));
	if(SpecialKeyDown[GLUT_KEY_RIGHT])myLPVApp->m_ls.updatePositionVector(glm::vec3(-0.01, 0.0, 0.0));
	if(SpecialKeyDown[GLUT_KEY_DOWN])myLPVApp->m_ls.updatePositionVector(glm::vec3(0.0, 0.0, 0.01));
	if(SpecialKeyDown[GLUT_KEY_UP])myLPVApp->m_ls.updatePositionVector(glm::vec3(0.0, 0.0, -0.01));*/

	if(SpecialKeyDown[GLUT_KEY_LEFT])myLPVApp->changeLightPosition(glm::vec3(1.0, 0.0, 0.0)*tslf);
	if(SpecialKeyDown[GLUT_KEY_RIGHT])myLPVApp->changeLightPosition(glm::vec3(-1.0, 0.0, 0.0)*tslf);
	if(SpecialKeyDown[GLUT_KEY_DOWN])myLPVApp->changeLightPosition(glm::vec3(0.0, 1.0, 0.0)*tslf);
	if(SpecialKeyDown[GLUT_KEY_UP])myLPVApp->changeLightPosition(glm::vec3(0.0, -1.0, 0.0)*tslf);

	if(KeyDown['y'])myLPVApp->m_ls.updateDirectionVector(glm::vec3(0.01, 0.0, 0.0));
	if(KeyDown['x'])myLPVApp->m_ls.updateDirectionVector(glm::vec3(-0.01, 0.0, 0.0));
	if(KeyDown['c'])myLPVApp->m_ls.updateDirectionVector(glm::vec3(0.0, 0.0, 0.01));
	if(KeyDown['v'])myLPVApp->m_ls.updateDirectionVector(glm::vec3(0.0, 0.0, -0.01));

	if(mousedown)
	{
		myLPVApp->m_Camera.camRotate(deltax * 0.5 * tslf, glm::vec3(0.0,1.0,0.0));
		myLPVApp->m_Camera.camRotate(deltay * 0.5 * tslf, glm::vec3(1.0,0.0,0.0),TS_LOCAL);
	}

	myLPVApp->RenderScene();
	glutSwapBuffers();
}

void OnResize(int w, int h)
{
	//does NOT work atm
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
}
void OnKey(unsigned char key, int x, int y)
{
    /*switch (key)
	{
        case ' ':

        break;
        case 'r': case 'R':
            if (filling==0)
            {
                glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
                filling=1;
            }
            else
            {
                glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); // Polygon rasterization mode (polygon outlined)
                filling=0;
            }
        break;
    }*/
	if(key == 27)//escape
	{
		glutDestroyWindow (winID);
		exit(0);
	}
	KeyDown[key] = true;
}
void OnKeyUp(unsigned char key, int x, int y)
{
	KeyDown[key] = false;
}

void OnSpecialKey(int key, int x, int y)
{
	SpecialKeyDown[key] =  true;
}

void OnSpecialKeyUp(int key, int x, int y)
{
	SpecialKeyDown[key] =  false;
}

void OnIdle() {
	glutPostRedisplay();
}
void MouseButton(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN && button == GLUT_LEFT)mousedown = true;
	if(state == GLUT_UP && button == GLUT_LEFT)mousedown = false;
	mousex = x;
	mousey = y;
	glutPostRedisplay();
}
void MouseMotion(int x, int y)
{
  deltax = x - mousex;
  deltay = y - mousey;
  glutPostRedisplay();
}

void main(int argc, char** argv) {

	for(unsigned int i = 0; i<256 ; i++){
		KeyDown[i]= false;
		SpecialKeyDown[i] = false;
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion (3, 3);
	glutInitContextFlags (GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitWindowSize(m_windowWidth, m_windowHeight);
	winID = glutCreateWindow("Test - OpenGL 3.3");
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)	{
		cerr<<"Error: "<<glewGetErrorString(err)<<endl;
	} else {
		if (GLEW_VERSION_3_3)
		{
			cout<<"Driver supports OpenGL 3.3\nDetails:"<<endl;
		}
	}
	cout<<"Using GLEW "<<glewGetString(GLEW_VERSION)<<endl;
	cout<<"Vendor: "<<glGetString (GL_VENDOR)<<endl;
	cout<<"Renderer: "<<glGetString (GL_RENDERER)<<endl;
	cout<<"Version: "<<glGetString (GL_VERSION)<<endl;
	cout<<"GLSL: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<endl;
	InitGL();

	myLPVApp = new LPVApp();
	myLPVApp->SetScreenSize(m_windowWidth, m_windowHeight);
	myLPVApp->Initialize();

	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnResize);
	glutKeyboardFunc(OnKey);
	glutKeyboardUpFunc(OnKeyUp);
	glutSpecialFunc(OnSpecialKey);
	glutSpecialUpFunc(OnSpecialKeyUp);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutIdleFunc(OnIdle);
	time_start = clock();
	time_last = time_start;
	//glutWarpPointer(m_windowWidth / 2, m_windowHeight / 2);
	glutMainLoop();
}
