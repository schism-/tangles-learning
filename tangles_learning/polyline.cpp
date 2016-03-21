//
//  polyline.cpp
//  tangles_learning
//
//  Created by Christian on 15/02/16.
//  Copyright © 2016 csantoni. All rights reserved.
//

#include "polyline.h"

template<typename T>
polyline2r make_polyline(const polyline2r& curve, const T& f) {
    auto nc = polyline2r(curve.size());
    for(auto i : range(curve)) nc[i] = f(curve[i]);
    return nc;
}

template<typename A, typename T>
polyline2r make_polyline(const vector<A>& points, const T& f) {
    auto nc = polyline2r(points.size());
    for(auto i : range(points)) nc[i] = f(points[i]);
    return nc;
}

polyline2r operator+(const polyline2r& a, const vec2r& b) { auto c = a; c.push_back(b); return c; }

polyline2r operator+(const polyline2r& a, const polyline2r& b) { auto c = a; for(auto&& p : b) c.push_back(p); return c; }

polyline2r transform_polyline(const frame2r& frame, const polyline2r& curve) {
    return make_polyline(curve,[&](const vec2r& p){ return transform_point(frame, p); });
}

polyline2r transform_polyline_inverse(const frame2r& frame, const polyline2r& curve) {
    return make_polyline(curve,[&](const vec2r& p){ return transform_point_inverse(frame, p); });
}

vector<polyline2r> transform_polylines(const frame2r& frame, const vector<polyline2r>& curves) {
    return make_vector(curves, [&](const polyline2r& curve){ return transform_polyline(frame,curve); });
}

vector<polyline2r> transform_polylines_inverse(const frame2r& frame, const vector<polyline2r>& curves) {
    return make_vector(curves, [&](const polyline2r& curve){ return transform_polyline_inverse(frame,curve); });
}

bool closed_polyline(const polyline2r& curve) {
    return curve.front() == curve.back();
}

void print_polyline(const polyline2r& curve) {
    for(auto i : range((int)curve.size())) printf("(%f, %f) -> ", curve[i].x, curve[i].y);
    printf("\n");
}

polyline2r reverse_polyline(const polyline2r& curve) {
    auto ret = curve; std::reverse(ret.begin(), ret.end()); return ret;
}

polyline2r close_polyline(const polyline2r& curve) {
    if(closed_polyline(curve)) return curve;
    else return curve + curve[0];
}

polyline2r remove_doubles_polyline(const polyline2r& curve){
    auto res = polyline2r();
    for(auto i : range((int)curve.size()-1)) if (curve[i] != curve[i+1]) res.push_back(curve[i]);
    res.push_back(curve[(int)curve.size() -1]);
    return res;
}

real length_polyline(const polyline2r& curve) {
    auto l = 0.0f;
    for(auto i : range((int)curve.size()-1)) l += length(curve[i]-curve[i+1]);
    return l;
}

range2r bounds_polyline(const polyline2r& curve) { return make_range2r(curve); }

range2r bounds_polylines(const vector<polyline2r>& curves) {
    auto bbox = range2r();
    for(auto& curve : curves) bbox = runion(bbox, bounds_polyline(curve));
    return bbox;
}

// uses even-odd rule
bool inside_polyline(const polyline2r& curve, const vec2r& p) {
    return inside_polylines({curve}, p);
}

polyline2r make_polyline_segment(const vec2r& a, const vec2r& b, real dist, bool skip_last) {
    auto steps = round(length(a-b) / dist);
    auto curve = polyline2r();
    for(auto i : range(steps)) {
        auto t = real(i) / real(steps);
        curve += a*(1-t)+b*t;
    }
    if(not skip_last) curve += b;
    return curve;
}

polyline2r make_polyline_rect(const vec2r& a, const vec2r& b) {
    return { {
        {min(a.x,b.x),min(a.y,b.y)},
        {max(a.x,b.x),min(a.y,b.y)},
        {max(a.x,b.x),max(a.y,b.y)},
        {min(a.x,b.x),max(a.y,b.y)},
        {min(a.x,b.x),min(a.y,b.y)},
    } };
}

polyline2r make_polyline_rect(const vec2r& a, const vec2r& b, real dist) {
    auto points = make_polyline_rect(a,b);
    return
    make_polyline_segment(points[0],points[1],dist,true) +
    make_polyline_segment(points[1],points[2],dist,true) +
    make_polyline_segment(points[2],points[3],dist,true) +
    make_polyline_segment(points[3],points[4],dist,false);
}

