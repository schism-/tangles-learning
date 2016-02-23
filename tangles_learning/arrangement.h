//
//  arrangement.hpp
//  tangles_learning
//
//  Created by Christian on 22/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#ifndef arrangement_h
#define arrangement_h

#include "config.h"
#include "cgal_arr.h"

struct Shape {
    polygon2r poly;

    frame2r frame;
    
    vector<polyline2r> aux_lines;
};

using POI = vec2r;

struct Arrangement {
    vector<Shape*> shapes;

    vector<POI*> intersections;

    CGAL_arrangement arrangement;
};

#endif /* arrangement_h */
