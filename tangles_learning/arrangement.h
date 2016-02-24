//
//  arrangement.hpp
//  tangles_learning
//
//  Created by Christian on 22/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#ifndef arrangement_h
#define arrangement_h

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
    
    Arrangement() { arrangement = CGAL_arrangement(); }
    
    void test(){
        auto poly = polyline2r({ vec2r((real)0.0, (real)0.0), vec2r((real)1.0, (real)0.0),
                                 vec2r((real)0.0, (real)1.0), vec2r((real)1.0, (real)1.0) });
        print_polyline(poly);
        auto cgal_c = arrangement.to_cgal_curve(poly);
        auto curve = arrangement.from_cgal_curve(cgal_c);
        print_polyline(curve);
        
    }
};

#endif /* arrangement_h */