polyline2r make_polyline_circle(const vec2r& c, real radius, real dist) {
    auto steps = round(2*pif*radius / dist);
    auto curve = polyline2r();
    for(auto i : range(steps)) {
        auto theta = 2 * pif * real(i) / real(steps);
        curve += c + radius * vec2r{cos(theta),sin(theta)};
    }
    curve = close_polyline(curve);
    return curve;
}

bool inside_polylines(const vector<polyline2r>& curves, const vec2r& p) {
    auto flips = false;
    for(auto&& curve : curves) {
        for(auto i : range((int)curve.size()-1)) {
            if ( ((curve[i].y>p.y) != (curve[i+1].y>p.y)) &&
                (p.x < (curve[i].x-curve[i+1].x) * (p.y-curve[i].y) /
                 (curve[i+1].y-curve[i].y) + curve[i].x) )
                flips = not flips;
        }
    }
    return flips;
}

inline polyline2r remove_duplicates_polyline(const polyline2r& curve) {
    if(curve.empty()) return curve;
    auto cleaned = polyline2r({curve.front()});
    for(auto&& p : curve) if(p != cleaned.back()) cleaned += p;
    return cleaned;
}

polyline2r smooth_polyline(const polyline2r& curve, int iterations) {
    if(iterations == 0 or curve.empty()) return curve;
    auto relaxed = polyline2r(curve.size(),zero2r);
    for(auto i : range(1,(int)curve.size()-1)) {
        relaxed[i] = curve[i-1]*0.25f + curve[i]*0.5f + curve[i+1]*0.25f;
    }
    if(closed_polyline(curve)) {
        relaxed[0] = curve[curve.size()-2]*0.25f + curve[0]*0.5f + curve[1]*0.25f;
        relaxed.back() = relaxed.front();
    } else {
        relaxed[0] = curve[0]*0.75f + curve[1]*0.25f;
        relaxed[relaxed.size()-1] = curve[relaxed.size()-1]*0.75f + curve[relaxed.size()-2]*0.25f;
    }
    return smooth_polyline(relaxed, iterations-1);
}

bool intersect_segments(const vec2r& p0, const vec2r& p1, const vec2r& p2, const vec2r& p3,
                              vec2r *st, vec2r* p) {
    auto s1 = p1 - p0;
    auto s2 = p3 - p2;
    
    auto den = -s2.x * s1.y + s1.x * s2.y;
    if(den == 0) return false;
    
    auto s = ( s2.x * (p0.y - p2.y) - s2.y * (p0.x - p2.x)) / den;
    auto t = (-s1.y * (p0.x - p2.x) + s1.x * (p0.y - p2.y)) / den;
    
    if (s < 0 || s > 1 || t < 0 || t > 1) return false;
    
    if(st) *st = vec2r{s,t};
    if(p) *p = p0*(1-s)+p1*s;
    return true;
}

vector<pair<vec2i,vec2r>> intersections_polyline(const polyline2r& curvea, const polyline2r& curveb) {
    auto ret = vector<pair<vec2i,vec2r>>();
    for(auto i : range((int)curvea.size()-1)) {
        for(auto j : range((int)curveb.size()-1)) {
            auto p = zero2r;
            if(intersect_segments(curvea[i],curvea[i+1],curveb[j],curveb[j+1],nullptr,&p)) ret += {{i,j},p};
        }
    }
    return ret;
}

vector<pair<int,vec2r>> intersections_polyline(const polyline2r& curve) {
    auto cc = closed_polyline(curve);
    auto ret = vector<pair<int,vec2r>>();
    for(auto i : range((int)curve.size()-2)) {
        for(auto j : range(i+2,(int)curve.size()-1)) {
            if(cc and i == 0 and j == curve.size()-2) continue;
            auto p = zero2r;
            if(intersect_segments(curve[i],curve[i+1],curve[j],curve[j+1],nullptr,&p)) {
                ret += {i,p};
                ret += {j,p};
            }
        }
    }
    return ret;
}

