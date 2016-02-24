//
//  draw.hpp
//  tangles_learning
//
//  Created by Christian on 15/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#ifndef draw_h
#define draw_h

#include "ui.h"
#include "polygon.h"
#include "arrangement.h"

#define NANOVG_GL2
#include "external/nanovg/nanovg.h"
#include "external/nanovg/nanovg_gl.h"

struct DrawContext {
    NVGcontext* draw_init();
    
    virtual void begin_frame(const vec2i& wh, const vec2r& offset, const vec2r& scale) = 0;
    virtual void end_frame(const vec2r& offset, const vec2r& scale) = 0;
    
    virtual void draw_shape(Shape* shape, bool draw_frames, bool draw_as_points, const vec4f& stroke, const vec4f& fill);
    virtual void draw_labels(const vector<string>& labels);
    virtual void draw_polygon(const polygon2r& poly, const vec4f& stroke, const vec4f& fill) = 0;
    virtual void draw_line(const polyline2r& curve, const vec4f& stroke) = 0;
    virtual void draw_point(const vec2r& p, const vec4f& stroke) = 0;
    virtual void draw_text(const vec2r& p, const string& msg, real size, const vec4f& stroke) = 0;
};

struct NVGContext : DrawContext {
    NVGcontext* vg = nullptr;
    
    NVGContext();
    
    virtual void begin_frame(const vec2i& wh, const vec2r& offset, const vec2r& scale) override;
    virtual void end_frame(const vec2r& offset, const vec2r& scale) override;
    
    virtual void draw_polygon(const polygon2r& poly, const vec4f& stroke, const vec4f& fill) override;
    virtual void draw_line(const polyline2r& curve, const vec4f& stroke) override;
    virtual void draw_point(const vec2r& p, const vec4f& stroke) override;
    virtual void draw_text(const vec2r& p, const string& msg, real size, const vec4f& stroke) override;
};

struct SVGContext : DrawContext {
    string  svg = "";
    
    SVGContext();
    
    virtual void begin_frame(const vec2i& wh, const vec2r& offset, const vec2r& scale = {}) override;
    virtual void end_frame(const vec2r& offset, const vec2r& scale = {}) override;
    
    virtual void draw_polygon(const polygon2r& poly, const vec4f& stroke, const vec4f& fill) override;
    virtual void draw_line(const polyline2r& curve, const vec4f& stroke) override;
    virtual void draw_point(const vec2r& p, const vec4f& stroke) override;
    virtual void draw_text(const vec2r& p, const string& msg, real size, const vec4f& stroke) override;
    
    string _svg_style(const vec4f& stroke, const vec4f& fill);
    string _svg_points(const polyline2r& curve);
};

#endif /* draw_hpp */
