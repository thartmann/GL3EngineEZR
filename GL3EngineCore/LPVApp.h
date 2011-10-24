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
#include "LightSource.h"

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
		void changeLightPosition(glm::vec3 translate);

		
		//public for testing
		Camera m_Camera;
			
		LightSource m_ls;

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

		Node* m_rootNode;

		Shader *m_shader;
		Shader *m_LightInjectionShader;
		Shader *m_outputShader;

		MultipleRenderTarget *m_mrt;
		MultipleRenderTarget *m_light_mrt;

        GLuint m_projectionMatrixUniformLocation;
        GLuint m_viewMatrixUniformLocation;
        GLuint m_modelMatrixUniformLocation;

		GLuint m_lightProjectionMatrixUniformLocation;
        GLuint m_lightViewMatrixUniformLocation;
        GLuint m_modelMatrixUniformLocation2;
		GLuint m_lightSpaceMatrixUniformLocation;

		glm::core::type::mat4 m_projectionMatrix; // Store the projection matrix
		glm::core::type::mat4 m_viewMatrix; // Store the view matrix
		glm::core::type::mat4 m_modelMatrix; // Store the model matrix
		glm::core::type::mat4 m_lightViewMatrix;
		glm::core::type::mat4 m_lightProjectionMatrix;
		glm::core::type::mat4 m_lightSpaceMatrix;
		
		GLuint m_normalMatrixLoc;
		GLuint m_lightNormalMatrixLoc;

		int rtID;

		GLint m_nodeTexLoc;
		GLint m_nodeTexLoc2;

		GLint m_colorMapLocation;
		GLint m_positionMapLocation;
		GLint m_normalMapLocation;
		GLint m_fluxMapLocation;
		GLint m_depthMapLocation;
		GLint m_shadowMapLocation;

		GLint m_outputSwitch;
		GLint m_viewportDimensionLoc;

		GLint m_randomMapLoc;
		GLuint m_randomMapUnit;
		GLuint m_randomMapID;
		GLuint m_randomMapSampler;

		GLfloat m_sampleKernel[48];
		GLuint m_sampleKernelSize;
		GLuint m_sampleKernelLoc;

		GLuint m_projMatForSSAOLoc;
		GLuint m_viewMatForSSAOLoc;

		GLuint m_farPlaneLoc;
		
		float *m_lightArray;
};

#endif // LPVAPP_H