polyline2r subdivide_polyline(const polyline2r& curve, real dist) {
    auto subdivided = polyline2r();
    subdivided += curve[0];
    for(auto i : range((int)curve.size()-1)) {
        auto l = length(curve[i]-curve[i+1]);
        auto steps = (int)round(l/dist);
        if(steps >= 2) {
            for(auto j = 1; j < steps; j ++) {
                auto t = j / (real)steps;
                subdivided += curve[i]*(1-t) + curve[i+1]*t;
            }
        }
        subdivided += curve[i+1];
    }
    return subdivided;
}

vector<polyline2r> split_polyline(const polyline2r& curve, const vector<pair<int,vec2r>>& intersections) {
    auto curves = vector<polyline2r>();
    curves += {curve[0]};
    for(auto i : range((int)curve.size()-1)) {
        for(auto&& intersection : intersections) {
            if(i == intersection.first) {
                // auto p = curve[i]*(1-intersection.second.x)+curve[i+1]*intersection.second.x;
                curves.back() += intersection.second;
                curves += {intersection.second};
            }
        }
        curves.back() += curve[i+1];
    }
    return curves;
}

polyline2r cleanup_stroke(const polyline2r& stroke, bool closed) {
    if(stroke.size() <= 1) return stroke;
    // remove duplicates
    auto cleaned = remove_duplicates_polyline(stroke);
    // close
    if(closed) cleaned = close_polyline(cleaned);
//    // simplify line
//    cleaned = simplify_polyline(cleaned, POLY_RES);
    // resample
    cleaned = subdivide_polyline(cleaned, POLY_RES);
    // smooth
    cleaned = smooth_polyline(cleaned, 10);
    // remove self intersections
    auto inters = intersections_polyline(cleaned);
    if(not inters.empty()) {
        auto splitted = split_polyline(cleaned, inters);
        auto max_l = length_polyline(splitted[0]); auto max_split = &splitted[0];
        for(auto&& split : splitted) {
            auto l = length_polyline(split);
            if(l > max_l) { max_split = &split; max_l = l; }
        }
        *max_split = close_polyline(*max_split);
        cleaned = *max_split;
        if(closed) cleaned = close_polyline(cleaned);
    }
    return cleaned;
}

#define NANOSVG_ALL_COLOR_KEYWORDS	// Include full list of color keywords.
#define NANOSVG_IMPLEMENTATION	// Expands implementation
#include "external/nanosvg/nanosvg.h"

polyline2r sample_bezier_polyline(const vector<vec2r>& cp, int steps) {
    if (steps == 0) return polyline2r();
    auto curve = polyline2r(steps+1);
    curve[0] = cp[0];
    for(auto i : range(1,steps)) {
        auto t = i / (real)steps;
        auto it = 1-t;
        curve[i] = it*it*it*cp[0] + 3*it*it*t*cp[1] + 3*it*t*t*cp[2] + t*t*t*cp[3];
    }
    curve[steps] = cp[3];
    return curve;
}

vector<polyline2r> parse_svg_polylines(const string& svg, real resolution) {
    auto buffer = new char[svg.size()+1]; strcpy(buffer, svg.c_str());
    auto image = nsvgParse(buffer, "px", 96);
    delete [] buffer;
    auto curves = vector<polyline2r>();
    for (auto shape = image->shapes; shape != nullptr; shape = shape->next) {
        for (auto path = shape->paths; path != nullptr; path = path->next) {
            curves += polyline2r();
            auto cp = vector<vec2r>();
            for(auto i : range(path->npts)) cp += vec2r(path->pts[i*2],path->pts[i*2+1]);
            for (auto i = 0; i < cp.size()-1; i += 3) {
                if(cp[i+0] == cp[i+1] and cp[i+1] == cp[i+2] and cp[i+2] == cp[i+3]) continue;
                auto len = length_polyline(sample_bezier_polyline({cp[i+0],cp[i+1],cp[i+2],cp[i+3]}, 100));
                curves.back() += sample_bezier_polyline({cp[i+0],cp[i+1],cp[i+2],cp[i+3]}, (int)round(len / resolution));
            }
            curves.back() = remove_duplicates_polyline(curves.back());
            if(path->closed) curves.back() = close_polyline(curves.back());
        }
    }
    nsvgDelete(image);
    auto final_curves = vector<polyline2r>();
    for (auto c : curves) { if (not c.empty()) final_curves.push_back(c); }
    return final_curves;
}