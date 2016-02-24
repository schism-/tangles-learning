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
    bool invert;
    
    Shape(const polygon2r& poly, const frame2r& frame, const vector<polyline2r>& aux_lines) : poly(poly), frame(frame), aux_lines(aux_lines) { invert = false; };
    
    ~Shape() {};
};

using POI = vec2r;

struct Tangle {
    vector<Shape*> shapes;
    vector<POI*> intersections;
    CGAL_arrangement* arrangement;
    
    Tangle() {
        arrangement = new CGAL_arrangement();
        shapes = vector<Shape*>();
        intersections = vector<POI*>();
    };
    
    ~Tangle() {};
    
    range2r bounds() const {
        auto bbox = range2r();
        for(auto shape : shapes) {
            if(shape->poly.empty()) continue;
            bbox = runion(bbox, bounds_polygon(shape->poly));
        }
        return bbox;
    }
    
    void update_tangle(){
        auto polys = get_shapes(arrangement);
        shapes = vector<Shape*>();
        for (auto&& p : polys){
            shapes.push_back(new Shape(p, frame2r(), {}));
        }
    }
    
    void test(){
        auto poly = polyline2r({ vec2r((real)0.0, (real)0.0), vec2r((real)100.0, (real)0.0),
                                 vec2r((real)0.0, (real)100.0), vec2r((real)100.0, (real)100.0) });
        
        auto poly_2 = polyline2r({ vec2r((real)-200.0, (real)-200.0), vec2r((real)-200.0, (real)200.0),
                                   vec2r((real)200.0, (real)200.0), vec2r((real)200.0, (real)-200.0),
                                   vec2r((real)-200.0, (real)-200.0) });
        print_polyline(poly);
        auto cgal_c = arrangement->to_cgal_curve(poly);
        auto curve = arrangement->from_cgal_curve(cgal_c);
        print_polyline(curve);
        arrangement->add_curve(poly);
        arrangement->add_curve(poly_2);
        update_tangle();
    }
    
    void print_cgal_arrangement(){
        print_arrangement(arrangement->arr);
    }
};

#endif /* arrangement_h */
