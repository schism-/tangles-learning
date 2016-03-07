//  cgal_arr.cpp
//  tangles_learning
//
//  Created by Christian on 22/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#include "cgal_arr.h"

void print_ccb(const Arr_with_hist_2::Ccb_halfedge_const_circulator& circ){
    std::cout << "(" << circ->source()->point() << ")";
    auto curr = circ;
    do {
        auto he = curr;
        std::cout << " --> " << "(" << he->target()->point() << ")";
    } while (++curr != circ);
    std::cout << std::endl;
}

void print_face(const Arr_with_hist_2& arr, const Arr_with_hist_2::Face_const_handle& f){
    if(f->is_unbounded())
        std::cout << "Unbounded face. " << std::endl;
    else{
        std::cout<<"Outer boundary: ";
        print_ccb(f->outer_ccb());
    }
    
    int index=1;
    Arr_with_hist_2::Hole_const_iterator hole;
    for(hole = f->holes_begin(); hole != f->holes_end(); ++hole, ++index){
        std::cout << "    Hole #" << index << ": ";
        print_ccb(*hole);
    }
    
    Arr_with_hist_2::Isolated_vertex_const_iterator iv;
    for(iv = f->isolated_vertices_begin(), index=1; iv != f->isolated_vertices_end(); ++iv,++index)
        std::cout << "    Isolated vertex #" << index << ": " << "(" << iv->point() << ")" << std::endl;
}

void print_arrangement(const Arr_with_hist_2& arr, bool verbose){
    CGAL_precondition(arr.is_valid());
    
    Arr_with_hist_2::Vertex_const_iterator vit;
    std::cout<<arr.number_of_vertices()<< " vertices:"<<std::endl;
    if (verbose){
        for (vit=arr.vertices_begin(); vit!=arr.vertices_end();++vit){
            std::cout << " (" << vit->point() << ") ";
            if (vit->is_isolated()) std::cout << " − Isolated. " << std::endl;
            else std::cout << " − degree " << vit->degree()<< std::endl;
        }
    }
    std::cout << std::endl;
    
    Arr_with_hist_2::Edge_const_iterator eit;
    std::cout<<arr.number_of_edges() << " edges:" << std::endl;
    if (verbose){
        for(eit=arr.edges_begin(); eit!=arr.edges_end(); ++eit)
            std::cout << "(" << eit->source()->point() << " -> " << eit->target()->point() << ")" << std::endl;
    }
    std::cout << std::endl;
    
    Arr_with_hist_2::Face_const_iterator fit;
    std::cout << arr.number_of_faces() << " faces:" << std::endl;
    if(verbose) for(fit=arr.faces_begin(); fit!=arr.faces_end(); ++fit) print_face(arr, fit);
    std::cout << std::endl;
    
    std::cout << arr.number_of_curves() << " curves:" << std::endl;
    std::cout << std::endl;
}

Point_2 to_cgal_point(const vec2r& point){
    return Point_2((real)point.x, -(real)point.y);
}

vector<Segment_2> to_cgal_arr_segments(const polyline2r& curve) {
    auto points = make_vector(curve, to_cgal_point);
    auto segments = vector<Segment_2>();
    for(auto i : range((int)curve.size()-1)) {
        if(points[i] == points[i+1]) {
            message("bad \n");
            continue;
        }
        segments += Segment_2(points[i],points[i+1]);
    }
    return segments;
}

vector<Segment_2> to_cgal_curve(const polyline2r& polyline){
    return to_cgal_arr_segments(polyline);
}

vec2r from_cgal_point(const Point_2& point){
    return vec2r((real)(CGAL::to_double(point.x())), -(real)(CGAL::to_double(point.y())));
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