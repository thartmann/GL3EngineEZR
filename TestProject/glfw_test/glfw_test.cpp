#include "glfw_test.h"

/// TODO (anybody#1#): There is some refactoring needed. Most of the code in this class should be moved to the main to be able to use GLFWs callback functions.

/// TODO (anybody#1#): We need a scene graph. Found a nice article on how to write one at http://scriptionary.com/2009/02/17/simple-scene-graph-in-c/



glfw_test::glfw_test(void)
{
    m_running = false;

    m_scene = 0;
    m_shader = 0;

    m_windowWidth = 512;
    m_windowHeight = 512;

    Initialize();
}


glfw_test::~glfw_test(void)
{
    delete m_scene;

	m_shader->UnBindShader();
    delete m_shader;

    DeleteVBO();

    DestroyAILogger();
}

/// TODO (anybody#1#): this should probably be replaced with GLFW native commands
void glfw_test::CheckVersion(void)
{
    GLint maxTex1, maxTex2, MajorVersion, MinorVersion, numext, pointSize, uniformSize;
    std::stringstream tmp;

    tmp << "OpenGL " << glGetString(GL_VERSION);
    LogInfo(tmp.str());
    tmp.str("");

    glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);
    tmp << "Version " << MajorVersion << "." << MinorVersion;
    LogInfo(tmp.str());
    tmp.str("");

    tmp << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION);
    LogInfo(tmp.str());
    tmp.str("");

    glGetIntegerv(GL_POINT_SIZE, &pointSize);
    tmp << "Point Size " << pointSize;
    LogInfo(tmp.str());
    tmp.str("");

    tmp << "Hardware " << glGetString(GL_VENDOR) << " - " << glGetString(GL_RENDERER);
    LogInfo(tmp.str());
    tmp.str("");

    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxTex1);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTex2);
    tmp << "MaxTex " << maxTex1 << " " << maxTex2;
    LogInfo(tmp.str());
    tmp.str("");

    glGetIntegerv(GL_NUM_EXTENSIONS, &numext);
    tmp << "Found " << numext << " GL_EXTENSIONS";
    LogInfo(tmp.str());
    tmp.str("");

    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &uniformSize);
    tmp << "GL_MAX_VERTEX_UNIFORM_BLOCKS " << uniformSize;
    LogInfo(tmp.str());
    tmp.str("");

    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &uniformSize);
    tmp << "GL_MAX_FRAGMENT_UNIFORM_BLOCKS " << uniformSize;
    LogInfo(tmp.str());
    tmp.str("");

    glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &uniformSize);
    tmp << "GL_MAX_GEOMETRY_UNIFORM_BLOCKS " << uniformSize;
    LogInfo(tmp.str());
    tmp.str("");

    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &uniformSize);
    tmp << "GL_MAX_UNIFORM_BLOCK_SIZE " << uniformSize;
    LogInfo(tmp.str());
    tmp.str("");
}


