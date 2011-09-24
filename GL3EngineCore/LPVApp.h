#ifndef LPVAPP_H
#define LPVAPP_H

/*!
 *\brief    framework for LPVs
 *
 *\author   Dominik Ospelt, Hans-Christian Wollert, Tobias Hartmann
 *\date     01-07-2011
 */


#include "shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
#include "Node.h"
#include "MultipleRenderTarget.h"

class LPVApp
{
    public:

        LPVApp(void);


        ~LPVApp(void);


       /*!
         *\brief    does all the window setup and initializing work
         *
         *\return   0 on success
         */
        int Initialize(void);


		/*!
		 *\brief	does the rendering
		 */
		void RenderScene(void);


		/*!
		 *\brief 	sets screen size parameters
		 *
		 *\param	width - screen width
		 *\param	height - screen height
		 */
		void SetScreenSize(int width, int height);


		void showRenderTarget(int value);

		
		//public for testing
		Camera m_Camera;
		Node* m_rootNode;
		Node* m_testNode;


    protected:

        /*!
         *\brief    checks for OpenGL context and capabilities
         *
         *\author   Lubosz Sarnecki
         */
        void CheckVersion(void);


		/*!
		 *\brief	generates the scene to be loaded
		 */
		void SetupScene(void);

    private:
		int m_windowWidth;
		int m_windowHeight;

		Shader *m_shader;
		Shader *m_outputShader;

		MultipleRenderTarget *m_mrt;

        GLuint m_projectionMatrixUniformLocation;
        GLuint m_viewMatrixUniformLocation;
        GLuint m_modelMatrixUniformLocation;

		glm::core::type::mat4 m_projectionMatrix; // Store the projection matrix
		glm::core::type::mat4 m_viewMatrix; // Store the view matrix
		glm::core::type::mat4 m_modelMatrix; // Store the model matrix

		int rtID;

		GLint m_nodeTexLoc;

		GLint m_colorMapLocation;
		GLint m_positionMapLocation;
		GLint m_normalMapLocation;
		GLint m_fluxMapLocation;
		GLint m_depthMapLocation;

		GLint m_outputSwitch;
};

#endif // LPVAPP_H
