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

void print_arrangement(const Arr_with_hist_2& arr){
    CGAL_precondition(arr.is_valid());
    
    Arr_with_hist_2::Vertex_const_iterator vit;
    std::cout<<arr.number_of_vertices()<< " vertices:"<<std::endl;
    for (vit=arr.vertices_begin(); vit!=arr.vertices_end();++vit){
        std::cout << " (" << vit->point() << ") ";
        if (vit->is_isolated()) std::cout << " − Isolated. " << std::endl;
        else std::cout << " − degree " << vit->degree()<< std::endl;
    }
    
    Arr_with_hist_2::Edge_const_iterator eit;
    std::cout<<arr.number_of_edges() << " edges:" << std::endl;
    for(eit=arr.edges_begin(); eit!=arr.edges_end(); ++eit)
        std::cout << "(" << eit->source()->point() << " -> " << eit->target()->point() << ")" << std::endl;
    
    Arr_with_hist_2::Face_const_iterator fit;
    std::cout << arr.number_of_faces() << " faces:" << std::endl;
    for(fit=arr.faces_begin(); fit!=arr.faces_end(); ++fit) print_face(arr, fit);
}

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
                pts.push_back(cgal_arr->from_cgal_point(cc->source()->point()));
            } while (++cc != fit->outer_ccb());
            res_poly.push_back(pts);
            for (auto hi = fit->holes_begin(); hi != fit->holes_end(); ++hi) {
                holes.push_back(polyline2r());
                auto curr = *hi;
                do {
                    holes.back().push_back(cgal_arr->from_cgal_point(curr->source()->point()));
                } while (++curr != *hi);
            }
            res_poly += holes;
            
            res.push_back(res_poly);
        }
    }
    return res;
}