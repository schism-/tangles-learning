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

vector<polygon2r> get_shapes(CGAL_arrangement* cgal_arr);

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
        arrangement->add_curve(make_polyline_rect({-100.0, -100.0}, {100.0, 100.0}, (real)2.0));
        arrangement->add_curve(make_polyline_rect({-200.0, -200.0}, {0.0, 0.0}, (real)10.0));
        arrangement->add_curve(make_polyline_circle(vec2r((real)0.0, (real)0.0), (real)50.0, (real)2.0));
        arrangement->add_curve(make_polyline_segment({-400.0, -400.0}, {150.0, 150.0}, (real)1.0));
        arrangement->add_curve(make_polyline_segment({400.0, -400.0}, {0.0, 0.0}, (real)1.0));
        arrangement->add_curve(make_polyline_segment({0.0, 200.0}, {-100.0, -300.0}, (real)1.0));
        update_tangle();
    }
    
    void print_cgal_arrangement(){
        print_arrangement(arrangement->arr);
    }
};

#endif /* arrangement_h */
