#ifndef SHADER_H
#define SHADER_H

#include "common.h"

class Shader {

public:
    /*!
     *\brief	constructor
     *
     *\param	vpFile - filename of vertex program
     *\param	fpFile - filename of fragment program
     */
    Shader(std::string vpFile, std::string fpFile);


    /*!
     *\brief	destructor
     */
    ~Shader();


	/*!
	 *\brief	binds the shaders
	 *
	 *\return	true on success
	 */
    bool BindShader(void);


	/*!
	 *\brief	unbinds the shaders
	 *
	 *\return	true on success
	 */
    bool UnBindShader(void);


    GLuint GetShaderID();

protected:
	/*!
	 *\brief	loads a file containing the shader code
	 *
	 *\param	shaderFile - name of file to load
	 *
	 *\return	shader code on success or "Failed to open " + shaderFile on failure
	 */
	std::string LoadShaderProgram(std::string shaderFile);


	/*!
	 *\brief	initializes the shader
	 *
	 *\return	true on success
	 */
	bool InitializeShader(void);


private:
	//shader code
    std::string m_vp;
    std::string m_fp;

	//needed indizes
    GLuint m_shaderID;
    GLuint m_shaderVP;
    GLuint m_shaderFP;
};

#endif //SHADER_H
