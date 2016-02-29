//
//  arrangement.cpp
//  tangles_learning
//
//  Created by Christian on 22/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#include "arrangement.h"

vector<polygon2r> get_shapes(CGAL_arrangement* cgal_arr){
    auto arr = cgal_arr->arr;
    auto res = vector<polygon2r>();
    for (auto fit = arr.faces_begin(); fit != arr.faces_end(); ++fit){
        auto pts = polyline2r();
        auto holes = vector<polyline2r>();
        auto res_poly = polygon2r();
        
        if (!fit->is_unbounded()){
            auto cc = fit->outer_ccb();
            do {
                std::cout << cc->direction() << std::endl;
                std::cout << cc->source()->point() << std::endl;
                std::cout << cc->curve() << std::endl;
                std::cout << cc->target()->point() << std::endl;
                std::cout << std::endl;
                if (cc->source()->point() != cc->curve().begin_subcurves()->source()){
                    pts += reverse_polyline(from_cgal_curve(cc->curve()));
                }
                else {
                    pts += from_cgal_curve(cc->curve());
                }
            } while (++cc != fit->outer_ccb());
            res_poly.push_back(pts);
            std::cout << std::endl;
            std::cout << std::endl;
//            for (auto hi = fit->holes_begin(); hi != fit->holes_end(); ++hi) {
//                holes.push_back(polyline2r());
//                auto curr = *hi;
//                do {
//                    holes.back().push_back(from_cgal_point(curr->source()->point()));
//                } while (++curr != *hi);
//            }
//            res_poly += holes;
            res.push_back(res_poly);
        }
    }
    return res;
}
