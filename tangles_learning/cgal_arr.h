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

struct CGAL_arrangement {
    Geom_traits_2 traits;
    Arr_with_hist_2 arr;
    
    CGAL_arrangement() {
        arr = Arr_with_hist_2(&traits);
    }
    ~CGAL_arrangement() {}
    
    pair<Curve_handle_2, vector<pair<vec2r, vector<Arr_with_hist_2::Curve_handle>>>> add_curve(const polyline2r& curve) {
        auto cgal_curve = to_cgal_curve(curve);
        Geom_traits_2::Construct_curve_2 obj = traits.construct_curve_2_object();
        Polyline_2 pi2 = obj(cgal_curve.begin(), cgal_curve.end());
        
        auto cgal_intersections = polyline_intersect(arr, curve);
        auto handle = insert(arr, pi2);
        std::cout << "Added curve" << std::endl;
        std::cout << "- Intersections (" << cgal_intersections.size() << ")"<< std::endl;
        
        auto intersections = vector<pair<vec2r, vector<Arr_with_hist_2::Curve_handle>>>();
        for (auto i : cgal_intersections)
            intersections.push_back(make_pair(from_cgal_point(i.first), i.second));
        
        for (auto i : intersections) std::cout << i.first.x << " " << i.first.y << std::endl;
        
        return make_pair(handle, intersections);
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
    
    vector<pair<Point_2, vector<Arr_with_hist_2::Curve_handle>>> polyline_intersect(Arr_with_hist_2 &arr, const polyline2r& curve){
        std::vector<CGAL::Object> zone_elems;
        Arr_with_hist_2::Halfedge_handle face;
        Arr_with_hist_2::Vertex_handle vertex;

        auto cgal_curve = to_cgal_curve(curve);
        auto make_curve = traits.construct_curve_2_object();
        auto c2 = make_curve(cgal_curve.begin(), cgal_curve.end());
        
        auto to_mono = traits.make_x_monotone_2_object();
        std::vector<CGAL::Object> mono_curves;
        to_mono(c2, std::back_inserter(mono_curves));
        
        auto intersections = std::set<Point_2>();
        auto added_points = std::set<Point_2>();
        
        auto int_2 = vector<pair<Point_2, vector<Arr_with_hist_2::Curve_handle>>>();
        
        for (auto m_c : mono_curves){
            Geom_traits_2::X_monotone_curve_2 curr;
            bool check = CGAL::assign(curr, m_c);
            assert(check); CGAL_USE(check);
            zone(arr, curr, std::back_inserter(zone_elems));
            for ( int i = 0; i < (int)zone_elems.size(); ++i ){
                bool check_half = CGAL::assign(face, zone_elems[i]);
                if (check_half){
                    auto temp = vector<Polyline_2>();
                    temp.push_back(c2);
                    temp.push_back(face->curve());
                    std::list<Point_2> pts;
                    CGAL::compute_intersection_points(temp.begin(), temp.end(), std::back_inserter(pts), false, traits);
                    if (!pts.empty()){
                        for (auto pt : pts){
                            intersections.insert(pt);
                            auto orig_curves = vector<Arr_with_hist_2::Curve_handle>();
                            for (auto ocit = arr.originating_curves_begin(face); ocit != arr.originating_curves_end(face); ++ocit)
                                orig_curves.push_back(ocit);
                            if (std::find(added_points.begin(), added_points.end(), pt) == added_points.end()){
                                int_2.push_back(make_pair(pt, orig_curves));
                                added_points.insert(pt);
                            }
                        }
                    }
                }
                bool check_vert = CGAL::assign(vertex, zone_elems[i]);
                if (check_vert) intersections.insert(vertex->point());
            }
        }
        return int_2;
    }
    
    void polyline_intersect_2(Arr_with_hist_2 &arr, const polyline2r& curve){
    
        auto cgal_curve = to_cgal_curve(curve);
        auto make_curve = traits.construct_curve_2_object();
        auto c2 = make_curve(cgal_curve.begin(), cgal_curve.end());
        
    }
};
#endif /* cgal_arr_h */
