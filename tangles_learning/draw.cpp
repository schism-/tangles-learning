//
//  draw.cpp
//  tangles_learning
//
//  Created by Christian on 15/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#include "draw.h"

#include "external/format.h"

void DrawContext::draw_labels(const vector<string>& labels) {
    auto offset = vec2r{8,18};
    for(auto i : range(labels)) {
        draw_text({offset.x,offset.y*(i+1)}, labels[i], 18, {0,0,0,1});
    }
}

void NVGContext::draw_polygon(const polygon2r& poly, const vec4f& stroke, const vec4f& fill) {
    nvgBeginPath(vg);
    auto hole = false;
    for(auto&& curve : poly) {
        auto first = true;
        for(auto&& p : curve) {
            if(first) {
                nvgMoveTo(vg, p.x, p.y);
                first = false;
            } else nvgLineTo(vg, p.x, p.y);
        }
        nvgClosePath(vg);
        if(hole) nvgPathWinding(vg, NVG_HOLE);
        hole = true;
    }
    if(fill.w > 0) {
        nvgFillColor(vg, nvgRGBA(255*fill.x, 255*fill.y, 255*fill.z, 255*fill.w));
        nvgFill(vg);
    }
    if(stroke.w > 0) {
        nvgStrokeColor(vg, nvgRGBA(255*stroke.x, 255*stroke.y, 255*stroke.z, 255*stroke.w));
        nvgStroke(vg);
    }
}

void NVGContext::draw_line(const polyline2r& curve, const vec4f& stroke) {
    nvgBeginPath(vg);
    auto first = true;
    for(auto&& p : curve) {
        if(first) {
            nvgMoveTo(vg, p.x, p.y);
            first = false;
        } else nvgLineTo(vg, p.x, p.y);
    }
    nvgStrokeColor(vg, nvgRGBA(255*stroke.x, 255*stroke.y, 255*stroke.z, 255*stroke.w));
    nvgStroke(vg);
}

void NVGContext::draw_point(const vec2r& p, const vec4f& stroke) {
    nvgBeginPath(vg);
    nvgMoveTo(vg, p.x, p.y);
    nvgLineTo(vg, p.x + 1, p.y + 1);
    nvgStrokeColor(vg, nvgRGBA(255*stroke.x, 255*stroke.y, 255*stroke.z, 255*stroke.w));
    nvgStroke(vg);
}

void NVGContext::draw_text(const vec2r& p, const string& text, real size, const vec4f& stroke) {
    nvgFontSize(vg, size);
    nvgFontFace(vg, "default");
    nvgFillColor(vg, nvgRGBA(stroke.x*255, stroke.y*255, stroke.z*255, stroke.w*255));
    nvgBeginPath(vg);
    nvgText(vg, p.x, p.y, text.c_str(), NULL);
    nvgRestore(vg);
}

NVGContext::NVGContext() {
    vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    nvgCreateFont(vg, "default", "resources/open-sans/OpenSans-Regular.ttf");
}

void NVGContext::begin_frame(const vec2i& wh, const vec2r& offset, const vec2r& scale) {
    glClearColor(255, 255, 255, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    nvgBeginFrame(vg, wh.x, wh.y, 1);
    nvgTranslate(vg, offset.x, offset.y);
    nvgScale(vg, scale.x, scale.y);
}

void NVGContext::end_frame(const vec2r& offset, const vec2r& scale) {
    nvgScale(vg, (real) 1.0 / scale.x,  (real) 1.0 / scale.x);
    nvgTranslate(vg, -offset.x, -offset.y);
    nvgEndFrame(vg);
}

void SVGContext::draw_polygon(const polygon2r& poly, const vec4f& stroke, const vec4f& fill) {
    auto style = _svg_style(stroke, fill);
    svg += fmt::format("<path {} d=\"", style);
    for(auto&& curve : poly) svg += _svg_points(curve) + " ";
    svg += fmt::format("\" />\n");
}

void SVGContext::draw_line(const polyline2r& curve, const vec4f& stroke) {
    auto style = _svg_style(stroke, {0,0,0,0});
    auto points = _svg_points(curve);
    svg += fmt::format("<path {} d=\"{}\" />\n", style, points);
}

void SVGContext::draw_point(const vec2r& p, const vec4f& stroke) {
}

void SVGContext::draw_text(const vec2r& p, const string& text, real size, const vec4f& stroke) {
}

string SVGContext::_svg_style(const vec4f& stroke, const vec4f& fill) {
    std::string svg = "";
    if(stroke.w != 0) svg += fmt::format("stroke=\"rgb({},{},{})\" stroke-opacity=\"{}\"", stroke.x, stroke.y, stroke.z, stroke.w);
    else svg += "stroke=\"none\"";
    svg += " ";
    if(fill.w != 0) svg += fmt::format("fill=\"rgb({},{},{})\" fill-opacity=\"{}\"", fill.x, fill.y, fill.z, fill.w);
    else svg += "fill=\"none\"";
    return svg;
}

string SVGContext::_svg_points(const polyline2r &curve) {
    std:string svg = "";
    auto first = true;
    for(auto&& p : curve) {
        svg += fmt::format("{}{} {} ", (first)?"M":"L", p.x, p.y);
        first = false;
    }
    if(closed_polyline(curve)) svg += "Z";
    return svg;
}

SVGContext::SVGContext() {
    svg = "<!DOCTYPE svg>\n";
}

void SVGContext::begin_frame(const vec2i& wh, const vec2r& offset, const vec2r& scale) {
    svg += "<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"" + to_string(wh.x) + "\" height=\"" + to_string(wh.y) + "\">\n";
    svg += "<g transform=\"translate(" + to_string(offset.x) + "," +to_string(offset.y) + ")\">\n";
    svg += "<g transform=\"scale(" + to_string(scale.x) + "," +to_string(scale.y) + ")\">\n";
}

void SVGContext::end_frame(const vec2r& offset, const vec2r& scale) {
    svg += "</g>\n";
    svg += "</g>\n";
    svg += "</svg>\n";
}
