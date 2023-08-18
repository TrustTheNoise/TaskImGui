# CMakeLists.txt for library imgui (also add all needed cpp files like my cpp
# file App.cpp with )

add_library(
  imgui # LibName
  # Included sources
  imgui.cpp
  imgui_demo.cpp
  App.cpp
  imgui_draw.cpp
  imgui_impl_glfw.cpp
  imgui_impl_opengl3.cpp
  imgui_tables.cpp
  imgui_widgets.cpp)

# Add directory of headers
target_include_directories(imgui PUBLIC ../headers/)
