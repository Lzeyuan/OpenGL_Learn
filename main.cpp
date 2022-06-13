#include "include/GlobalHead.hpp"
#include "include/GLFWwindowClass.hpp"
#include "include/ModelData.hpp"
#include "include/Shader.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void RenderingUpdate();
void BindVBOData(const GLuint VAOId, const GLuint VBOId, const std::vector<float>& vertices,
	const std::vector<int>& attributePointer);
glm::mat4 GetProjectionMat4(const float fovDegrees, const float& aspectRatio, float zNear, float zFar);

const GLuint WIDTH = 800, HEIGHT = 600;
glm::vec3 cameraPosition{0, 0, 3.0f};


std::shared_ptr<Shader> triangleShader;
GLuint triangleVAO[4];
GLuint triangleVBO;
GLuint triangleEBO;

glm::vec4 color[4] {
    {1.0f, 0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f, 1.0f},
    {1.0f, 0.0f, 1.0f, 1.0f},
};

int main()
{
    std::shared_ptr<GLFWwindowClass> window = std::make_shared<GLFWwindowClass>(WIDTH, HEIGHT);
    window->Init();
    window->SetFramebufferSizeEvent(framebuffer_size_callback);
    window->SetKeyboradEvent(key_callback);

	glGenVertexArrays(4, triangleVAO);
	glGenBuffers(1, &triangleVBO);
    glGenBuffers(1, &triangleEBO);

    for (int i = 0; i < 4; i++)
    {
	    BindVBOData(triangleVAO[i], triangleVBO, vertices, {3});
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }
    
    triangleShader = std::make_shared<Shader>("Shaders/Vertex/triangleVertex.vs",
                        "Shaders/Fragment/triangleFrag.fs");

    std::vector<render_event_ptr> render_event_ptrs {RenderingUpdate};
    window->SetRenderEvent(render_event_ptrs);

    window->Start();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void RenderingUpdate() 
{
    glClearColor(0.3f, 0.0f, 0.0f, 1.0f);

    // z depth buffer default 1
    glClearDepth(1.0f);                             
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    triangleShader->use();
    glm::mat4 projection = GetProjectionMat4(45.0f, 1, .01f, 50.0f);
    // glm::mat4 projection = glm::perspective(45.0f, 1.0f, .01f, 50.0f);
    glm::mat4 view = glm::lookAt(cameraPosition, glm::vec3{0, 0, 0}, glm::vec3{0, 1.0f, 0});


    // RGB Pink
    for (int i = 0; i < 4; i++)
    {
        glBindVertexArray(triangleVAO[i]);

        float offset = 0.1f * i;
        glm::mat4x4 model {1.0f};
        model = glm::translate(model, glm::vec3(-offset, -offset, -offset));
        triangleShader->setMat4("model", model);
        triangleShader->setMat4("view", view);
        triangleShader->setMat4("projection", projection);

        triangleShader->setVec4("color", color[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

void BindVBOData(const GLuint VAOId, const GLuint VBOId, const std::vector<float>& vertices,
	const std::vector<int>& attributePointer) 
{
	if (vertices.size() <= 0) return;

	int elementSize = sizeof(vertices[0]);
	glBindVertexArray(VAOId);
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * elementSize, 
		vertices.data(), GL_STATIC_DRAW);

	int stride = std::accumulate(attributePointer.begin(), attributePointer.end(), 0) * elementSize;
	int step = 0;
	for (int i = 0; i < attributePointer.size() && i < 16; i++) {
		glVertexAttribPointer(i, attributePointer[i], GL_FLOAT, GL_FALSE, stride, 
			(void *)(step * elementSize));
		glEnableVertexAttribArray(i);
		step += attributePointer[i];
	}
}


glm::mat4 GetProjectionMat4(const float fovDegrees, const float& aspectRatio, float zNear, float zFar)
{
	float fovHalfRadians = glm::radians(fovDegrees/2);

    // 列优先
	// 先进行透视投影，挤压透视形成的棱台，成矩形
	glm::mat4 projection{0.0f};	
	projection = {
		zNear, 	0.0f, 	0.0f, 		     0.0f,
		0.0f, 	zNear, 	0.0f, 		     0.0f,
		0.0f, 	0.0f,	zNear + zFar,   -1.0f,
		0.0f, 	0.0f, 	zNear*zFar,      0.0f
	};

	// Orthoprojection
	glm::mat4 scale{1.0f};
    scale[0][0] = 1 / (zNear * aspectRatio * tan(fovHalfRadians));
    scale[1][1] = 1 / (zNear * tan(fovHalfRadians));
    scale[2][2] = 2 / -(zNear - zFar);  // difference point

	glm::mat4 translation{1.0f};
	translation[3][2] = (zFar + zFar) * 0.5f;

	auto ret =  scale * translation * projection;
	return ret;
}