#include <chrono>
#include <ios>
#include <iostream>
#include "color.h"
#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include "material.h"
#include "rwqueue/readerwriterqueue.h"
#include <SDL.h>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <complex>
#include <thread>
#include <mutex>
#include <atomic>
#include <optional>
#include <GL/glew.h>
#include "gfx_funcs.h"
#include "game.h"
#include "map.h"
#include "cam.h"

ImColor fg_clr = ImColor({250, 250, 250});

/*
void draw_polygon(ImDrawList* drawList, const polygon &pol,
    bool draw_labels=false)
{
    auto draw_lbl = [drawList](polygon::dot_t dot){
        std::stringstream ss;
        ss.precision(2);
        ss<< std::fixed << dot.x << " " << dot.y;
        auto str = ss.str();
        drawList->AddText({dot.x, dot.y}, fg_clr, str.c_str(), NULL);
    };
    std::optional<std::vector<game::dot>> norms;
    for(size_t n = 0; n<pol.size(); n++){
        auto beg = pol.at(n);
        auto end = pol.at((n+1)%pol.size());
        drawList->AddLine({beg.x, beg.y}, {end.x, end.y}, fg_clr);
        if(draw_labels)
            draw_lbl(beg);
    }
}
*/

int main(int, char**) {
    moodycamel::BlockingReaderWriterQueue<int> q;
    game::engine g(q);
    g.run();
    game::material air;
    air.set_color({230, 230, 230, 230});
    game::material ground;
    ground.set_color({100, 100, 100, 255});
    game::map_gen_settings sets{air, ground, 16, 16, 128};
    game::map m(sets);

    const unsigned w = 1920, h = 1080;
    size_t depth = 1;
    size_t tile_draw_w = 20, 
        tile_draw_h = 20;

    game::cam c;
    c.set_pos({0,0,0});
    c.set_fov({w/tile_draw_w, h/tile_draw_h, 1});
    int ui_beg_x = w/2 - c.fov().x * tile_draw_w/2;
    int ui_beg_y = h/2 - c.fov().y * tile_draw_h/2;


    if (SDL_Init(SDL_INIT_VIDEO |
        SDL_INIT_TIMER |
        SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    bool err = glewInit() != GLEW_OK;
    if (err) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Main loop
    bool done = false;
    GLuint id;
    GLubyte raw[w*h*4];
    std::fill(std::begin(raw), std::end(raw), 128);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &raw);
    glBindTexture(GL_TEXTURE_2D, 0);
    bool draw_labels = false;

    ImVec2 drag_1, drag_2, prev_drag_2;
    bool drag_beg;
    size_t cam_x = c.pos().x, cam_y = c.pos().y, cam_z = c.pos().z;
    size_t cam_fv_x = c.fov().x, cam_fv_y = c.fov().y, cam_fv_z = c.fov().z;

    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        auto drawList = ImGui::GetBackgroundDrawList();

        ImGui::Begin("Parameters");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Checkbox("Draw verts labels", &draw_labels);
        ImGui::InputInt("X", (int*)&cam_x);
        ImGui::InputInt("Y", (int*)&cam_y);
        ImGui::InputInt("Z", (int*)&cam_z);
        ImGui::InputInt("X fov", (int*)&cam_fv_x);
        ImGui::InputInt("Y fov", (int*)&cam_fv_y);
        ImGui::InputInt("Z fov", (int*)&cam_fv_z);
        c.set_pos({cam_x, cam_y, cam_z});
        c.set_fov({cam_fv_x, cam_fv_y, cam_fv_z});
        ImGui::End();
        if(ImGui::IsMouseClicked(0)){
            drag_1 = ImGui::GetMousePos();
        }
        if(std::abs(ImGui::GetMouseDragDelta(0).x) > .0f &&
            std::abs(ImGui::GetMouseDragDelta(0).y) > .0f)
        {
            drag_beg = true;
        }
        if(drag_beg){
            drag_2 = ImGui::GetMousePos();
        }
        if(ImGui::IsMouseReleased(0)){
            drag_2 = ImGui::GetMousePos();
            drag_beg = false;
        }
        auto size = ImGui::GetIO().DisplaySize;
        glBindTexture(GL_TEXTURE_2D, id);
        auto txtr_size = tile_draw_h*tile_draw_w*4;
        size_t x_cnt = 0;
        size_t y_cnt = 0;
        auto x_end = c.pos().x+c.fov().x;
        auto y_end = c.pos().y+c.fov().y;
        for(size_t x = c.pos().x; x < x_end; x++){
            y_cnt = 0;
            for(size_t y = c.pos().y; y < y_end; y++){
                auto chunk_ptr = m.get_chunk(x, y);
                if(!chunk_ptr){
                    chunk_ptr = m.gen_chunk(x, y);
                }
                auto& chunk = *chunk_ptr;
                for(int z = c.fov().z-1; z > -1; z--){
                    auto tl = chunk.get_tile(x, y, c.pos().z + z);
                    GLubyte txtr[txtr_size];
                    auto clr = tl.mat().color();
                    for(size_t px = 0; px<txtr_size; px+=4){
                        txtr[px+0] = clr.r;
                        txtr[px+1] = clr.g;
                        txtr[px+2] = clr.b;
                        txtr[px+3] = clr.a;
                    }
                    auto draw_x = ui_beg_x + x_cnt*tile_draw_w;
                    auto draw_y = ui_beg_y + y_cnt*tile_draw_h;
                    glTexSubImage2D(GL_TEXTURE_2D, 0, draw_x, draw_y,
                        tile_draw_w, tile_draw_h, GL_RGBA, GL_UNSIGNED_BYTE,
                        &txtr);
                }
                y_cnt++;
            }
            x_cnt++;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        /*
        if(q.try_dequeue(res)){
            do{
                glBindTexture(GL_TEXTURE_2D, id);
                glTexSubImage2D(GL_TEXTURE_2D, 0, i.x, i.y,
                        i.w, i.h, GL_RGB, GL_UNSIGNED_BYTE,
                        i.pixels.data());
                glBindTexture(GL_TEXTURE_2D, 0);
            }while(q.try_dequeue(res));
        }
        */
        
        drawList->AddImage((void*)(intptr_t)id, ImVec2(0,0), ImVec2(1920, 1080));
        if(drag_beg){
        }

        prev_drag_2 = drag_2;
        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    g.stop();

    return 0;
}
