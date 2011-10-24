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
	m_lightViewMatrix = glm::lookAt(glm::vec3(1.0,3.0,1.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0));
	m_lightProjectionMatrix = glm::perspective(60.0f, 1.0f, 0.5f, 20.0f);
    m_projectionMatrix = glm::perspective(60.0f, (float)m_windowWidth / (float)m_windowHeight, 0.5f, 20.0f);


	m_rootNode = new Node("Root");

	m_Camera.camPerspective(60.0f, (float)m_windowWidth / (float)m_windowHeight, 0.5f, 20.0f);

	//generate random Texture
	GLfloat noise[48];

	for (GLuint i = 0; i < 48; ++i)
	{
		glm::vec3 randVec(random(-1.0f, 1.0f), random(-1.0f, 1.0f), 0.0f);
		randVec = glm::normalize(randVec);
		noise[i] = randVec.x;
		i++;
		noise[i] = randVec.y;
		i++;
		noise[i] = randVec.z;
	}

	m_randomMapUnit = 5;

	glGenTextures(1, &m_randomMapID);

	glBindTexture( GL_TEXTURE_2D, m_randomMapID );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_FLOAT, noise);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenSamplers(1, &m_randomMapSampler);
	glSamplerParameteri(m_randomMapSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(m_randomMapSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(m_randomMapSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(m_randomMapSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//generate sample kernel
	m_sampleKernelSize = 48;
	for (GLuint i = 0; i < m_sampleKernelSize; ++i)
	{
		glm::vec3 randVec(random(-1.0f, 1.0f), random(-1.0f, 1.0f), random(0.0f, 1.0f));
		randVec = glm::normalize(randVec);
		randVec *= random(0.0f, 1.0f);
		float scale = float(i) / float(m_sampleKernelSize);
		scale = lerp(0.1f, 1.0f, scale * scale);
		randVec *= scale;
		m_sampleKernel[i] = randVec.x;
		++i;
		m_sampleKernel[i] = randVec.y;
		++i;
		m_sampleKernel[i] = randVec.z;
	}

	SetupScene();

	//LightSpaceMatrix
	glm::mat4 bias = glm::mat4(1.0f);
	bias = glm::translate(bias, glm::vec3(0.5f,0.5f,0.5f));
	bias = glm::scale(bias, glm::vec3(0.5f,0.5f,0.5f));
	m_lightSpaceMatrix = bias * (m_lightProjectionMatrix * m_lightViewMatrix);
	
	if (!CheckOpenGLError("setup")) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


void LPVApp::RenderScene(void)
{
	m_ls.updatePosition( m_lightArray, 1 );
	m_ls.updateDirection( m_lightArray, 1 );

	//LIGHTINJECTIONPASS

	m_light_mrt->useMRT();
	glViewport(0, 0, 1024, 1024);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_LightInjectionShader->BindShader();

	glUniformMatrix4fv(m_lightViewMatrixUniformLocation, 1, GL_FALSE, glm::gtc::type_ptr::value_ptr(m_lightViewMatrix));
	glUniformMatrix4fv(m_lightProjectionMatrixUniformLocation, 1, GL_FALSE, glm::gtc::type_ptr::value_ptr(m_lightProjectionMatrix));
	//GLint locus = glGetUniformLocation( m_LightInjectionShader->GetShaderID(), "megaLightArray");
	//glUniform3fv( locus , m_ls.getCounter() * 4  , m_lightArray );

	m_rootNode->Render(m_nodeTexLoc2, m_modelMatrixUniformLocation2, m_lightNormalMatrixLoc, m_lightViewMatrix);

	m_LightInjectionShader->UnBindShader();

	//RENDERPASS
	m_mrt->useMRT();
	CheckOpenGLError("using mrt");

	glViewport(0, 0, m_windowWidth, m_windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_shader->BindShader();

	GLint loc = glGetUniformLocation( m_shader->GetShaderID(), "m_lightArray");
	glUniform3fv( loc , m_ls.getCounter() * 4  , m_lightArray );

	glUniform1f(m_farPlaneLoc, m_Camera.getFar());
	
	glUniformMatrix4fv(m_viewMatrixUniformLocation, 1, GL_FALSE, glm::gtc::type_ptr::value_ptr(m_Camera.getView()));
	glUniformMatrix4fv(m_projectionMatrixUniformLocation, 1, GL_FALSE, glm::gtc::type_ptr::value_ptr(m_Camera.getProjection()));
	glUniformMatrix4fv(m_lightSpaceMatrixUniformLocation, 1, GL_FALSE, glm::gtc::type_ptr::value_ptr(m_lightSpaceMatrix));

	if (!CheckOpenGLError("assigning matrices")) return;

	glActiveTexture(GL_TEXTURE0 + m_light_mrt->getTextureUnit(4));
	glBindTexture(GL_TEXTURE_2D, m_light_mrt->getTexture(4));
	glBindSampler(m_light_mrt->getTextureUnit(4), m_light_mrt->getSampler(4));

	m_rootNode->Render(m_nodeTexLoc, m_modelMatrixUniformLocation, m_normalMatrixLoc, m_Camera.getView());
	m_shader->UnBindShader();

	//FULLSCREENQUAD
	//draw texture from render target into full screen quad
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	CheckOpenGLError("binding framebuffer 0");

	glViewport(0, 0, m_windowWidth, m_windowHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	CheckOpenGLError("glClear()");

	m_outputShader->BindShader();

	glUniformMatrix4fv(m_projMatForSSAOLoc, 1, GL_FALSE, glm::gtc::type_ptr::value_ptr(m_Camera.getProjection()));
	glUniformMatrix4fv(m_viewMatForSSAOLoc, 1, GL_FALSE, glm::gtc::type_ptr::value_ptr(m_Camera.getView()));

	//switch to tell shader wich texture to render
	glUniform1i(m_outputSwitch, rtID);

	//uniform to pass viewport dimensions
	GLint viewportDim[2] = {m_windowWidth, m_windowHeight};
	glUniform2f(m_viewportDimensionLoc, (float)m_windowWidth, (float)m_windowHeight);

	CheckOpenGLError("setting uniforms");

	m_outputShader->BindShader();
	CheckOpenGLError("m_outputShader->BindShader() in RenderScene()");

	//activate and bind textures
	for (size_t i = 0; i < 5; i++)
	{
		glActiveTexture(GL_TEXTURE0 + m_mrt->getTextureUnit(i));
		glBindTexture(GL_TEXTURE_2D, m_mrt->getTexture(i));
		glBindSampler(m_mrt->getTextureUnit(i), m_mrt->getSampler(i));
	}

	glActiveTexture(GL_TEXTURE0 + m_randomMapUnit);
	glBindTexture(GL_TEXTURE_2D, m_randomMapID);
	glBindSampler(m_randomMapUnit, m_randomMapSampler);

	CheckOpenGLError("binding textures");

	glEnableVertexAttribArray(3);
	CheckOpenGLError("enable FSQ");
	glBindBuffer(GL_ARRAY_BUFFER, m_mrt->getFSQbuffer() );
	CheckOpenGLError("bind FSQ");

	glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 0,	(void*)0 );
	CheckOpenGLError("attrib FSQ");
	glDrawArrays( GL_TRIANGLES, 0, 6 );

	CheckOpenGLError("draw FSQ");

	m_outputShader->UnBindShader();
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
	//RENDERTARGETS
	m_mrt = new MultipleRenderTarget();
	m_mrt->createMRT( m_windowWidth, m_windowHeight );
	m_mrt->createFSQ();

	m_light_mrt = new MultipleRenderTarget();
	m_light_mrt->createMRT(1024,1024);

	//RENDERSHADER
	m_shader = new Shader("./shader/gatherVP.vert", "./shader/gatherFP.frag");
	m_shader->BindShader();

	m_farPlaneLoc = glGetUniformLocation(m_shader->GetShaderID(), "farPlane");
	m_normalMatrixLoc = glGetUniformLocation(m_shader->GetShaderID(), "normalMat");

   	m_modelMatrixUniformLocation = glGetUniformLocation(m_shader->GetShaderID(), "ModelMatrix");
	m_viewMatrixUniformLocation = glGetUniformLocation(m_shader->GetShaderID(), "ViewMatrix");
	m_projectionMatrixUniformLocation = glGetUniformLocation(m_shader->GetShaderID(), "ProjectionMatrix");
	m_lightSpaceMatrixUniformLocation = glGetUniformLocation(m_shader->GetShaderID(), "LightSpaceMatrix");
	m_nodeTexLoc = glGetUniformLocation(m_shader->GetShaderID(), "assignedtexture");
	CheckOpenGLError("getting uniform location in node");

	m_shadowMapLocation = glGetUniformLocation(m_shader->GetShaderID(), "shadowMap");

	glUniform1i(m_shadowMapLocation, m_mrt->getTextureUnit(4));

	m_shader->UnBindShader();

	
	//INJECTIONSHADER
	m_LightInjectionShader = new Shader("./shader/injectionVP.vert", "./shader/injectionFP.frag");
	m_LightInjectionShader->BindShader();

	m_modelMatrixUniformLocation2 = glGetUniformLocation(m_LightInjectionShader->GetShaderID(), "ModelMatrix");
	m_lightViewMatrixUniformLocation = glGetUniformLocation(m_LightInjectionShader->GetShaderID(), "LightViewMatrix");
	m_lightProjectionMatrixUniformLocation = glGetUniformLocation(m_LightInjectionShader->GetShaderID(), "LightProjectionMatrix");
	m_nodeTexLoc2 = glGetUniformLocation(m_shader->GetShaderID(), "meshtexture");

	m_lightNormalMatrixLoc = glGetUniformLocation(m_LightInjectionShader->GetShaderID(), "normalMat");

	m_LightInjectionShader->UnBindShader();

	//OUTPUTSHADER
	m_outputShader = new Shader("./shader/finalVP.vert", "./shader/finalFP.frag");

	//get matrix location
	m_projMatForSSAOLoc = glGetUniformLocation(m_outputShader->GetShaderID(), "projMat");
	CheckOpenGLError("getting projMatForSSAO uniform location");

	m_viewMatForSSAOLoc = glGetUniformLocation(m_outputShader->GetShaderID(), "viewMat");
	CheckOpenGLError("getting viewMatForSSAO uniform location");

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
	m_randomMapLoc = glGetUniformLocation(m_outputShader->GetShaderID(), "randMap");

	//get location for switch
	m_outputSwitch = glGetUniformLocation(m_outputShader->GetShaderID(), "outputSwitch");
	CheckOpenGLError("getting outputSwitch uniform location");
	m_viewportDimensionLoc = glGetUniformLocation(m_outputShader->GetShaderID(), "viewport");
	CheckOpenGLError("getting viewportDimension uniform location");

	m_sampleKernelLoc = glGetUniformLocation(m_outputShader->GetShaderID(), "sampleKernel");
	CheckOpenGLError("getting sample kernel uniform location");

	m_outputShader->BindShader();
	CheckOpenGLError("m_outputShader->BindShader() in SetupScene()");

	//pass textures to shader
	glUniform1i(m_colorMapLocation, m_mrt->getTextureUnit(0));
	glUniform1i(m_positionMapLocation, m_mrt->getTextureUnit(1));
	glUniform1i(m_normalMapLocation, m_mrt->getTextureUnit(2));
	glUniform1i(m_fluxMapLocation, m_mrt->getTextureUnit(3));
	glUniform1i(m_depthMapLocation, m_mrt->getTextureUnit(4));

	glUniform1i(m_randomMapLoc, m_randomMapUnit);

	glUniform3fv(m_sampleKernelLoc, m_sampleKernelSize, m_sampleKernel);

	m_outputShader->UnBindShader();


	//LIGHTS
	m_lightArray = new float[96];

	m_ls.addLight( glm::vec3(5.0,3.0,0.0), glm::vec3(0.5,1.0,0.5), glm::vec3(1.0,0.0,0.0), 0.75, LT_SPOT );
	m_ls.addLight( glm::vec3(0.0,10.0,0.0), glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0), 0.7, LT_SPOT );
	
	m_ls.getLightArray( m_lightArray );

	//SCENE
	Texture * texture = new Texture();
    texture->LoadTGA("./textures/dragontex.tga");

	Texture * texture2 = new Texture();
    texture2->LoadTGA("./textures/whiteTiles.tga");

	Texture * textureC = new Texture();
    textureC->LoadTGA("./textures/coordtex.tga");

	Mesh* mesh = new Mesh();
    mesh->loadFromFile("./models/dragon.blend");
    mesh->init();

	Mesh* mesh2 = new Mesh();
	mesh2->loadFromFile("./models/Floor.blend");
	mesh2->init();

	Mesh* mesh3 = new Mesh();
	mesh3->loadFromFile("./models/cube.blend");
	mesh3->init();

	Mesh* mesh4 = new Mesh();
	mesh4->loadFromFile("./models/coord.blend");
	mesh4->init();

	m_rootNode->nodeAttach(mesh2);
	m_rootNode->nodeAttach(texture2);

	for(GLuint i = 0; i < 3; i++)
	{
		Node* node = new Node(("Dragon "+ i));
		
		node->nodeAttach(mesh);

		if(i%2)node->nodeMove(glm::vec3(1.0 * i,0.5,-1.0 * i));
		else node->nodeMove(glm::vec3(-1.0 * i,0.5,-1.0 * i));

		node->nodeAttach(texture);

		node->nodeRotate(30.0f * i,glm::vec3(0.0,1.0,0.0));
		m_rootNode->addChild(node);
	}

	//m_Camera.camLookAt(glm::vec3(0.0,3.0,9.0), glm::vec3(0.0,3.0,0.0), glm::vec3(0.0,1.0,0.0));
	//m_Camera.camLookAt(glm::vec3(4.0,9.0,3.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0));
	m_Camera.camLookAt(glm::vec3(1.0,3.0,1.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0));

	Texture * texture3 = new Texture(m_light_mrt->getTexture(4), m_light_mrt->getTextureUnit(4), m_light_mrt->getSampler(4));
	
	Node *cubeNode = new Node("Wuerfel");
	cubeNode->nodeAttach(mesh3);
	cubeNode->nodeAttach(texture3);
	cubeNode->nodeMove(glm::vec3(0.0f,2.0f,5.0f));
	m_rootNode->addChild(cubeNode);

	Node *coordNode = new Node("Coord");
	coordNode->nodeAttach(mesh4);
	coordNode->nodeAttach(textureC);
	m_rootNode->addChild(coordNode);
}

void LPVApp::showRenderTarget(int value)
{
	rtID = value;
}

void LPVApp::changeLightPosition(glm::vec3 translate)
{
	m_lightViewMatrix = glm::translate(m_lightViewMatrix, translate);

	glm::mat4 test = glm::inverse(glm::mat4(m_lightViewMatrix));
	m_rootNode->getChildByName("Coord")->transform_local = test;

	glm::mat4 bias = glm::mat4(1.0f);
	bias = glm::translate(bias, glm::vec3(0.5f,0.5f,0.5f));
	bias = glm::scale(bias, glm::vec3(0.5f,0.5f,0.5f));
	m_lightSpaceMatrix = bias * (m_lightProjectionMatrix * m_lightViewMatrix);
}