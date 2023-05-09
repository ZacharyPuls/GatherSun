//
// Created by zach on 2021-09-26.
//

#include "Window.h"
#include "Debug.h"
#include "event/EventManager.h"

#include <iostream>
#include <utility>

namespace gathersun::game {

    static bool glfwIsInitialized_ = false;

    Window::Window(std::string title, int width, int height, bool fullscreen)
            : title_(std::move(title)), width_(width), height_(height), fullscreen_(fullscreen) {
        glfwSetErrorCallback(glfwErrorCallback_);

        if (!gathersun::game::glfwIsInitialized_) {
            if (!glfwInit()) {
                // TODO: formalize debug/logging
                std::cerr << "Could not initialize GLFW." << std::endl;
                return;
            }
            gathersun::game::glfwIsInitialized_ = true;
        }

        // TODO: separate window creation from render context creation
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
#ifndef NDEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#elif
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, false);
#endif

        GLFWmonitor *monitor = fullscreen_ ? glfwGetPrimaryMonitor() : nullptr;
        handle_ = glfwCreateWindow(width_, height_, title_.c_str(), monitor, nullptr);

        glfwSetKeyCallback(handle_, Window::keyboardCallback_);
        glfwSetCursorPosCallback(handle_, Window::cursorCallback_);
        glfwSetMouseButtonCallback(handle_, Window::mouseButtonCallback_);
        glfwSetWindowSizeCallback(handle_, Window::resizeCallback_);
        glfwSetWindowCloseCallback(handle_, Window::closeCallback_);

        glfwMakeContextCurrent(handle_);
        glfwSwapInterval(1);

        if (!gladLoadGL(glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

#ifndef NDEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenglDebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

        glfwSetWindowUserPointer(handle_, this);
    }

    Window::~Window() {
        glfwDestroyWindow(handle_);
        // TODO: either break GLFW init/deinit out into its own class, or refcount Window and only glfwTerminate when all references have been released
        glfwTerminate();
    }

    glm::ivec2 Window::GetSize() const {
        int width, height = 0;
        glfwGetFramebufferSize(handle_, &width, &height);
        return {width, height};
    }

    void Window::Swap() const {
        glfwSwapBuffers(handle_);
        glfwPollEvents();
    }

    bool Window::IsKeyPressed(int key) const {
        return keysPressed_[key];
    }

    glm::dvec2 Window::GetMousePosition() const {
        return {mousePosition_[0], mousePosition_[1]};
    }

    bool Window::IsMouseButtonPressed(int button) const {
        return mouseButtonPressed_[button];
    }

    bool Window::IsActive() const {
        return !glfwWindowShouldClose(handle_);
    }

    void Window::glfwErrorCallback_(int error, const char *description) {
        // TODO: formalize debug/logging
        std::cerr << "GLFW error with ID " << error << " encountered: " << description << std::endl;
    }

    void Window::keyboardCallback_(GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto windowPointer = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        bool pressed = action == GLFW_PRESS;
        bool repeated =  action == GLFW_REPEAT;
        windowPointer->keysPressed_[key] = pressed || repeated;
        event::EventManager::Instance().TriggerEvent<event::KeyboardEvent>(
                event::KeyboardEvent{.Key = key, .Type = pressed ? event::KeyboardEvent::EventType::PRESS : repeated
                                                                                                            ? event::KeyboardEvent::EventType::REPEAT
                                                                                                            : event::KeyboardEvent::EventType::RELEASE});
    }

    void Window::cursorCallback_(GLFWwindow *window, double xpos, double ypos) {
        auto windowPointer = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        windowPointer->mousePosition_[0] = xpos;
        // Invert y-axis due to Windows origin being at bottom
        windowPointer->mousePosition_[1] = windowPointer->height_ - ypos;
        event::EventManager::Instance().TriggerEvent<event::MouseMotionEvent>({});
    }

    void Window::mouseButtonCallback_(GLFWwindow *window, int button, int action, int mods) {
        // TODO: push event to message pump instead of setting mouseButtonPressed_?
        auto windowPointer = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        windowPointer->mouseButtonPressed_[button] = action == GLFW_PRESS;
        event::EventManager::Instance().TriggerEvent<event::MouseButtonEvent>({});
    }

    void Window::resizeCallback_(GLFWwindow *window, int width, int height) {
        // TODO: send resize event to renderer to resize viewport
        auto windowPointer = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        windowPointer->width_ = width;
        windowPointer->height_ = height;
        event::EventManager::Instance().TriggerEvent<event::WindowResizeEvent>({});
    }

    void Window::closeCallback_(GLFWwindow *) {
        // TODO: Send event::WindowCloseEvent, which processes by destroying the Window and triggering an event::ExitEvent
        event::EventManager::Instance().TriggerEvent<event::ExitEvent>({});
    }
}