int glfw_test::Initialize(void)
{
	m_rotX = 0.0f;
	m_rotY = 0.0f;

    CreateAILogger();

    if (!glfwInit())
    {
        LogInfo("Initialization of GLFW failed!");

        return EXIT_FAILURE;
    }

    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!glfwOpenWindow( m_windowWidth, m_windowHeight, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
    {
        glfwTerminate();

        LogInfo("GLFW opening of window failed!");

        return EXIT_FAILURE;
    }

    glfwSetWindowTitle("GLFW Test Application");

    //turn vsync on
    glfwSwapInterval(1);

    LogDebug("OpenGL window context version: " + glfwGetWindowParam(GLFW_OPENGL_PROFILE));

    CheckVersion();

   	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	m_modelMatrix = glm::core::type::mat4(1.0f);
	m_viewMatrix = glm::core::type::mat4(1.0f);
	m_viewMatrix = glm::gtc::matrix_transform::translate(m_viewMatrix, glm::core::type::vec3(0.0f, 0.0f, -2.0f));
    m_projectionMatrix = glm::gtc::matrix_transform::perspective(60.0f, (float)m_windowWidth / (float)m_windowHeight, 0.1f, 100.0f);

	SetupScene();

    if (MainLoop() != 0)
    {
        LogInfo("Failure in main loop!");

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


///from assimp/samples/SimpleTexturedOpenGL
void glfw_test::CreateAILogger(void)
{
	//Assimp::Logger::LogSeverity severity = Assimp::Logger::NORMAL;
	Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;

	// Create a logger instance for Console Output
	Assimp::DefaultLogger::create("",severity, aiDefaultLogStream_STDOUT);

	// Create a logger instance for File Output (found in project folder or near .exe)
//	Assimp::DefaultLogger::create("glfw_test_log.txt",severity, aiDefaultLogStream_FILE);

	// Now I am ready for logging my stuff
	Assimp::DefaultLogger::get()->info("this is my info-call");
}


///from assimp/samples/SimpleTexturedOpenGL
void glfw_test::DestroyAILogger(void)
{
	// Kill it after the work is done
	Assimp::DefaultLogger::kill();
}


///from assimp/samples/SimpleTexturedOpenGL
void glfw_test::LogInfo(std::string logString)
{
	//Will add message to File with "info" Tag
	Assimp::DefaultLogger::get()->info(logString.c_str());
}


///from assimp/samples/SimpleTexturedOpenGL
void glfw_test::LogDebug(const char* logString)
{
	//Will add message to File with "debug" Tag
	Assimp::DefaultLogger::get()->debug(logString);
}


///from assimp/samples/SimpleTexturedOpenGL
bool glfw_test::Import3DFromFile( const std::string& pFile)
{
	//check if file exists
	std::ifstream fin(pFile.c_str());
	if (!fin.fail())
	{
		fin.close();
	}
	else
	{
		LogInfo("Couldn't open file: " + pFile);
		LogInfo( m_importer.GetErrorString());
		return false;
	}

	m_scene = m_importer.ReadFile( pFile, aiProcessPreset_TargetRealtime_Quality);

	// If the import failed, report it
	if (!m_scene)
	{
		LogInfo( m_importer.GetErrorString());
		return false;
	}

	// Now we can access the file's contents.
	LogInfo("Import of scene " + pFile + " succeeded.");

	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}

/// TODO (anybody#1#): The Model seems to get loaded. We need to build the VBO from it now. assimp documentation gives hints on how to do this and it seems not to be very complicated as the scene that gets created by the loader seems to be very structured. Nevertheless we need a scene graph...
void glfw_test::SetupScene(void)
{
//    if (!Import3DFromFile("./relativitystairs.blend"))
//    {
//        LogInfo("Import of file failed!");
//
//        return;
//    }

    MakeVBO();
}

void glfw_test::MakeVBO(void)
{
	const Vertex vertices[8] =
	{
		{ { -0.5f, -0.5f,  0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { -0.5f,  0.5f,  0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ {  0.5f,  0.5f,  0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ {  0.5f, -0.5f,  0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -0.5f,  0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ {  0.5f,  0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
		{ {  0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
	};

	const GLuint indices[36] =
	{
		0,2,1,  0,3,2,
		4,3,0,  4,7,3,
		4,1,5,  4,0,1,
		3,6,2,  3,7,6,
		1,6,5,  1,2,6,
		7,5,6,  7,4,5
	};

	GLenum errorCheckValue = glGetError();

	m_shader = new Shader("simpleVP.vert", "simpleFP.frag");

	m_modelMatrixUniformLocation = glGetUniformLocation(m_shader->GetShaderID(), "ModelMatrix");
	m_viewMatrixUniformLocation = glGetUniformLocation(m_shader->GetShaderID(), "ViewMatrix");
	m_projectionMatrixUniformLocation = glGetUniformLocation(m_shader->GetShaderID(), "ProjectionMatrix");
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not get shader uniform locations!" << std::endl;
		return;
	}

	glGenVertexArrays(1, &m_vaoID);
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not create VAO!" << std::endl;
		return;
	}
	glBindVertexArray(m_vaoID);
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not bind VAO!" << std::endl;
		return;
	}

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not enable vertex attributes!" << std::endl;
		return;
	}

	glGenBuffers(2, &m_vboID[0]);
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not create buffer objects!" << std::endl;
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not bind VBO to VAO!" << std::endl;
		return;
	}

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (GLvoid*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (GLvoid*)sizeof(vertices[0].position));
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not set VAO attributes!" << std::endl;
		return;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboID[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not bind the IBO to the VAO!" << std::endl;
		return;
	}

	glBindVertexArray(0);

	errorCheckValue = glGetError();
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not create VBO!" << std::endl;
		return;
	}
}

void glfw_test::DeleteVBO(void)
{
	GLenum errorCheckValue = glGetError();

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, &m_vboID[0]);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_vaoID);

	errorCheckValue = glGetError();
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not delete VBO!" << std::endl;
		return;
	}
}


void glfw_test::DrawVBO(void)
{
	GLenum errorCheckValue = glGetError();

	m_modelMatrix = glm::gtc::matrix_transform::rotate(m_modelMatrix, m_rotX, glm::core::type::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::gtc::matrix_transform::rotate(m_modelMatrix, m_rotY, glm::core::type::vec3(0.0f, 1.0f, 0.0f));

//    std::cout << "[DEBUG] m_viewMatrix" << std::endl
//    << glm::value_ptr(m_viewMatrix)[0] << " " << glm::value_ptr(m_viewMatrix)[4] << " " << glm::value_ptr(m_viewMatrix)[8] << " " << glm::value_ptr(m_viewMatrix)[12] << std::endl
//    << glm::value_ptr(m_viewMatrix)[1] << " " << glm::value_ptr(m_viewMatrix)[5] << " " << glm::value_ptr(m_viewMatrix)[9] << " " << glm::value_ptr(m_viewMatrix)[13] << std::endl
//    << glm::value_ptr(m_viewMatrix)[2] << " " << glm::value_ptr(m_viewMatrix)[6] << " " << glm::value_ptr(m_viewMatrix)[10] << " " << glm::value_ptr(m_viewMatrix)[14] << std::endl
//    << glm::value_ptr(m_viewMatrix)[3] << " " << glm::value_ptr(m_viewMatrix)[7] << " " << glm::value_ptr(m_viewMatrix)[11] << " " << glm::value_ptr(m_viewMatrix)[15] << std::endl
//    << "[DEBUG]" << std::endl;
//
//    std::cout << "[DEBUG] m_modelMatrix" << std::endl
//    << glm::value_ptr(m_modelMatrix)[0] << " " << glm::value_ptr(m_modelMatrix)[4] << " " << glm::value_ptr(m_modelMatrix)[8] << " " << glm::value_ptr(m_modelMatrix)[12] << std::endl
//    << glm::value_ptr(m_modelMatrix)[1] << " " << glm::value_ptr(m_modelMatrix)[5] << " " << glm::value_ptr(m_modelMatrix)[9] << " " << glm::value_ptr(m_modelMatrix)[13] << std::endl
//    << glm::value_ptr(m_modelMatrix)[2] << " " << glm::value_ptr(m_modelMatrix)[6] << " " << glm::value_ptr(m_modelMatrix)[10] << " " << glm::value_ptr(m_modelMatrix)[14] << std::endl
//    << glm::value_ptr(m_modelMatrix)[3] << " " << glm::value_ptr(m_modelMatrix)[7] << " " << glm::value_ptr(m_modelMatrix)[11] << " " << glm::value_ptr(m_modelMatrix)[15] << std::endl
//    << "[DEBUG]" << std::endl;
//
//    std::cout << "[DEBUG] m_projectionMatrix" << std::endl
//    << glm::value_ptr(m_projectionMatrix)[0] << " " << glm::value_ptr(m_projectionMatrix)[4] << " " << glm::value_ptr(m_projectionMatrix)[8] << " " << glm::value_ptr(m_projectionMatrix)[12] << std::endl
//    << glm::value_ptr(m_projectionMatrix)[1] << " " << glm::value_ptr(m_projectionMatrix)[5] << " " << glm::value_ptr(m_projectionMatrix)[9] << " " << glm::value_ptr(m_projectionMatrix)[13] << std::endl
//    << glm::value_ptr(m_projectionMatrix)[2] << " " << glm::value_ptr(m_projectionMatrix)[6] << " " << glm::value_ptr(m_projectionMatrix)[10] << " " << glm::value_ptr(m_projectionMatrix)[14] << std::endl
//    << glm::value_ptr(m_projectionMatrix)[3] << " " << glm::value_ptr(m_projectionMatrix)[7] << " " << glm::value_ptr(m_projectionMatrix)[11] << " " << glm::value_ptr(m_projectionMatrix)[15] << std::endl
//    << "[DEBUG]" << std::endl;

	m_shader->BindShader();

	glUniformMatrix4fv(m_modelMatrixUniformLocation, 1, GL_FALSE, glm::gtc::type_ptr::value_ptr(m_modelMatrix));
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not assign modelMatrix!" << std::endl;
		return;
	}

	glUniformMatrix4fv(m_viewMatrixUniformLocation, 1, GL_FALSE, glm::gtc::type_ptr::value_ptr(m_viewMatrix));
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not assign viewMatrix!" << std::endl;
		return;
	}

	glUniformMatrix4fv(m_projectionMatrixUniformLocation, 1, GL_FALSE, glm::gtc::type_ptr::value_ptr(m_projectionMatrix));
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not assign viewMatrix!" << std::endl;
		return;
	}

	glBindVertexArray(m_vaoID);
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not bind VAO!" << std::endl;
		return;
	}

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLvoid *)0);
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not draw VBO!" << std::endl;
		return;
	}

	glBindVertexArray(0);
