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

#include "json.hpp"
using json = nlohmann::json;


vector<polygon2r> get_shapes(CGAL_arrangement* cgal_arr);

struct Shape {
    polygon2r poly;
    frame2r frame;
    vector<polyline2r> aux_lines;
    bool invert;
    
    Shape(const polygon2r& poly, const frame2r& frame, const vector<polyline2r>& aux_lines) : poly(poly), frame(frame), aux_lines(aux_lines) { invert = false; };
    
    ~Shape() {};
};

struct Curve {
    polyline2r curve;
    Curve_handle_2 handle;
    vector<pair<vec2r, vector<Arr_with_hist_2::Curve_handle>>> intersections;
    //Add curve attributes
    Curve(const polyline2r& poly, const Curve_handle_2& handle, const vector<pair<vec2r, vector<Arr_with_hist_2::Curve_handle>>>& intersections) :
            curve(poly), handle(handle), intersections(intersections) {};
    ~Curve() {};
    
    range2r bounds() const {
        return bounds_polyline(curve);
    }
};

struct Tangle {
    vector<Shape*> shapes;
    vector<Curve*> curves;
    
    CGAL_arrangement* arrangement;
    
    Tangle() {
        arrangement = new CGAL_arrangement();
        shapes = vector<Shape*>();
        curves = vector<Curve*>();
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
    
    void load_instructions(const string& filename){
        string json_string, temp;
        json_string = load_text_file(filename);
        
        auto json_data = json::parse(json_string);
        
        std::cout << json_data << std::endl;
        
        auto svg_base = load_text_file(json_data["canvas"]);
        auto base_shapes = parse_svg_polygons(svg_base, POLY_RES);
        
        for (auto bs : base_shapes){
            for (auto poly : bs) add_curve(poly);
        }
        
        for (auto step : json_data["steps"]){
            std::cout << "step found" << std::endl;
        }
    }
    
    void update_tangle(){
        auto polys = get_shapes(arrangement);
        shapes = vector<Shape*>();
        for (auto p : polys){
            shapes.push_back(new Shape(p, frame2r(), {}));
        }
        std::cout << shapes.size() << " shapes in arrangement" << std::endl;
    }
    
    void add_curve(const polyline2r& curve){
        auto c_h = arrangement->add_curve(curve);
        auto c_obj = new Curve(curve, c_h.first, c_h.second);
        curves.push_back(c_obj);
        update_tangle();
    }
    
    void test(){
        add_curve(make_polyline_circle(vec2r((real)0.0, (real)0.0), (real)100.0, (real)2.0));
        add_curve(make_polyline_circle(vec2r((real)50.0, (real)0.0), (real)100.0, (real)2.0));
        add_curve(make_polyline_circle(vec2r((real)-50.0, (real)0.0), (real)100.0, (real)2.0));
    }
    
    void print_cgal_arrangement(){
        print_arrangement(arrangement->arr, true);
    }
    
    Shape* select_shape(const vec2r& mouse_pos){
        auto picked = (Shape*)nullptr;
        for(auto&& shape : shapes) {
            if(inside_polygon(shape->poly, mouse_pos)) picked = shape;
            if(picked) break;
        }
        if(not picked) return nullptr;
        return picked;
    }
};

#endif /* arrangement_h */
