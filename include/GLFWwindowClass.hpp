#pragma once
#include "GlobalHead.hpp"

class GLFWwindowClass
{
public:
    GLFWwindowClass(const GLuint width = 800, const GLuint height = 600);
    int Init();
    void Start();
    void SetFramebufferSizeEvent(GLFWframebuffersizefun frameBufferSizeEvent);
    void SetKeyboradEvent(GLFWkeyfun keyboardEvent);
    void SetRenderEvent(std::vector<render_event_ptr> renderEventPtrVector);
private:
    GLFWwindow* mWindow;
    std::vector<render_event_ptr> mRenderEventPtrVector;
    int mWidth;
    int mHeight;
};