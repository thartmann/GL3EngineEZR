#ifndef GLFW_TEST_H
#define GLFW_TEST_H

/*!
 *\brief    small testproject to develop an OpenGL3+ framework
 *
 *\author   Dominik Ospelt, Hans-Christian Wollert, Tobias Hartmann
 *\date     01-07-2011
 */


//#include "common.h" <-- already included in shader.h
#include "shader.h"



class glfw_test
{
    public:

        glfw_test(void);


        ~glfw_test(void);


    protected:

        /*!
         *\brief    checks for OpenGL context and capabilities
         *
         *\author   Lubosz Sarnecki
         */
        void CheckVersion(void);


       /*!
         *\brief    does all the window setup and initializing work
         *
         *\return   0 on success
         */
        int Initialize(void);


        ///from assimp/samples/SimpleTexturedOpenGL
        void CreateAILogger(void);


        ///from assimp/samples/SimpleTexturedOpenGL
        void DestroyAILogger(void);


        ///from assimp/samples/SimpleTexturedOpenGL
        void LogInfo(std::string logString);


        ///from assimp/samples/SimpleTexturedOpenGL
        void LogDebug(const char* logString);


        ///from assimp/samples/SimpleTexturedOpenGL
        bool Import3DFromFile( const std::string& pFile);

		/*!
		 *\brief	generates the scene to be loaded
		 */
		void SetupScene(void);

		/*!
		 *\brief	makes a VBO
		 */
		void MakeVBO(void);


		/*!
		 *\brief	destroys the VBO
		 */
		void DeleteVBO(void);


		/*!
		 *\brief	draws a rotating cube
		 */
		void DrawVBO(void);


		/*!
		 *\brief	does the rendering
		 */
		void RenderScene(void);


        /*!
         *\brief    starts the main loop
         *
         *\return   0 on success
         */
        int MainLoop(void);


    private:

        bool m_running;

        int m_windowWidth;
        int m_windowHeight;

       	float m_rotX;
		float m_rotY;


        GLuint m_vaoID;
        GLuint m_vboID[2];

        // the global Assimp scene object
        const aiScene *m_scene;

        // Create an instance of the Importer class
        Assimp::Importer m_importer;

        Shader *m_shader;

        GLuint m_projectionMatrixUniformLocation;
        GLuint m_viewMatrixUniformLocation;
        GLuint m_modelMatrixUniformLocation;

		glm::core::type::mat4 m_projectionMatrix; // Store the projection matrix
		glm::mat4 m_viewMatrix; // Store the view matrix
		glm::mat4 m_modelMatrix; // Store the model matrix
};

#endif // GLFW_TEST_H
