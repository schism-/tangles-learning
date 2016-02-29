//
//  main.cpp
//  tangles_learning
//
//  Created by Christian on 15/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#include <string>

#include "draw.h"
#include "arrangement.h"

void run() {
    glfwInit();
    
    auto window = ui_init_window({600,600}, "tangles");
    auto callbacks = ui_init_callbacks(window);
    
    auto context = new NVGContext();
    
    auto tangle = new Tangle();
    tangle->test();
    tangle->print_cgal_arrangement();

    auto vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    nvgCreateFont(vg, "default", "resources/open-sans/OpenSans-Regular.ttf");
    
    auto draw_frames = false, draw_as_points = false, draw_debug = false, draw_colors = false;
    
    auto scale_factor = [&] {
        auto wh = ui_window_size(window);
        return vec2r((real)1.0, (real)1.0);
    };
    
    auto offset = [&] {
        auto wh = ui_window_size(window);
        return vec2r(wh.x,wh.y)/2 - 0.5 * center(rscale(tangle->bounds(), scale_factor().x));
        return vec2r((real)0.0, (real)0.0);
    };
    
    auto window_to_tangle = [&](const vec2r& p) { return ((real)1.0 / scale_factor().x) * (p - offset()); };
    
    auto mouse_pos = [&]{ return window_to_tangle(to_vec2r(ui_mouse_pos(window))); };
    
    std::string mouse_mode = "mouse_select"; // modes: select, outline, stroke
    auto mouse_stroke = polyline2r();
    
    callbacks->text = [&](ui_window* window, int key) {
        switch (key) {
            case 'v': draw_colors = not draw_colors; break;
            case 'f': draw_frames = not draw_frames; break;
            case 'p': draw_as_points = not draw_as_points; break;
            case 'd': draw_debug = not draw_debug; break;
            case 'q': mouse_mode = "mouse_select"; break;
            case 'w': mouse_mode = "mouse_outline"; break;
            case 'e': mouse_mode = "mouse_stroke"; break;
        }
    };
    
    while(not ui_done(window)) {
        ui_set_title(window, "tangles | "+ mouse_mode +" | ");
        auto wh = ui_window_size(window);
        
        context->begin_frame(wh, offset(), scale_factor());
        for(auto shape : tangle->shapes)
            context->draw_shape(shape, draw_frames, draw_as_points, {0,0,0,1}, {0,0,0,0});
        context->draw_line(mouse_stroke, {0,0,1,1});
        context->end_frame(-offset(), scale_factor());
        
        if(ui_mouse_button(window, ui_mouse_left)) {
            if(mouse_mode == "mouse_select") {
//                selected = tangle->select_added(mouse_pos());
            } else if(mouse_mode == "mouse_outline" or mouse_mode == "mouse_stroke") {
                mouse_stroke += mouse_pos();
            } else {
            
            }
        } else {
            if(mouse_mode == "mouse_outline" or mouse_mode == "mouse_stroke") {
                if(not mouse_stroke.empty() or ((int)mouse_stroke.size() > 2)) {
                    mouse_stroke = cleanup_stroke(mouse_stroke, mouse_mode == "mouse_outline");
                    tangle->add_curve(mouse_stroke);
                    mouse_stroke.clear();
                }
            } else { }
        }
        
        ui_end_frame(window);
    }
    
    nvgDeleteGL2(vg);
    
    ui_clear_window(window);
}

int main(int argc, const char * argv[]) {
    run();
}
