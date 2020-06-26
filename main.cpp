#include <chrono>
#include <ios>
#include <iostream>
#include <queue>
#include <future>
#include "chunk.h"
#include "color.h"
#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include "material.h"
#include "rwqueue/readerwriterqueue.h"
#include <SDL.h>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
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
ImColor bg_clr;

int main(int, char**) {
    game::engine g;
    const size_t ch_w = 32, ch_h = 32; //in tiles
    float air_clr_fl[4] = { 200./255., 245./255., 255./255, 40./255. };
    float gnd_clr_fl[4] = {   0./255.,  30./255.,   7./255, 255./255. };
    game::material air, gnd;
    {
        game::color air_clr;
        air_clr.r = air_clr_fl[0]*255;
        air_clr.g = air_clr_fl[1]*255;
        air_clr.b = air_clr_fl[2]*255;
        air.set_color(air_clr);

        game::color gnd_clr;
        gnd_clr.r = gnd_clr_fl[0]*255;
        gnd_clr.g = gnd_clr_fl[1]*255;
        gnd_clr.b = gnd_clr_fl[2]*255;
        gnd.set_color(gnd_clr);

        bg_clr = ImColor({air_clr.r, air_clr.g, air_clr.b});
        game::map_gen_settings sets{air, gnd, ch_w, ch_h, 128};
        game::map m(sets);
        g.set_map(m);
    }
    g.run();

    auto air_clr = air.color();
    auto gnd_clr = gnd.color();

    const unsigned w = 1920, h = 1080;
    const size_t tile_draw_w = 32, 
        tile_draw_h = 32;

    game::cam c;
    c.set_pos({0,0,0});//in tiles
    c.set_fov({w/tile_draw_w, h/tile_draw_h, 1}); //in tiles
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
    /*
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &raw);
    glBindTexture(GL_TEXTURE_2D, 0);
    */
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
        ImGui::ColorEdit4("Air color", air_clr_fl);
        ImGui::ColorEdit3("Ground color", gnd_clr_fl);
        ImGui::End();

        c.set_pos({cam_x, cam_y, cam_z});
        c.set_fov({cam_fv_x, cam_fv_y, cam_fv_z});
        air_clr->r = air_clr_fl[0]*255;
        air_clr->g = air_clr_fl[1]*255;
        air_clr->b = air_clr_fl[2]*255;
        air_clr->a = air_clr_fl[3]*255;
        gnd_clr->r = gnd_clr_fl[0]*255;
        gnd_clr->g = gnd_clr_fl[1]*255;
        gnd_clr->b = gnd_clr_fl[2]*255;

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
        //glBindTexture(GL_TEXTURE_2D, id);
        //auto txtr_size = tile_draw_h*tile_draw_w*4;
        std::queue<std::future<game::map::ptr_t<game::chunk>>> ftrs;

        {
            const auto x_end = (c.pos().x+c.fov().x+ch_w-1); //in tiles
            const auto y_end = (c.pos().y+c.fov().y+ch_h-1); //in tiles
            for(size_t x = c.pos().x; x < x_end; x+=ch_w){
                for(size_t y = c.pos().y; y < y_end; y+=ch_h){
                    auto chunk_ftr = g.get_chunk(x, y);
                    ftrs.emplace(std::move(chunk_ftr));
                }
            }
            std::cout << "futures placed\n";
        }
        while(!ftrs.empty()){
            const auto chunk_ptr = ftrs.front().get();
            const auto& chunk = *chunk_ptr;
            const auto x_ = chunk.x();
            const auto y_ = chunk.y();
            std::cout << "ch:" << x_ << "tl " << y_ << "tl\n";
            const ImVec2 draw_size{tile_draw_w*1.f, tile_draw_h*1.f};
            const auto draw_x_start = std::max(x_, c.pos().x);
            const auto draw_x_end = std::min(x_ + chunk.w(), c.pos().x + c.fov().x);
            const auto draw_y_start = std::max(y_, c.pos().y);
            const auto draw_y_end = std::min(y_ + chunk.h(), c.pos().y + c.fov().y);
            for(size_t x = draw_x_start; x < draw_x_end; x++){
                const auto draw_x = ui_beg_x + (x-c.pos().x)*tile_draw_w;
                for(size_t y = draw_y_start; y < draw_y_end; y++){
                    const auto draw_y = ui_beg_y + (y-c.pos().y)*tile_draw_h;
                    const ImVec2 draw_coords1{draw_x*1.f, draw_y*1.f};
                    const ImVec2 draw_coords2{draw_coords1.x + draw_size.x,
                        draw_coords1.y + draw_size.y};
                    for(int z = c.fov().z-1; z > -1; z--){
                        const auto& tl = chunk.get_tile(x%chunk.w(), y%chunk.h(), c.pos().z + z);
                        const auto clr = tl.mat().color();
                        const auto clr_imtui = ImColor(clr->r, clr->g, clr->b, clr->a);
                        drawList->AddRectFilled(draw_coords1, draw_coords2, clr_imtui);
                    }
                }
            }
            ftrs.pop();
        }

        //drawList->AddImage((void*)(intptr_t)id, ImVec2(0,0), ImVec2(1920, 1080));

        prev_drag_2 = drag_2;
        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(1, 1, 1, 1);
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
