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
    
    void add_curve(const polyline2r& curve) {
        auto cgal_curve = to_cgal_curve(curve);
        Geom_traits_2::Construct_curve_2 obj = traits.construct_curve_2_object();
        Polyline_2 pi2 = obj(cgal_curve.begin(), cgal_curve.end());
        insert(arr, pi2);
        std::cout << "Added curve" << std::endl;
    }
    
    void add_curves(const vector<polyline2r>& curves) {
        for (auto&& c : curves) add_curve(c);
    }
};
#endif /* cgal_arr_h */
