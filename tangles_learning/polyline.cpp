//
//  polyline.cpp
//  tangles_learning
//
//  Created by Christian on 15/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#include "polyline.h"

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