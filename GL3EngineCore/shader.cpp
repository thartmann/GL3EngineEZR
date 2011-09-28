#include "shader.h"
#include <cassert>

Shader::Shader(std::string vpFile, std::string fpFile)
{
	m_vp.assign(LoadShaderProgram(vpFile));
	m_gp.clear();
	m_fp.assign(LoadShaderProgram(fpFile));

	InitializeShader();
}

Shader::Shader(std::string vpFile, std::string gpFile, std::string fpFile)
{
	m_vp.assign(LoadShaderProgram(vpFile));
	m_gp.assign(LoadShaderProgram(gpFile));
	m_fp.assign(LoadShaderProgram(fpFile));

	InitializeShader();
}

Shader::~Shader()
{
	glDeleteShader(m_shaderFP);
	if (!m_gp.empty())
	{
		glDeleteShader(m_shaderGP);
	}
	glDeleteShader(m_shaderVP);

	glDeleteProgram(m_shaderID);
}

bool Shader::BindShader(void)
{
	glUseProgram(m_shaderID);

	return CheckOpenGLError("bind shader in shader");
}


bool Shader::UnBindShader(void)
{
	glUseProgram(0);

	//glDetachShader(m_shaderID, m_shaderVP);
	//if (!m_gp.empty())
	//{
	//	glDetachShader(m_shaderID, m_shaderGP);
	//}
	//glDetachShader(m_shaderID, m_shaderFP);

	return CheckOpenGLError("unbind shader in shader");
}


GLuint Shader::GetShaderID()
{
	return m_shaderID;
}


std::string Shader::LoadShaderProgram(std::string shaderFile)
{
	std::string line("");
	std::string program("");

    std::ifstream file(shaderFile.c_str());
    if (file.is_open())
    {
        while (file.good())
        {
            std::getline(file,line);
            program.append(line);
            program.append("\n");
        }
        file.close();
    }
    else
	{
		std::cerr << "[ERROR] Could not open shader program " << shaderFile << std::endl;

		program.assign("Failed to open " + shaderFile);
		return program;
	}

    return program;
}


bool Shader::InitializeShader(void)
{
	const GLchar *vp = m_vp.c_str();

	m_shaderVP = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_shaderVP, 1, &vp, 0);
	glCompileShader(m_shaderVP);

    if (!m_gp.empty())
    {
        const GLchar *gp = m_gp.c_str();

        m_shaderGP = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(m_shaderGP, 1, &gp, 0);
        glCompileShader(m_shaderGP);
    }

	const GLchar *fp = m_fp.c_str();

	m_shaderFP = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_shaderFP, 1, &fp, 0);
	glCompileShader(m_shaderFP);

	m_shaderID = glCreateProgram();
	glAttachShader(m_shaderID, m_shaderVP);
    if (!m_gp.empty())
    {
        glAttachShader(m_shaderID, m_shaderGP);
    }
	glAttachShader(m_shaderID, m_shaderFP);

	glLinkProgram(m_shaderID);
	printProgramInfoLog();

	return CheckOpenGLError("initialize shaders in shader");
}

void Shader::printProgramInfoLog() {
  int infologLen = 0;
  glGetProgramiv(m_shaderID, GL_INFO_LOG_LENGTH, &infologLen);
  if (infologLen > 1) {
    GLchar * infoLog = new GLchar[infologLen];
    if (infoLog == NULL) {
      std::cout << "[ERROR] Could not allocate InfoLog buffer";
    }
    int charsWritten = 0;
    glGetProgramInfoLog(m_shaderID, infologLen, &charsWritten, infoLog);
    std::string shaderlog = infoLog;
    free(infoLog);
    std::cout << "[DEBUG] Program Log "<< shaderlog << "\n";
  } else {
    std::cout << "[DEBUG] Program compiled" << "\n";
  }
}
