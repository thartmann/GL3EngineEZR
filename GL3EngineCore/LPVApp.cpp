#include "LPVApp.h"

LPVApp::LPVApp(void)
{
	rtID = 0;
}


LPVApp::~LPVApp(void)
{
}


int LPVApp::Initialize(void)
{
    CheckVersion();
   	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	if (!CheckOpenGLError("init")) return EXIT_FAILURE;

	m_modelMatrix = glm::mat4(1.0f);
	m_viewMatrix = glm::mat4(1.0f);
	m_viewMatrix = glm::translate(m_viewMatrix, glm::core::type::vec3(0.0f, 0.0f, -2.0f));
    m_projectionMatrix = glm::perspective(60.0f, (float)m_windowWidth / (float)m_windowHeight, 0.1f, 100.0f);

	m_rootNode = new Node("Root");


	m_Camera.camPerspective(60.0f, (float)m_windowWidth / (float)m_windowHeight, 0.1f, 100.0f);
	//m_Camera.camMove(glm::vec3(0.0f, 0.0f, -2.0f));

	SetupScene();

	if (!CheckOpenGLError("setup")) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


void LPVApp::RenderScene(void)
{
	////Set framebuffer 
	//m_mrt->useMRT();
	//CheckOpenGLError("using mrt");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, m_windowWidth, m_windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_shader->BindShader();

	//glUniformMatrix4fv(m_modelMatrixUniformLocation, 1, GL_FALSE, glm::gtc::type_ptr::value_ptr(m_modelMatrix));
	glUniformMatrix4fv(m_viewMatrixUniformLocation, 1, GL_FALSE, glm::gtc::type_ptr::value_ptr(m_Camera.getView()));
	glUniformMatrix4fv(m_projectionMatrixUniformLocation, 1, GL_FALSE, glm::gtc::type_ptr::value_ptr(m_Camera.getProjection()));
	if (!CheckOpenGLError("assigning matrices")) return;

	m_shader->UnBindShader();
	
	m_rootNode->Render(m_nodeTexLoc, m_modelMatrixUniformLocation);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	////draw texture from render target into full screen quad
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//CheckOpenGLError("binding framebuffer 0");

	//glViewport(0, 0, m_windowWidth, m_windowHeight);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	//CheckOpenGLError("glClear()");

	//m_outputShader->BindShader();

	////switch to tell shader wich texture to render
	//glUniform1i(m_outputSwitch, rtID);

	//CheckOpenGLError("setting uniforms");

	//if (!m_outputShader->BindShader()) std::cout << "[ERROR] m_outputShader->BindShader() in RenderScene()!"  << std::endl;

	////activate and bind textures
	//for (size_t i = 0; i < 5; i++)
	//{
	//	glActiveTexture(GL_TEXTURE0 + m_mrt->getTextureUnit(i));
	//	glBindTexture(GL_TEXTURE_2D, m_mrt->getTexture(i));
	//	glBindSampler(m_mrt->getTextureUnit(i), m_mrt->getSampler(i));
	//}

	//CheckOpenGLError("binding textures");

	//glEnableVertexAttribArray(3);
	//CheckOpenGLError("enable FSQ");
	//glBindBuffer(GL_ARRAY_BUFFER, m_mrt->getFSQbuffer() );
	//CheckOpenGLError("bind FSQ");

	//glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 0,	(void*)0 );
	//CheckOpenGLError("attrib FSQ");
	//glDrawArrays( GL_TRIANGLES, 0, 6 );

	//CheckOpenGLError("draw FSQ");

	//m_outputShader->UnBindShader();

	//glDisableVertexAttribArray(3);
}


void LPVApp::SetScreenSize(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}


void LPVApp::CheckVersion(void)
{
    GLint maxTex1, maxTex2, MajorVersion, MinorVersion, numext, pointSize, uniformSize;
    std::stringstream tmp;

    tmp << "OpenGL " << glGetString(GL_VERSION);
    std::cout << tmp.str() << std::endl;
    tmp.str("");

    glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);
    tmp << "Version " << MajorVersion << "." << MinorVersion;
    std::cout << tmp.str() << std::endl;
    tmp.str("");

    tmp << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << tmp.str() << std::endl;
    tmp.str("");

    glGetIntegerv(GL_POINT_SIZE, &pointSize);
    tmp << "Point Size " << pointSize;
    std::cout << tmp.str() << std::endl;
    tmp.str("");

    tmp << "Hardware " << glGetString(GL_VENDOR) << " - " << glGetString(GL_RENDERER);
    std::cout << tmp.str() << std::endl;
    tmp.str("");

    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxTex1);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTex2);
    tmp << "MaxTex " << maxTex1 << " " << maxTex2;
    std::cout << tmp.str() << std::endl;
    tmp.str("");

    glGetIntegerv(GL_NUM_EXTENSIONS, &numext);
    tmp << "Found " << numext << " GL_EXTENSIONS";
    std::cout << tmp.str() << std::endl;
    tmp.str("");

    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &uniformSize);
    tmp << "GL_MAX_VERTEX_UNIFORM_BLOCKS " << uniformSize;
    std::cout << tmp.str() << std::endl;
    tmp.str("");

    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &uniformSize);
    tmp << "GL_MAX_FRAGMENT_UNIFORM_BLOCKS " << uniformSize;
    std::cout << tmp.str() << std::endl;
    tmp.str("");

    glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &uniformSize);
    tmp << "GL_MAX_GEOMETRY_UNIFORM_BLOCKS " << uniformSize;
    std::cout << tmp.str() << std::endl;
    tmp.str("");

    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &uniformSize);
    tmp << "GL_MAX_UNIFORM_BLOCK_SIZE " << uniformSize;
    std::cout << tmp.str() << std::endl;
    tmp.str("");
}


