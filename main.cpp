#include <iostream>
#include <ios>
#include <memory>
#include <queue>
#include <future>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <SDL.h>
#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include "nfd.hpp"
#include "nlohmann-json/single_include/nlohmann/json.hpp"
#include "rwqueue/readerwriterqueue.h"
#include "gfx_funcs.h"
#include "game.h"
#include "map.h"
#include "cam.h"
#include "item.h"
#include "chunk.h"
#include "color.h"
#include "material.h"

ImColor fg_clr = ImColor({250, 250, 250});
ImColor bg_clr;

void ui_save(const game::engine &g){
    std::string path;
    NFD_Init();
    nfdchar_t *outPath;
    nfdfilteritem_t filterItem[1] = { "roguelike saves", "json" };
    nfdresult_t result = NFD_SaveDialog(&outPath, filterItem, 1, NULL, "save");
    if ( result == NFD_OKAY ) {
        path = std::string(outPath);
        g.save(path);
        NFD_FreePath(outPath);
    } else if ( result == NFD_CANCEL ) {
    } else {
        printf("Error: %s\n", NFD_GetError() );
    }
}

void ui_load(game::engine &g){
    std::string path;
    NFD_Init();
    nfdchar_t *outPath;
    nfdfilteritem_t filterItem[1] = { "roguelike saves", "json" };
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
    if ( result == NFD_OKAY ) {
        path = std::string(outPath);
        g.load(path);
        NFD_FreePath(outPath);
    } else if ( result == NFD_CANCEL ) {
    } else {
        printf("Error: %s\n", NFD_GetError() );
    }
}

