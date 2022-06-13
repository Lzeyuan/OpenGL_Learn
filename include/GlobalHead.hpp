#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <numeric>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using render_event_ptr = void (*)();