void LPVApp::SetupScene(void)
{
	Texture * texture = new Texture();
    texture->LoadTGA("./textures/dragontex.tga");

	Texture * texture2 = new Texture();
    texture2->LoadTGA("./textures/dragontex2.tga");

	Mesh* mesh = new Mesh();
    mesh->loadFromFile("./models/dragon.blend");
    mesh->init();

    m_shader = new Shader("./shader/simpleVP.vert", "./shader/simpleFP.frag");

	m_rootNode->nodeAttach(m_shader);

	for(GLuint i = 0; i < 30; i++)
	{
		Node* node = new Node(("Dragon "+ i));
		
		node->nodeAttach(mesh);
		node->nodeAttach(m_shader);

		if(i%2)node->nodeMove(glm::vec3(1.0 * i,0.0,-1.0 * i));
		else node->nodeMove(glm::vec3(-1.0 * i,0.0,-1.0 * i));

		if(i%3)node->nodeAttach(texture);
		else node->nodeAttach(texture2);

		node->nodeRotate(30.0f * i,glm::vec3(0.0,1.0,0.0));
		m_rootNode->addChild(node);
	}

	m_Camera.camLookAt(glm::vec3(0.0,2.0,2.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0));

	m_shader->BindShader();

   	m_modelMatrixUniformLocation = glGetUniformLocation(m_shader->GetShaderID(), "ModelMatrix");
	m_viewMatrixUniformLocation = glGetUniformLocation(m_shader->GetShaderID(), "ViewMatrix");
	m_projectionMatrixUniformLocation = glGetUniformLocation(m_shader->GetShaderID(), "ProjectionMatrix");
	if (!CheckOpenGLError("shader uniform locations")) return;
	
	m_nodeTexLoc = glGetUniformLocation(m_shader->GetShaderID(), "assignedtexture");
	CheckOpenGLError("getting uniform location in node");

	m_shader->UnBindShader();

	m_mrt = new MultipleRenderTarget();
	m_mrt->createMRT( m_windowWidth, m_windowHeight );
	m_mrt->createFSQ();

	m_outputShader = new Shader("./shader/outputVP.vert", "./shader/outputFP.frag");

	//get texture locations
	m_colorMapLocation = glGetUniformLocation(m_outputShader->GetShaderID(), "colorMap");
	CheckOpenGLError("getting colorMap uniform location");
	m_positionMapLocation = glGetUniformLocation(m_outputShader->GetShaderID(), "positionMap");
	CheckOpenGLError("getting positionMap uniform location");
	m_normalMapLocation = glGetUniformLocation(m_outputShader->GetShaderID(), "normalMap");
	CheckOpenGLError("getting normalMap uniform location");
	m_fluxMapLocation = glGetUniformLocation(m_outputShader->GetShaderID(), "fluxMap");
	CheckOpenGLError("getting fluxMap uniform location");
	m_depthMapLocation = glGetUniformLocation(m_outputShader->GetShaderID(), "depthMap");
	CheckOpenGLError("getting depthMap uniform location");

	//get location for switch
	m_outputSwitch = glGetUniformLocation(m_outputShader->GetShaderID(), "outputSwitch");
	CheckOpenGLError("getting outputSwitch uniform location");

	if (!m_outputShader->BindShader()) std::cout << "[ERROR] m_outputShader->BindShader() in SetupScene()!"  << std::endl;

	//pass textures to shader
	glUniform1i(m_colorMapLocation, m_mrt->getTextureUnit(0));
	glUniform1i(m_positionMapLocation, m_mrt->getTextureUnit(1));
	glUniform1i(m_normalMapLocation, m_mrt->getTextureUnit(2));
	glUniform1i(m_fluxMapLocation, m_mrt->getTextureUnit(3));
	glUniform1i(m_depthMapLocation, m_mrt->getTextureUnit(4));

	m_outputShader->UnBindShader();
}

void LPVApp::showRenderTarget(int value)
{
	rtID = value;
}