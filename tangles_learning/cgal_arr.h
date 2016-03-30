//
//  cgal_arr.h
//  tangles_learning
//
//  Created by Christian on 22/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#ifndef cgal_arr_h
#define cgal_arr_h

#include "polygon.h"

// Helper functions
void print_face(const Arr_with_hist_2& arr, const Arr_with_hist_2::Face_const_handle& f);
void print_arrangement(const Arr_with_hist_2& arr, bool verbose = false);

Point_2 to_cgal_point(const vec2r& point);
vector<Segment_2> to_cgal_arr_segments(const polyline2r& curve);
vector<Segment_2> to_cgal_curve(const polyline2r& polyline);

vec2r from_cgal_point(const Point_2& point);
polyline2r from_cgal_subcurve(const Subcurve_2& sub);
polyline2r from_cgal_curve(const Polyline_2& polyline);

struct CGAL_data{
    vector<Point_2> point;
    vector<vector<Arr_with_hist_2::Curve_handle>> orig_curves;
    vector<Arr_with_hist_2::Halfedge_handle> halfedge;
    vector<Arr_with_hist_2::Vertex_handle> vertex;
    vector<Arr_with_hist_2::Face_handle> face;
    
    CGAL_data() {
        point = vector<Point_2>();
        orig_curves = vector<vector<Arr_with_hist_2::Curve_handle>>();
        halfedge = vector<Arr_with_hist_2::Halfedge_handle>();
        vertex = vector<Arr_with_hist_2::Vertex_handle>();
        face = vector<Arr_with_hist_2::Face_handle>();
    }
    
    ~CGAL_data() {}
};


struct CGAL_arrangement {
    Geom_traits_2 traits;
    Arr_with_hist_2 arr;
    
    CGAL_arrangement() {
        arr = Arr_with_hist_2(&traits);
    }
    ~CGAL_arrangement() {}
    
    pair<Curve_handle_2, CGAL_data> add_curve(const polyline2r& curve) {
        auto cgal_curve = to_cgal_curve(curve);
        Geom_traits_2::Construct_curve_2 obj = traits.construct_curve_2_object();
        Polyline_2 pi2 = obj(cgal_curve.begin(), cgal_curve.end());
        
        auto cgal_data = polyline_intersect(arr, curve);
        auto handle = insert(arr, pi2);
        std::cout << "Added curve" << std::endl;

        std::cout << "-H Intersections (" << cgal_data.point.size() << ")"<< std::endl;
        for (auto i : cgal_data.point) std::cout << i.x() << " " << i.y() << std::endl;

        std::cout << "- V Intersections (" << cgal_data.vertex.size() << ")"<< std::endl;
        for (auto i : cgal_data.vertex) std::cout << &i << std::endl;

        
        std::cout << "- F Intersections (" << cgal_data.face.size() << ")"<< std::endl;
        for (auto i : cgal_data.face) std::cout << &i << std::endl;

        return make_pair(handle, cgal_data);
    }
    
    void add_curves(const vector<polyline2r>& curves) {
        for (auto&& c : curves) add_curve(c);
    }
    
    void segment_intersect(Arr_with_hist_2 &arr, Segment_2 &c){
        std::vector<CGAL::Object> zone_elems;
        Arr_with_hist_2::Halfedge_handle face;
        auto obj = traits.construct_x_monotone_curve_2_object();
        
        auto pi2 = obj(c.source(), c.target());
        zone(arr, pi2, std::back_inserter(zone_elems));
        for ( int i = 0; i < (int)zone_elems.size(); ++i ){
            if ( assign(face, zone_elems[i]) )
                std::cout << "boh" << std::endl;
        }
    }
    
    CGAL_data polyline_intersect(Arr_with_hist_2 &arr, const polyline2r& curve){
        std::vector<CGAL::Object> zone_elems;
        // Transform the curve into a list of x-monotone curves, for CGAL zone
        auto cgal_curve = to_cgal_curve(curve);
        auto make_curve = traits.construct_curve_2_object();
        auto c2 = make_curve(cgal_curve.begin(), cgal_curve.end());
        auto to_mono = traits.make_x_monotone_2_object();
        std::vector<CGAL::Object> mono_curves;
        to_mono(c2, std::back_inserter(mono_curves));
        
        auto intersections = std::set<Point_2>();
        auto added_points = std::set<Point_2>();
        auto intersection_data = CGAL_data();
        
        auto int_2 = vector<pair<Point_2, vector<Arr_with_hist_2::Curve_handle>>>();
        
        for (auto m_c : mono_curves){
            Geom_traits_2::X_monotone_curve_2 curr;
            bool check = CGAL::assign(curr, m_c);
            assert(check); CGAL_USE(check);
            zone(arr, curr, std::back_inserter(zone_elems));
            
            for ( int i = 0; i < (int)zone_elems.size(); ++i ){
                Arr_with_hist_2::Face_handle face;
                Arr_with_hist_2::Halfedge_handle half;
                Arr_with_hist_2::Vertex_handle vertex;
                
                // Check for intersection with halfedges
                bool check_half = CGAL::assign(half, zone_elems[i]);
                if (check_half){
                    auto temp = vector<Polyline_2>();
                    temp.push_back(c2);
                    temp.push_back(half->curve());
                    std::list<Point_2> pts;
                    CGAL::compute_intersection_points(temp.begin(), temp.end(), std::back_inserter(pts), false, traits);
                    if (!pts.empty()){
                        for (auto pt : pts){
                            if (std::find(added_points.begin(), added_points.end(), pt) == added_points.end()){
                                //Finding originating curves of intersected halfedges
                                auto orig_curves = vector<Arr_with_hist_2::Curve_handle>();
                                for (auto ocit = arr.originating_curves_begin(half); ocit != arr.originating_curves_end(half); ++ocit)
                                    orig_curves.push_back(ocit);
                                
                                intersection_data.point.push_back(pt);
                                intersection_data.orig_curves.push_back(orig_curves);
                                intersection_data.halfedge.push_back(half);
                                
                                int_2.push_back(make_pair(pt, orig_curves));
                                added_points.insert(pt);
                            }
                        }
                    }
                }
                
                // Check for intersection with faces
                bool check_face = CGAL::assign(face, zone_elems[i]);
                if (check_face && (std::find(intersection_data.face.begin(), intersection_data.face.end(), face) == intersection_data.face.end())){
                    if (!face->is_unbounded()) intersection_data.face.push_back(face);
                }
                
                // Check for intersection with vertices
                bool check_vert = CGAL::assign(vertex, zone_elems[i]);
                if (check_vert && std::find(intersection_data.vertex.begin(), intersection_data.vertex.end(), vertex) == intersection_data.vertex.end()){
                    intersection_data.vertex.push_back(vertex);
                }
            }
        }
        return intersection_data;
    }
    
    void polyline_intersect_2(Arr_with_hist_2 &arr, const polyline2r& curve){
    
        auto cgal_curve = to_cgal_curve(curve);
        auto make_curve = traits.construct_curve_2_object();
        auto c2 = make_curve(cgal_curve.begin(), cgal_curve.end());
        
    }
};
#endif /* cgal_arr_h */