//	m_shader->UnBindShader();
}


void glfw_test::RenderScene(void)
{
    	glViewport(0, 0, m_windowWidth, m_windowHeight);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		DrawVBO();

        glfwSwapBuffers();

        glfwGetWindowSize(&m_windowWidth, &m_windowHeight);
        if (m_windowHeight <= 0)
		{
			m_windowHeight = 1;
		}
}


int glfw_test::MainLoop()
{
    m_running = true;

    while(m_running)
    {
		RenderScene();

		if (glfwGetKey('D'))
		{
			m_rotX += 0.2f;
//			std::cout << "rotX: " << m_rotX << std::endl;
		}
		if (glfwGetKey('A'))
		{
			m_rotX -= 0.2f;
//			std::cout << "rotX: " << m_rotX << std::endl;
		}
		if (glfwGetKey('W'))
		{
			m_rotY += 0.2f;
//			std::cout << "rotY: " << m_rotY << std::endl;
		}
		if (glfwGetKey('S'))
		{
			m_rotY -= 0.2f;
//			std::cout << "rotY: " << m_rotY << std::endl;
		}
		if (glfwGetKey('C'))
		{
			m_rotX = 0.0f;
			m_rotY = 0.0f;
		}
		if (glfwGetKey('E'))
		{
			m_viewMatrix = glm::gtc::matrix_transform::translate(m_viewMatrix, glm::core::type::vec3(0.0f, 0.0f, 0.2f));
		}
		if (glfwGetKey('Q'))
		{
			m_viewMatrix = glm::gtc::matrix_transform::translate(m_viewMatrix, glm::core::type::vec3(0.0f, 0.0f, -0.2f));
		}


        // exit if ESC was pressed or window was closed
        m_running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }

    glfwCloseWindow();

    glfwTerminate();

    return EXIT_SUCCESS;
}
