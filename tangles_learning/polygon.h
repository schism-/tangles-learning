//
//  polygon.hpp
//  tangles_learning
//
//  Created by Christian on 15/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#ifndef polygon_h
#define polygon_h

#include "polyline.h"

struct polygon2r : vector<polyline2r> {
    using vector<polyline2r>::vector;
};

template<typename T>
inline polygon2r make_polygon(const polygon2r& poly, const T& f) {
    auto nc = polygon2r(poly.size());
    for(auto i : range(poly)) nc[i] = f(poly[i]);
    return nc;
}

template<typename A, typename T>
inline polygon2r make_polygon(const vector<A>& curves, const T& f) {
    auto nc = polygon2r(curves.size());
    for(auto i : range(curves)) nc[i] = f(curves[i]);
    return nc;
}

inline range2r bounds_polygon(const polygon2r& poly) { return bounds_polyline(poly[0]); }
inline range2r bounds_polygons(const vector<polygon2r>& polys) {
    auto bbox = range2r();
    for(auto&& poly : polys) bbox = runion(bbox,bounds_polygon(poly));
    return bbox;
}

inline polygon2r transform_polygon(const frame2r& frame, const polygon2r& poly) {
    return make_polygon(poly, [&](const polyline2r& curve){ return transform_polyline(frame,curve); });
}
inline polygon2r transform_polygon_inverse(const frame2r& frame, const polygon2r& poly) {
    return make_polygon(poly, [&](const polyline2r& curve){ return transform_polyline_inverse(frame,curve); });
}

// uses even-odd rule
inline bool inside_polygon(const polygon2r& poly, const vec2r& p) { return inside_polylines(poly, p); }
inline bool inside_polygons(const vector<polygon2r>& polys, const vec2r& p) {
    for(auto&& poly : polys) if(inside_polylines(poly, p)) return true;
    return false;
}

#endif /* polygon_h */
