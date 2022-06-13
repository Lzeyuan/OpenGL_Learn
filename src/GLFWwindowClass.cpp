#include "GLFWwindowClass.hpp"

GLFWwindowClass::GLFWwindowClass(const GLuint width, const GLuint height)
    : mWidth(width), mHeight(height)
{
    mRenderEventPtrVector = std::move(std::vector<render_event_ptr>{[]()->void
    {
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }});
}

int GLFWwindowClass::Init()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW 
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    mWindow = glfwCreateWindow(mWidth, mHeight, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(mWindow);
    if (mWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_GREATER);

    // default [0,1] znear zfar
    // glDepthRange(1,0);
    glViewport(0, 0, mWidth, mHeight);
    return 0;
}

void GLFWwindowClass::Start()
{
    // Game loop
    while (!glfwWindowShouldClose(mWindow))
    {
        glfwPollEvents();
        for (auto i : mRenderEventPtrVector)
        {
            i();
        }

        glfwSwapBuffers(mWindow);
    }
    glfwTerminate();
}

void GLFWwindowClass::SetFramebufferSizeEvent(GLFWframebuffersizefun frameBufferSizeEvent)
{
    glfwSetFramebufferSizeCallback(mWindow, frameBufferSizeEvent);
}

void GLFWwindowClass::SetKeyboradEvent(GLFWkeyfun keyboardEvent)
{
    glfwSetKeyCallback(mWindow, keyboardEvent);
}

void GLFWwindowClass::SetRenderEvent(std::vector<render_event_ptr> renderEventPtrVector)
{
    mRenderEventPtrVector.swap(renderEventPtrVector);
}