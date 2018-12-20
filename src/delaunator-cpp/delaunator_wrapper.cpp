/* 
 * Copyright (c) 2018, Danish Agency for Data Supply and Efficiency <sdfe@sdfe.dk>
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "delaunator.hpp"
#include <stdlib.h> // for malloc/free

#ifdef _WIN32
    #define SHARED_EXPORT __declspec(dllexport)
#else
    #define SHARED_EXPORT
#endif

extern "C" {
    SHARED_EXPORT void triangulate(double *vertices, ssize_t num_vertices, ssize_t **ptr_faces, ssize_t *ptr_num_faces, void *triangulation_void_p)
    {
        // The Delaunator instance needs input coordinates in a vector. Build
        // this from our C-style array using a range constructor.
        const std::vector<double> coords(vertices, vertices + 2*num_vertices);
        
        // Actually perform triangulation.
        delaunator::Delaunator *triangulation = new delaunator::Delaunator(coords);
        
        ssize_t num_faces = triangulation->triangles.size() / 3;

        // Cast from size_t to ssize_t to allow sign (needed for nodata-ish
        // values).
        *ptr_faces = (ssize_t *)triangulation->triangles.data();
        *ptr_num_faces = num_faces;

        // Also hang on to our Delaunator object so we can free its memory
        // correctly.
        triangulation_void_p = (void *)triangulation;
    }
    
    SHARED_EXPORT void free_triangulation(void *triangulation_void_p)
    {
        delete (delaunator::Delaunator *)triangulation_void_p;
        triangulation_void_p = NULL;
    }
}
