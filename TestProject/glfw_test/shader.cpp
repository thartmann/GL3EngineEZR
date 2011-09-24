#include "shader.h"


Shader::Shader(std::string vpFile, std::string fpFile)
{
	m_vp.assign(LoadShaderProgram(vpFile));
	m_fp.assign(LoadShaderProgram(fpFile));

	InitializeShader();
}

Shader::~Shader()
{
	glDeleteShader(m_shaderFP);
	glDeleteShader(m_shaderVP);

	glDeleteProgram(m_shaderID);
}

bool Shader::BindShader(void)
{
	GLenum errorCheckValue = glGetError();

	glUseProgram(m_shaderID);

	errorCheckValue = glGetError();
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not bind shader!" << std::endl;
		return false;
	}

	return true;
}


bool Shader::UnBindShader(void)
{
	GLenum errorCheckValue = glGetError();

	glUseProgram(0);

	glDetachShader(m_shaderID, m_shaderVP);
	glDetachShader(m_shaderID, m_shaderFP);

	errorCheckValue = glGetError();
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not unbind shader!" << std::endl;
		return false;
	}

	return true;
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
	GLenum errorCheckValue = glGetError();

	const GLchar *vp = m_vp.c_str();
	const GLchar *fp = m_fp.c_str();

	m_shaderVP = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_shaderVP, 1, &vp, 0);
	glCompileShader(m_shaderVP);

	m_shaderFP = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_shaderFP, 1, &fp, 0);
	glCompileShader(m_shaderFP);

	m_shaderID = glCreateProgram();
	glAttachShader(m_shaderID, m_shaderVP);
	glAttachShader(m_shaderID, m_shaderFP);

	glLinkProgram(m_shaderID);

	errorCheckValue = glGetError();
	if (errorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "[ERROR] Could not create shaders!" << std::endl;
		return false;
	}

	return true;
}
