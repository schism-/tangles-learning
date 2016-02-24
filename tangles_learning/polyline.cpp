//
//  polyline.cpp
//  tangles_learning
//
//  Created by Christian on 15/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#include "polyline.h"

template<typename T>
polyline2r make_polyline(const polyline2r& curve, const T& f) {
    auto nc = polyline2r(curve.size());
    for(auto i : range(curve)) nc[i] = f(curve[i]);
    return nc;
}

template<typename A, typename T>
polyline2r make_polyline(const vector<A>& points, const T& f) {
    auto nc = polyline2r(points.size());
    for(auto i : range(points)) nc[i] = f(points[i]);
    return nc;
}

polyline2r operator+(const polyline2r& a, const vec2r& b) { auto c = a; c.push_back(b); return c; }

polyline2r operator+(const polyline2r& a, const polyline2r& b) { auto c = a; for(auto&& p : b) c.push_back(p); return c; }

polyline2r transform_polyline(const frame2r& frame, const polyline2r& curve) {
    return make_polyline(curve,[&](const vec2r& p){ return transform_point(frame, p); });
}

polyline2r transform_polyline_inverse(const frame2r& frame, const polyline2r& curve) {
    return make_polyline(curve,[&](const vec2r& p){ return transform_point_inverse(frame, p); });
}

vector<polyline2r> transform_polylines(const frame2r& frame, const vector<polyline2r>& curves) {
    return make_vector(curves, [&](const polyline2r& curve){ return transform_polyline(frame,curve); });
}

vector<polyline2r> transform_polylines_inverse(const frame2r& frame, const vector<polyline2r>& curves) {
    return make_vector(curves, [&](const polyline2r& curve){ return transform_polyline_inverse(frame,curve); });
}

bool closed_polyline(const polyline2r& curve) {
    return curve.front() == curve.back();
}

void print_polyline(const polyline2r& curve) {
    for(auto i : range((int)curve.size())) printf("(%f, %f) -> ", curve[i].x, curve[i].y);
    printf("\n");
}

polyline2r reverse_polyline(const polyline2r& curve) {
    auto ret = curve; std::reverse(ret.begin(), ret.end()); return ret;
}

polyline2r close_polyline(const polyline2r& curve) {
    if(closed_polyline(curve)) return curve;
    else return curve + curve[0];
}

polyline2r remove_doubles_polyline(const polyline2r& curve){
    
}

real length_polyline(const polyline2r& curve) {
    auto l = 0.0f;
    for(auto i : range((int)curve.size()-1)) l += length(curve[i]-curve[i+1]);
    return l;
}

range2r bounds_polyline(const polyline2r& curve) { return make_range2r(curve); }

range2r bounds_polylines(const vector<polyline2r>& curves) {
    auto bbox = range2r();
    for(auto& curve : curves) bbox = runion(bbox, bounds_polyline(curve));
    return bbox;
}

// uses even-odd rule
bool inside_polyline(const polyline2r& curve, const vec2r& p) {
    return inside_polylines({curve}, p);
}

polyline2r make_polyline_segment(const vec2r& a, const vec2r& b, real dist, bool skip_last) {
    auto steps = round(length(a-b) / dist);
    auto curve = polyline2r();
    for(auto i : range(steps)) {
        auto t = real(i) / real(steps);
        curve += a*(1-t)+b*t;
    }
    if(not skip_last) curve += b;
    return curve;
}

polyline2r make_polyline_rect(const vec2r& a, const vec2r& b) {
    return { {
        {min(a.x,b.x),min(a.y,b.y)},
        {max(a.x,b.x),min(a.y,b.y)},
        {max(a.x,b.x),max(a.y,b.y)},
        {min(a.x,b.x),max(a.y,b.y)},
        {min(a.x,b.x),min(a.y,b.y)},
    } };
}

polyline2r make_polyline_rect(const vec2r& a, const vec2r& b, real dist) {
    auto points = make_polyline_rect(a,b);
    return
    make_polyline_segment(points[0],points[1],dist,true) +
    make_polyline_segment(points[1],points[2],dist,true) +
    make_polyline_segment(points[2],points[3],dist,true) +
    make_polyline_segment(points[3],points[4],dist,true) +
    points[4];
}

polyline2r make_polyline_circle(const vec2r& c, real radius, real dist) {
    auto steps = round(2*pif*radius / dist);
    auto curve = polyline2r();
    for(auto i : range(steps)) {
        auto theta = 2 * pif * real(i) / real(steps);
        curve += c + radius * vec2r{cos(theta),sin(theta)};
    }
    curve = close_polyline(curve);
    return curve;
}

bool inside_polylines(const vector<polyline2r>& curves, const vec2r& p) {
    auto flips = false;
    for(auto&& curve : curves) {
        for(auto i : range((int)curve.size()-1)) {
            if ( ((curve[i].y>p.y) != (curve[i+1].y>p.y)) &&
                (p.x < (curve[i].x-curve[i+1].x) * (p.y-curve[i].y) /
                 (curve[i+1].y-curve[i].y) + curve[i].x) )
                flips = not flips;
        }
    }
    return flips;
}