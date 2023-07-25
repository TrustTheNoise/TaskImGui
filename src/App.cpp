#define STB_IMAGE_IMPLEMENTATION
#include "App.h"
#include "../GLFW/glfw3.h"
#include "../imgui/imgui.h"
#include "stb_image.h"
#include <cstring>
#include <fstream>

#define INPUT_BUFF_SIZE 256
#define OUTPUT_BUFF_SIZE 4096

namespace MyApp {
static void FirstWin();
static void SecondWin();
char Input[INPUT_BUFF_SIZE] = "";
int radio_button_index = 0;
bool win1_button = true;
bool win2_button = false;
char *Output = new char[OUTPUT_BUFF_SIZE]{};
void RenderUI() {
  ImGui::Begin("Settigns", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  const ImVec4 grey_blue = ImVec4(0.4f, 0.6f, 0.8f, 1.0f);
  const ImVec2 button_size =
      ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 20);
  bool push_color = false;
  if (win1_button) {
    ImGui::PushStyleColor(ImGuiCol_Button, grey_blue);
    push_color = true;
  }

  if (ImGui::Button("First window", button_size) && !win1_button) {
    win1_button = true;
    win2_button = false;
  }

  if (win1_button && push_color)
    ImGui::PopStyleColor();

  ImGui::SameLine();

  if (win2_button) {
    push_color = true;
    ImGui::PushStyleColor(ImGuiCol_Button, grey_blue);
  }

  if (ImGui::Button("Second window", button_size) && !win2_button) {
    push_color = false;
    win1_button = false;
    win2_button = true;
  }

  if (win2_button && push_color)
    ImGui::PopStyleColor();

  if (win2_button)
    SecondWin();

  if (win1_button)
    FirstWin();
  ImGui::End();
}

static void FirstWin() {
  // Load the image data using stb_image.h
  int image_width = 0, image_height = 0;
  unsigned char *image_data =
      stbi_load("../Image/cat.jpg", &image_width, &image_height, NULL, 4);
  if (image_data == NULL)
    return;

  // Generate and bind an OpenGL texture
  GLuint image_texture;
  glGenTextures(1, &image_texture);
  glBindTexture(GL_TEXTURE_2D, image_texture);

  // Set texture filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Set the pixel store parameter for unpacking rows
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif

  // Load the image data into the texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image_data);
  // Free the memory used by the image data
  stbi_image_free(image_data);

  ImGui::BeginChild("ChildLeft",
                    ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 0),
                    true, ImGuiWindowFlags_None);
  ImGui::TextWrapped("%s", Output);
  ImGui::EndChild();

  ImGui::SameLine();

  ImGui::BeginChild("ChildRight",
                    ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 0),
                    true, ImGuiWindowFlags_None);
  ImGui::InputTextMultiline("##InputText", Input, IM_ARRAYSIZE(Input),
                            ImVec2(ImGui::GetWindowContentRegionWidth(), 100));

  ImGui::NewLine();
  ImGui::SameLine(0.0f, 230.0f);
  if (ImGui::Button("Send") || ImGui::IsKeyPressed(ImGuiKey_Enter) ||
      ImGui::IsKeyPressed(ImGuiKey_KeyPadEnter)) {
    strcat(Output, Input);
  }
  ImGui::NewLine();
  ImGui::SameLine(0.0f, 230.0f);

  if (ImGui::Button("Clear")) {
    memset(Output, 0, sizeof(Output));
  }

  ImGui::NewLine();
  ImGui::SameLine(0.0f, 230.0f);
  if (ImGui::Button("Save")) {
    std::ofstream file("output.txt");
    file << Output;
    file.close();
    memset(Output, 0, sizeof(Output));
  }

  ImGui::Image((void *)(intptr_t)image_texture,
               ImVec2(ImGui::GetWindowContentRegionWidth() * 0.98,
                      ImGui::GetWindowHeight() * 0.6));
  ImGui::EndChild();
}

static void SecondWin() {
  const char *options[] = {"Point 1", "Point 2", "Point 3"};
  const float data[] = {1.0f, 2.0f, 1.5f, 3.0f, 2.5f, 4.0f, 3.5f, 2.0f};
  ImGui::BeginChild("",
                    ImVec2(ImGui::GetWindowContentRegionWidth(),
                           ImGui::GetWindowHeight() * 0.30),
                    true, ImGuiWindowFlags_None);
  ImGui::GetWindowDrawList()->AddCircle(ImVec2(250, 100), 50.0f,
                                        IM_COL32(0, 255, 0, 255), 12, 2.0f);
  ImGui::GetWindowDrawList()->AddRect(ImVec2(350, 50), ImVec2(450, 150),
                                      IM_COL32(0, 0, 255, 255), 0.0f, 0, 2.0f);
  ImGui::EndChild();

  if (ImGui::RadioButton("Point 1", &radio_button_index, 0))
    radio_button_index = 0;
  ImGui::SameLine();
  if (ImGui::RadioButton("Point 2", &radio_button_index, 1))
    radio_button_index = 1;
  ImGui::SameLine();
  if (ImGui::RadioButton("Point 3", &radio_button_index, 2))
    radio_button_index = 2;
  ImGui::SameLine();

  ImGui::Combo("Select an option", &radio_button_index, options,
               IM_ARRAYSIZE(options));

  ImGui::PlotLines("Graph", data, sizeof(data) / sizeof(float));

  ImGui::BeginChild("TextToo",
                    ImVec2(ImGui::GetWindowContentRegionWidth(),
                           ImGui::GetWindowHeight() * 0.30),
                    true, ImGuiWindowFlags_None);
  ImGui::Text("Hey, im here too >///<");
  ImGui::TextWrapped("%s", Output);
  ImGui::EndChild();
}

} // namespace MyApp
