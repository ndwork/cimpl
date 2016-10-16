//
//  cimpl.h
//  cimpl
//
//  Created by Nicholas Dwork on 9/18/16.
//  Copyright © 2016 Nicholas Dwork. All rights reserved.
//

#ifndef cimpl_h
#define cimpl_h

struct cimpl_imgf {
  unsigned int w;  // width
  unsigned int h;  // height
  float* data;  // Column major ordering
};

void cimpl_add( struct cimpl_imgf const img1, struct cimpl_imgf const img2, struct cimpl_imgf * const out );
void cimpl_display( struct cimpl_imgf const in );
void cimpl_divide( struct cimpl_imgf const img1, struct cimpl_imgf const img2, struct cimpl_imgf * const out );
struct cimpl_imgf cimpl_extractSubImg( struct cimpl_imgf const img1, unsigned int const h1, unsigned int const h2, unsigned int const v1, unsigned int const v2 );
void cimpl_freeImg( struct cimpl_imgf *out );
struct cimpl_imgf cimpl_mallocImg( unsigned int const w, unsigned int const h );
void cimpl_multiply( struct cimpl_imgf const img1, struct cimpl_imgf const img2, struct cimpl_imgf * const out );
void cimpl_subtract( struct cimpl_imgf const img1, struct cimpl_imgf const img2, struct cimpl_imgf * const out );
float cimpl_sum( struct cimpl_imgf const * const in );



#endif /* cimpl_h */
