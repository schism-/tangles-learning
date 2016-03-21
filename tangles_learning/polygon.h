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
polygon2r make_polygon(const polygon2r& poly, const T& f);

template<typename A, typename T>
polygon2r make_polygon(const vector<A>& curves, const T& f);

range2r bounds_polygon(const polygon2r& poly);

range2r bounds_polygons(const vector<polygon2r>& polys);

polygon2r transform_polygon(const frame2r& frame, const polygon2r& poly);

polygon2r transform_polygon_inverse(const frame2r& frame, const polygon2r& poly);

// uses even-odd rule
bool inside_polygon(const polygon2r& poly, const vec2r& p);

bool inside_polygons(const vector<polygon2r>& polys, const vec2r& p);

vector<polygon2r> parse_svg_polygons(const string& svg, real resolution);

#endif /* polygon_h */
