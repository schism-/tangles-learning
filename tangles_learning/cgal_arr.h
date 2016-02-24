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

struct CGAL_arrangement {
    Geom_traits_2 traits;
    Arr_with_hist_2 arr;
    
    CGAL_arrangement() { arr = Arr_with_hist_2(); }
    
    ~CGAL_arrangement() {}
    
    void add_curve(const polyline2r& curve) {
        insert(arr, to_cgal_curve(curve));
    }
    
    void add_curves(const vector<polyline2r>& curves) {
        for (auto&& c : curves) add_curve(c);
    }
    
    Point_2 to_cgal_point(const vec2r& point){
        return Point_2(point.x, point.y);
    }
    
    vec2r from_cgal_point(const Point_2& point){
        return vec2r((real)(CGAL::to_double(point.x())), (real)(CGAL::to_double(point.y())));
    }
    
    Polyline_2 to_cgal_curve(const polyline2r& polyline){
        Geom_traits_2::Construct_curve_2 polyline_construct = traits.construct_curve_2_object();
        vector<Point_2> points2;
        for (auto&& p : polyline) points2.push_back(to_cgal_point(p));
        Polyline_2 pi2 = polyline_construct(points2.begin(), points2.end());
        return pi2;
    }
    
    polyline2r from_cgal_subcurve(const Subcurve_2& sub){
        return {from_cgal_point(sub.source()), from_cgal_point(sub.target())};
    }
    
    polyline2r from_cgal_curve(const Polyline_2& polyline){
        auto res = polyline2r();
        for(auto sit = polyline.begin_subcurves(); sit != polyline.end_subcurves(); ++sit){
            auto sub = from_cgal_subcurve(*sit);
            res += sub;
        }
        return remove_doubles_polyline(res);
    }
};

// Helper functions
void print_face(const Arr_with_hist_2& arr, const Arr_with_hist_2::Face_const_handle& f);
void print_arrangement(const Arr_with_hist_2& arr);
vector<polygon2r> get_shapes(CGAL_arrangement* cgal_arr);

#endif /* cgal_arr_h */
