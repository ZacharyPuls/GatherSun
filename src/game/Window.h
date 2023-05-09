//
// Created by zach on 2021-09-26.
//

#ifndef GATHERSUN_GAME_WINDOW_H
#define GATHERSUN_GAME_WINDOW_H

#include <string>
#include "Core.h"

namespace gathersun::game {

    class Window {
    public:
        explicit Window(std::string title, int width, int height, bool fullscreen);

        ~Window();

        glm::ivec2 GetSize() const;

        void Swap() const;

        bool IsKeyPressed(int key) const;

        glm::dvec2 GetMousePosition() const;

        bool IsMouseButtonPressed(int button) const;

        bool IsActive() const;

    private:
        std::string title_;
        int width_ = 0;
        int height_ = 0;
        bool fullscreen_ = false;
        bool keysPressed_[GLFW_KEY_LAST] = {false};
        double mousePosition_[2] = {0.0};
        bool mouseButtonPressed_[GLFW_MOUSE_BUTTON_LAST] = {false};
        GLFWwindow *handle_ = nullptr;

        static void glfwErrorCallback_(int error, const char *description);

        static void keyboardCallback_(GLFWwindow *window, int key, int scancode, int action, int mods);

        static void cursorCallback_(GLFWwindow *window, double xpos, double ypos);

        static void mouseButtonCallback_(GLFWwindow *window, int button, int action, int mods);

        static void resizeCallback_(GLFWwindow *window, int width, int height);

        static void closeCallback_(GLFWwindow *);
    };
}

#endif // GATHERSUN_GAME_WINDOW_H
