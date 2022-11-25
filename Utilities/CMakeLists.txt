add_library(
    Utilities

    Button.hpp
    Font.cpp
    Font.hpp
    FontAtlas.cpp
    FontAtlas.hpp
    Glyph.hpp
    GraphicsMath.hpp
    Program.cpp
    Program.hpp
    Rect.hpp
    Resource.hpp
    String.cpp
    String.hpp
    Window.cpp
    Window.hpp
)

target_link_libraries(Utilities PUBLIC glfw opengl32)
target_include_directories(Utilities PUBLIC glfw opengl32)