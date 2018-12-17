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
    //SHARED_EXPORT void triangulate(unsigned long long num_vertices, double *vertices, int *ptr_num_faces, int **ptr_faces, void *triangulation_void_p)
    SHARED_EXPORT void triangulate(double *vertices, size_t num_vertices, size_t **ptr_faces, size_t *ptr_num_faces, void *triangulation_void_p)
    {
        std::vector<double> coords;
        
        //TODO can the vector be constructed directly from the pointer?
        for (size_t i = 0; i < num_vertices; i++)
        {
            coords.push_back(vertices[2*i + 0]);
            coords.push_back(vertices[2*i + 1]);
        }
        
        // Actually perform triangulation
        delaunator::Delaunator *triangulation = new delaunator::Delaunator(coords);
        
        size_t num_faces = triangulation->triangles.size() / 3;

        *ptr_faces = triangulation->triangles.data();
        *ptr_num_faces = num_faces;
        triangulation_void_p = (void *)triangulation;
    }
    
    SHARED_EXPORT void free_triangulation(void *triangulation_void_p)
    {
        delete (delaunator::Delaunator *)triangulation_void_p;
        triangulation_void_p = NULL;
    }
}
