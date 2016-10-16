//
//  cimpl.h
//  cimpl
//
//  Created by Nicholas Dwork on 9/18/16.
//  Copyright © 2016 Nicholas Dwork. All rights reserved.
//

#ifndef cimpl_h
#define cimpl_h

typedef struct {
  unsigned int w;  // width
  unsigned int h;  // height
  float* data;  // Column major ordering
} cimpl_imgf;

void cimpl_add( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out );
void cimpl_addScalar( cimpl_imgf const in, float const scalar, cimpl_imgf * const out );
void cimpl_display( cimpl_imgf const in );
void cimpl_divide( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out );
int cimpl_equal( cimpl_imgf const img1, cimpl_imgf const img2 );
cimpl_imgf cimpl_extractSubImg( cimpl_imgf const in, unsigned int const h1, unsigned int const h2, unsigned int const v1, unsigned int const v2 );
void cimpl_freeImg( cimpl_imgf *out );
cimpl_imgf cimpl_mallocImg( unsigned int const w, unsigned int const h );
void cimpl_multiply( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out );
void cimpl_multiplyScalar( cimpl_imgf const in, float const scalar, cimpl_imgf * const out );
void cimpl_subtract( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out );
float cimpl_sum( cimpl_imgf const * const in );



#endif /* cimpl_h */
