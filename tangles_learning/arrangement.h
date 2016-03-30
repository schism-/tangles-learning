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
    CGAL_data intersection_data;
    //Add curve attributes
    Curve(const polyline2r& poly, const Curve_handle_2& handle, const CGAL_data& intersections) :
            curve(poly), handle(handle), intersection_data(intersection_data) {};
    ~Curve() {};
    
    range2r bounds() const {
        return bounds_polyline(curve);
    }
};

struct Tangle {
    vector<Shape*> shapes;
    vector<Curve*> curves;
    vector<Curve*> base_curves;
    
    CGAL_arrangement* arrangement;
    
    Tangle() {
        arrangement = new CGAL_arrangement();
        shapes = vector<Shape*>();
        curves = vector<Curve*>();
        base_curves = vector<Curve*>();
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
        
        auto first = true;
        for (auto bs : base_shapes){
            for (auto poly : bs){
                if (first){
                    first = false;
                    add_curve(poly, true);
                    
                }
                else add_curve(poly);
            }
        }
        
        analyze_step();
    }
    
    void update_tangle(){
        auto polys = get_shapes(arrangement);
        shapes = vector<Shape*>();
        for (auto p : polys){
            shapes.push_back(new Shape(p, frame2r(), {}));
        }
        std::cout << shapes.size() << " shapes in arrangement" << std::endl;
    }
    
    void analyze_step(){
        std::cout << "== STEP ANALYSIS ==" << std::endl;
        for (auto i = 0;  i < (int)curves.size(); i++){
            // POI WRT FACE(S)
            std::cout << "curve" << std::endl;
        }
    }
    
    void add_curve(const polyline2r& curve, bool is_base_curve = false){
        auto c_h = arrangement->add_curve(curve);
        auto c_obj = new Curve(curve, c_h.first, c_h.second);
        
        if (is_base_curve) base_curves.push_back(c_obj);
        else curves.push_back(c_obj);
        
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
