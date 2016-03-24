//
//  polygon.cpp
//  tangles_learning
//
//  Created by Christian on 15/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#include "polygon.h"

template<typename T>
polygon2r make_polygon(const polygon2r& poly, const T& f) {
    auto nc = polygon2r(poly.size());
    for(auto i : range(poly)) nc[i] = f(poly[i]);
    return nc;
}

template<typename A, typename T>
polygon2r make_polygon(const vector<A>& curves, const T& f) {
    auto nc = polygon2r(curves.size());
    for(auto i : range(curves)) nc[i] = f(curves[i]);
    return nc;
}

range2r bounds_polygon(const polygon2r& poly) { return bounds_polyline(poly[0]); }

range2r bounds_polygons(const vector<polygon2r>& polys) {
    auto bbox = range2r();
    for(auto&& poly : polys) bbox = runion(bbox,bounds_polygon(poly));
    return bbox;
}

polygon2r transform_polygon(const frame2r& frame, const polygon2r& poly) {
    return make_polygon(poly, [&](const polyline2r& curve){ return transform_polyline(frame,curve); });
}

polygon2r transform_polygon_inverse(const frame2r& frame, const polygon2r& poly) {
    return make_polygon(poly, [&](const polyline2r& curve){ return transform_polyline_inverse(frame,curve); });
}

// uses even-odd rule
bool inside_polygon(const polygon2r& poly, const vec2r& p) { return inside_polylines(poly, p); }

bool inside_polygons(const vector<polygon2r>& polys, const vec2r& p) {
    for(auto&& poly : polys) if(inside_polylines(poly, p)) return true;
    return false;
}

// svg

vector<polygon2r> parse_svg_polygons(const string& svg, real resolution) {
    auto curves = parse_svg_polylines(svg, resolution);
//    for(auto& curve : curves) curve = close_polyline(curve);
    auto inside = vector<vector<bool>>(curves.size());
    for(auto i : range(curves)) {
        inside[i] = vector<bool>(curves.size(),false);
        for(auto j : range(curves)) {
            if(i == j) continue;
            auto ins = true;
            //            for(auto&& p : curves[i]) if(inside_polyline(curves[j], p)) { inside[i][j] = true; break; }
            for(auto&& p : curves[i]) ins = ins and inside_polyline(curves[j], p);
            if (ins) inside[i][j] = true;
        }
    }
    auto polys = vector<polygon2r>();
    for(auto i : range(curves)) {
        auto inner = false;
        for(auto j : range(curves)) inner = inner or inside[i][j];
        if(inner) continue;
        polys += {curves[i]};
        for(auto j : range(curves)) if(inside[j][i]) polys.back() += curves[j];
    }
    return polys;
}