int main(int, char**) {
    game::engine g;

    std::shared_ptr<game::color> air_clr, gnd_clr;
    {
        auto mats = g.get_map().settings().materials;
        auto air_it = std::find_if(mats.begin(), mats.end(),
               [](auto mat){ return mat.name() == "air"; });
        auto gnd_it = std::find_if(mats.begin(), mats.end(),
               [](auto mat){ return mat.name() == "dirt"; });
        auto air = *air_it;
        auto gnd = *gnd_it;
        auto air_clr_tmp = air.color();
        auto gnd_clr_tmp = gnd.color();
        air_clr = std::const_pointer_cast<game::color>(air_clr_tmp);
        gnd_clr = std::const_pointer_cast<game::color>(gnd_clr_tmp);
    }
    float air_clr_fl[4] = { air_clr->r*1.f/255.f,
        air_clr->g*1.f/255.f,
        air_clr->b*1.f/255.f,
        air_clr->a*1.f/255.f };
    float gnd_clr_fl[4] = { gnd_clr->r*1.f/255.f,
        gnd_clr->g*1.f/255.f,
        gnd_clr->b*1.f/255.f,
        gnd_clr->a*1.f/255.f };
    auto ch_w = g.get_map().settings().chunk_w;
    auto ch_h = g.get_map().settings().chunk_h;

    unsigned short w = 1920, h = 1080;
    unsigned short tile_draw_w = 32;

    int cam_fov_z = 7;
    game::cam c;
    c.set_pos({0,0,0});//in tiles
    c.set_fov({w/tile_draw_w, h/tile_draw_w, cam_fov_z}); //in tiles
    int ui_beg_x = w/2 - c.fov().x * tile_draw_w/2;
    int ui_beg_y = h/2 - c.fov().y * tile_draw_w/2;


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
    bool show_create_item_win = false;
    GLuint id;
    //GLubyte raw[w*h*4];
    //std::fill(std::begin(raw), std::end(raw), 128);
    /*
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &raw);
    glBindTexture(GL_TEXTURE_2D, 0);
    */

    ImVec2 drag_1, drag_2, prev_drag_2;
    bool drag_beg;

    bool pressed_left  = false;
    bool pressed_right = false;
    bool pressed_up    = false;
    bool pressed_down  = false;
    bool pressed_shift = false;
    bool pressed_plus = false;
    bool pressed_minus = false;
    auto key_left  = SDLK_h;
    auto key_right = SDLK_l;
    auto key_up    = SDLK_k;
    auto key_down  = SDLK_j;
    auto key_up_z  = 'K';
    auto key_down_z= 'J';
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT){
                done = true;
            }
            if (event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(window))
            {
                done = true;
            }
            if (event.type == SDL_KEYDOWN) {
                auto sym = event.key.keysym.sym;
                pressed_plus =  (sym == SDLK_PLUS)?true:pressed_plus;
                pressed_minus=  (sym == SDLK_MINUS)?true:pressed_minus;
                pressed_left =  (sym == key_left )?true:pressed_left;
                pressed_right = (sym == key_right)?true:pressed_right;
                pressed_up =    (sym == key_up   )?true:pressed_up;
                pressed_down =  (sym == key_down )?true:pressed_down;
                pressed_shift = (event.key.keysym.mod == KMOD_LSHIFT ||
                    event.key.keysym.mod == KMOD_RSHIFT ||
                    sym == SDLK_LSHIFT ||
                    sym == SDLK_RSHIFT)
                    ?true:pressed_shift;
            } else if(event.type == SDL_KEYUP){
                auto sym = event.key.keysym.sym;
                pressed_plus =  (sym == SDLK_PLUS)?false:pressed_plus;
                pressed_minus=  (sym == SDLK_MINUS)?false:pressed_minus;
                pressed_left =  (sym == key_left )?false:pressed_left;
                pressed_right = (sym == key_right)?false:pressed_right;
                pressed_up =    (sym == key_up   )?false:pressed_up;
                pressed_down =  (sym == key_down )?false:pressed_down;
                pressed_shift = (sym == SDLK_LSHIFT ||
                    sym == SDLK_RSHIFT)?false:pressed_shift;
            }
        }
        static bool update_asked = true;

        if(pressed_left) {
            c.pos().x -= 1;
            update_asked = true;
        }
        if(pressed_right) {
            c.pos().x += 1;
            update_asked = true;
        }
        if(pressed_up) {
            if(!pressed_shift){
                c.pos().y -= 1;
            }else{
                c.pos().z -= 1;
            }
            update_asked = true;
        }
        if(pressed_down) {
            if(!pressed_shift){
                c.pos().y += 1;
            }else{
                c.pos().z += 1;
            }
            update_asked = true;
        }
        if(pressed_plus){
            c.zoom() += 0.05;
            update_asked = true;
        }else if(pressed_minus){
            c.zoom() -= 0.05;
            update_asked = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        auto drawList = ImGui::GetBackgroundDrawList();

        ImGui::Begin("Menu");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        if (ImGui::CollapsingHeader("Load/Save")){
            std::string path;
                /*
                igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", ".");
                if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey")) {
                    if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {
                        path = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
                        name = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
                    }
                    igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
                }
                */
            bool load_pressed = ImGui::Button("Load"); ImGui::SameLine();
            bool save_pressed = ImGui::Button("Save");
            if(load_pressed){
                ui_load(g);
            }else if(save_pressed){
                ui_save(g);
            }
        }
        if (ImGui::Button("Create item")){
            ImGui::OpenPopup("Create item");
        }
        {
            ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f,
                    ImGui::GetIO().DisplaySize.y * 0.5f);
            ImGui::SetNextWindowPos(center,
                    ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal("Create item", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                static char name[32];
                static int coords[3];
                ImGui::InputText("Name", &name[0], 32);
                ImGui::InputInt3("Coords", &coords[0]);
                if (ImGui::Button("OK", ImVec2(120, 0))){
                    std::string str_name(&name[0]);
                    game::item i(g.get_map().settings().materials.at(1), str_name);
                    i.set_x(coords[0]);
                    i.set_y(coords[1]);
                    i.set_z(coords[2]);
                    g.add_item(i);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
        }
        ImGui::LabelText("Cam X", "%lu", c.pos().x);
        ImGui::LabelText("Cam Y", "%lu", c.pos().y);
        ImGui::LabelText("Cam Z", "%lu", c.pos().z);
        ImGui::ColorEdit4("Air color", air_clr_fl);
        ImGui::ColorEdit3("Ground color", gnd_clr_fl);
        ImGui::End();

        air_clr->r = air_clr_fl[0]*255;
        air_clr->g = air_clr_fl[1]*255;
        air_clr->b = air_clr_fl[2]*255;
        air_clr->a = air_clr_fl[3]*255;
        gnd_clr->r = gnd_clr_fl[0]*255;
        gnd_clr->g = gnd_clr_fl[1]*255;
        gnd_clr->b = gnd_clr_fl[2]*255;

        auto size = ImGui::GetIO().DisplaySize;
        static std::vector<game::map::ptr_t<game::chunk>> chunks;

        if(update_asked){
            chunks.clear();
            static std::queue<std::future<game::map::ptr_t<game::chunk>>> ftrs;
            {
                const auto x_end = (c.pos().x+c.fov().x+ch_w-1); //in tiles
                const auto y_end = (c.pos().y+c.fov().y+ch_h-1); //in tiles
                for(size_t x = c.pos().x; x < x_end; x+=ch_w){
                    for(size_t y = c.pos().y; y < y_end; y+=ch_h){
                        auto chunk_ftr = g.get_chunk(x, y);
                        ftrs.emplace(std::move(chunk_ftr));
                    }
                }
            }
            while(!ftrs.empty()){
                chunks.emplace_back(std::move(ftrs.front().get()));
                ftrs.pop();
            }
            update_asked = false;
        }
        for(const auto chunk_ptr:chunks){
            const auto& chunk = *chunk_ptr;
            const auto x_ = chunk.x();
            const auto y_ = chunk.y();
            const ImVec2 draw_size{tile_draw_w*1.f, tile_draw_w*1.f};
            const auto draw_x_start = std::max(x_, c.pos().x);
            const auto draw_x_end = std::min(x_ + chunk.w(), c.pos().x + c.fov().x);
            const auto draw_y_start = std::max(y_, c.pos().y);
            const auto draw_y_end = std::min(y_ + chunk.h(), c.pos().y + c.fov().y);
            for(size_t x = draw_x_start; x < draw_x_end; x++){
                const auto draw_x = ui_beg_x + (x-c.pos().x)*tile_draw_w;
                for(size_t y = draw_y_start; y < draw_y_end; y++){
                    const auto draw_y = ui_beg_y + (y-c.pos().y)*tile_draw_w;
                    const ImVec2 draw_coords1{draw_x*1.f, draw_y*1.f};
                    const ImVec2 draw_coords2{draw_coords1.x + draw_size.x,
                        draw_coords1.y + draw_size.y};
                    for(int z = c.fov().z-1; z > -1; z--){
                        const auto& tl = chunk.get_tile(x%chunk.w(), y%chunk.h(), c.pos().z + z);
                        auto clr = tl.mat().color();
                        if(tl.size() != 0){
                            clr = tl.at(0)->mat().color();
                        }
                        const auto clr_imtui = ImColor(clr->r, clr->g, clr->b, clr->a);
                        drawList->AddRectFilled(draw_coords1, draw_coords2, clr_imtui);
                    }
                }
            }
        }

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

    return 0;
}
