//
//  config.h
//  tangles_learning
//
//  Created by Christian on 15/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "vmath.h"

#if 0

using real = float;
using vec2r = vec2f;
using vec4r = vec4f;
using frame2r = frame2f;
using range2r = range2f;

#endif

using real = double;
using vec2r = vec2d;
using vec4r = vec4d;
using frame2r = frame2d;
using range2r = range2d;

const vec2r zero2r = vec2r(0, 0);
const vec2r one2r = vec2r(1, 1);
const vec2r x2r = vec2r(1, 0);
const vec2r y2r = vec2r(0, 1);
const frame2r identity_frame2r = frame2r();

inline vec2r to_vec2r(const vec2f& v) { return {v.x,v.y}; }
inline vec2f to_vec2f(const vec2r& v) { return {(float)v.x,(float)v.y}; }

inline range2r make_range2r(const std::initializer_list<vec2r>& points) { auto bbox = range2r(); for(auto& p : points) bbox = runion(bbox,p); return bbox; }
inline range2r make_range2r(const vector<vec2r>& points) { auto bbox = range2r(); for(auto& p : points) bbox = runion(bbox,p); return bbox; }

#endif