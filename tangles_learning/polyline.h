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
polyline2r make_polyline(const polyline2r& curve, const T& f);
template<typename A, typename T>
polyline2r make_polyline(const vector<A>& points, const T& f);

polyline2r operator+(const polyline2r& a, const vec2r& b);
polyline2r operator+(const polyline2r& a, const polyline2r& b);

polyline2r transform_polyline(const frame2r& frame, const polyline2r& curve);
polyline2r transform_polyline_inverse(const frame2r& frame, const polyline2r& curve);
vector<polyline2r> transform_polylines(const frame2r& frame, const vector<polyline2r>& curves);
vector<polyline2r> transform_polylines_inverse(const frame2r& frame, const vector<polyline2r>& curves);

bool closed_polyline(const polyline2r& curve);
polyline2r reverse_polyline(const polyline2r& curve);
polyline2r close_polyline(const polyline2r& curve);
polyline2r remove_doubles_polyline(const polyline2r& curve);

real length_polyline(const polyline2r& curve);
range2r bounds_polyline(const polyline2r& curve);
range2r bounds_polylines(const vector<polyline2r>& curves);

bool inside_polylines(const vector<polyline2r>& curves, const vec2r& p);
bool inside_polyline(const polyline2r& curve, const vec2r& p);

polyline2r make_polyline_segment(const vec2r& a, const vec2r& b, real dist, bool skip_last = false);
polyline2r make_polyline_rect(const vec2r& a, const vec2r& b);
polyline2r make_polyline_rect(const vec2r& a, const vec2r& b, real dist);
polyline2r make_polyline_circle(const vec2r& c, real radius, real dist);

void print_polyline(const polyline2r& curve);
#endif