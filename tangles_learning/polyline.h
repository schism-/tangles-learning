//
//  polyline.h
//  tangles_learning
//
//  Created by Christian on 15/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#ifndef _POLYLINE_H_
#define _POLYLINE_H_

#include "config.h"

struct polyline2r : vector<vec2r> {
    using vector<vec2r>::vector;
};

template<typename T>
inline polyline2r make_polyline(const polyline2r& curve, const T& f) {
    auto nc = polyline2r(curve.size());
    for(auto i : range(curve)) nc[i] = f(curve[i]);
    return nc;
}

template<typename A, typename T>
inline polyline2r make_polyline(const vector<A>& points, const T& f) {
    auto nc = polyline2r(points.size());
    for(auto i : range(points)) nc[i] = f(points[i]);
    return nc;
}

inline polyline2r operator+(const polyline2r& a, const vec2r& b) { auto c = a; c.push_back(b); return c; }
inline polyline2r operator+(const polyline2r& a, const polyline2r& b) { auto c = a; for(auto&& p : b) c.push_back(p); return c; }

inline polyline2r transform_polyline(const frame2r& frame, const polyline2r& curve) {
    return make_polyline(curve,[&](const vec2r& p){ return transform_point(frame, p); });
}
inline polyline2r transform_polyline_inverse(const frame2r& frame, const polyline2r& curve) {
    return make_polyline(curve,[&](const vec2r& p){ return transform_point_inverse(frame, p); });
}

inline vector<polyline2r> transform_polylines(const frame2r& frame, const vector<polyline2r>& curves) {
    return make_vector(curves, [&](const polyline2r& curve){ return transform_polyline(frame,curve); });
}
inline vector<polyline2r> transform_polylines_inverse(const frame2r& frame, const vector<polyline2r>& curves) {
    return make_vector(curves, [&](const polyline2r& curve){ return transform_polyline_inverse(frame,curve); });
}

inline bool closed_polyline(const polyline2r& curve) {
    return curve.front() == curve.back();
}

inline polyline2r reverse_polyline(const polyline2r& curve) {
    auto ret = curve; std::reverse(ret.begin(), ret.end()); return ret;
}

inline polyline2r close_polyline(const polyline2r& curve) {
    if(closed_polyline(curve)) return curve;
    else return curve + curve[0];
}

inline real length_polyline(const polyline2r& curve) {
    auto l = 0.0f;
    for(auto i : range((int)curve.size()-1)) l += length(curve[i]-curve[i+1]);
    return l;
}

inline range2r bounds_polyline(const polyline2r& curve) { return make_range2r(curve); }

inline range2r bounds_polylines(const vector<polyline2r>& curves) {
    auto bbox = range2r();
    for(auto& curve : curves) bbox = runion(bbox, bounds_polyline(curve));
    return bbox;
}

// uses even-odd rule
bool inside_polylines(const vector<polyline2r>& curves, const vec2r& p);

inline bool inside_polyline(const polyline2r& curve, const vec2r& p) {
    return inside_polylines({curve}, p);
}

inline polyline2r make_polyline_segment(const vec2r& a, const vec2r& b, real dist, bool skip_last = false) {
    auto steps = round(length(a-b) / dist);
    auto curve = polyline2r();
    for(auto i : range(steps)) {
        auto t = real(i) / real(steps);
        curve += a*(1-t)+b*t;
    }
    if(not skip_last) curve += b;
    return curve;
}

inline polyline2r make_polyline_rect(const vec2r& a, const vec2r& b) {
    return { {
        {min(a.x,b.x),min(a.y,b.y)},
        {max(a.x,b.x),min(a.y,b.y)},
        {max(a.x,b.x),max(a.y,b.y)},
        {min(a.x,b.x),max(a.y,b.y)},
        {min(a.x,b.x),min(a.y,b.y)},
    } };
}

inline polyline2r make_polyline_rect(const vec2r& a, const vec2r& b, real dist) {
    auto points = make_polyline_rect(a,b);
    return
    make_polyline_segment(points[0],points[1],dist,true) +
    make_polyline_segment(points[1],points[2],dist,true) +
    make_polyline_segment(points[2],points[3],dist,true) +
    make_polyline_segment(points[3],points[4],dist,true) +
    points[4];
}

inline polyline2r make_polyline_circle(const vec2r& c, real radius, real dist) {
    auto steps = round(2*pif*radius / dist);
    auto curve = polyline2r();
    for(auto i : range(steps)) {
        auto theta = 2 * pif * real(i) / real(steps);
        curve += c + radius * vec2r{cos(theta),sin(theta)};
    }
    curve = close_polyline(curve);
    return curve;
}
#endif