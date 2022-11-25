#pragma once

#include <queue>
#include <memory>

#include "GraphicsMath.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum class EventType { KEY, CURSOR, MOUSE };

struct Key { int code, action, mods; };
struct Mouse { int button, action, mods; GMath::Vector<double, 2> pos; };
struct Cursor { GMath::Vector<double, 2> pos; };
struct Event
{
    EventType type;
    union 
    {
        Key key;
        Mouse mouse;
        Cursor cursor;
    };
};

class Window
{
    struct Implementation;
    std::unique_ptr<Implementation> m_pImpl;
    std::queue<Event> m_eventQueue;

public:
    bool hasEvent(Event& e);
    void pushEvent(const Event& e);
    Window(const Window&) = delete;
    Window(const int& width, const int& height, const char * title);
    ~Window();

    GMath::Vec2i size() const;
    void swapBuffers() const;
    bool isOpen() const;
    void pollEvents();
    void close();
};