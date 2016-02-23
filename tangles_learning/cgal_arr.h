//
//  cgal_arr.h
//  tangles_learning
//
//  Created by Christian on 22/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#ifndef cgal_arr_h
#define cgal_arr_h

//#include <CGAL/Cartesian.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_on_surface_with_history_2.h>
#include <CGAL/Arrangement_with_history_2.h>
#include <CGAL/Arr_simple_point_location.h>
#include <CGAL/Arr_polyline_traits_2.h>

#include "polygon.h"

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Arr_segment_traits_2<Kernel>                Segment_traits_2;
typedef CGAL::Arr_polyline_traits_2<Segment_traits_2>     Geom_traits_2;
typedef Geom_traits_2::Point_2                            Point_2;
typedef Geom_traits_2::Segment_2                          Segment_2;
typedef Geom_traits_2::Curve_2                            Polyline_2;
typedef CGAL::Arrangement_with_history_2<Geom_traits_2>   Arr_with_hist_2;


struct CGAL_arrangement {
    Arr_with_hist_2 arr;
    
    CGAL_arrangement() { arr = Arr_with_hist_2(); }
    
    void add_curve(const polyline2r& curve) {
    
    }
    
    void add_curves(const vector<polyline2r>& curves) {
        
    }
    
    Point_2 to_cgal_point(const vec2r& point){
        return Point_2(point.x, point.y);
    }
    
    vec2r from_cgal_point(const Point_2& point){
        return vec2r((real)(CGAL::to_double(point.x())), (real)(CGAL::to_double(point.y())));
    }
    
};

#endif /* cgal_arr_h */
