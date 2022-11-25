#include "Window.hpp"

#include <stdexcept>

struct Window::Implementation
{
    GLFWwindow * win;
};

void keyCallback(GLFWwindow* window, int key, int, int action, int mods)
{
    Event e;
    e.type = EventType::KEY;
    e.key.code = key;
    e.key.action = action;
    e.key.mods = mods;

    Window * windowObject = (Window*) glfwGetWindowUserPointer(window);
    windowObject->pushEvent(e);
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    Event e;
    e.type = EventType::MOUSE;
    e.mouse.button = button;
    e.mouse.action = action;
    e.mouse.mods = mods;
    glfwGetCursorPos(window, &e.mouse.pos[0], &e.mouse.pos[1]);

    Window * windowObject = (Window*) glfwGetWindowUserPointer(window);
    windowObject->pushEvent(e);
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    Event e;
    e.type = EventType::CURSOR;
    e.cursor.pos[0] = xpos;
    e.cursor.pos[1] = ypos;

    Window * windowObject = (Window*) glfwGetWindowUserPointer(window);
    windowObject->pushEvent(e);
}

void windowResizeCallback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}

Window::Window(const int& width, const int& height, const char * title)
    : m_pImpl(std::make_unique<Implementation>())
{
    if (!glfwInit()) throw std::runtime_error("Unable to initialize GLFW");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_pImpl->win = glfwCreateWindow(width, height, title, nullptr, nullptr);
    
    glfwSetWindowUserPointer(m_pImpl->win, this);
    //glfwSetWindowAspectRatio(win, width, height);
    glfwMakeContextCurrent(m_pImpl->win);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    glViewport(0, 0, width, height);
    
    glfwSetKeyCallback(m_pImpl->win, keyCallback);
    glfwSetCursorPosCallback(m_pImpl->win, cursorCallback);
    glfwSetMouseButtonCallback(m_pImpl->win, mouseCallback);
    glfwSetFramebufferSizeCallback(m_pImpl->win, windowResizeCallback);
}

GMath::Vec2i Window::size() const
{
    GMath::Vec2i windowSize;
    glfwGetWindowSize(m_pImpl->win, &windowSize[0], &windowSize[1]);
    return windowSize;
}

void Window::swapBuffers() const { glfwSwapBuffers(m_pImpl->win); }
bool Window::isOpen() const { return !glfwWindowShouldClose(m_pImpl->win); }
void Window::pollEvents() { glfwPollEvents(); }
void Window::pushEvent(const Event& e) { m_eventQueue.push(e); }

bool Window::hasEvent(Event& e)
{
    if (m_eventQueue.size() > 0)
    {
        e = m_eventQueue.back();
        m_eventQueue.pop();
        return true;
    }
    return false;
}

void Window::close() { glfwSetWindowShouldClose(m_pImpl->win, true); }
Window::~Window() { glfwDestroyWindow(m_pImpl->win); glfwTerminate(); }