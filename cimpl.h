//
//  cimpl.h
//  cimpl
//
//  Created by Nicholas Dwork starting on 9/18/16.
//  Copyright © 2016 Nicholas Dwork.
//

#ifndef cimpl_h
#define cimpl_h

typedef struct {
  unsigned int w;  // width
  unsigned int h;  // height
  float* data;  // column major ordering
} cimpl_imgf;

typedef struct {
  unsigned int w;  // width
  unsigned int h;  // height
  unsigned int s;  // number of slices
  float* data;  // column major ordering
} cimpl_volf;

void cimpl_absImg( cimpl_imgf const in, cimpl_imgf * const out );
void cimpl_absVol( cimpl_volf const in, cimpl_volf * const out );
void cimpl_addImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out );
void cimpl_addVols( cimpl_volf const vol1, cimpl_volf const vol2, cimpl_volf * const out );
void cimpl_addScalar2Img( float const scalar, cimpl_imgf const in, cimpl_imgf * const out );
void cimpl_addScalar2Vol( float const scalar, cimpl_volf const in, cimpl_volf * const out );
void cimpl_circShiftImg( cimpl_imgf const in, int hShift, int vShift, cimpl_imgf * const out );
void cimpl_circShiftVol( cimpl_volf const in, int hShift, int vShift, int sShift,
  cimpl_volf * const out );
void cimpl_cropImg( cimpl_imgf const in, cimpl_imgf * const out );
void cimpl_cropVol( cimpl_volf const in, cimpl_volf * const out );
void cimpl_displayImg( cimpl_imgf const in );
void cimpl_divideImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out );
void cimpl_divideImgByScalar( cimpl_imgf const in, float const scalar, cimpl_imgf * const out );
float cimpl_dotImgs( cimpl_imgf const img1, cimpl_imgf const img2 );
int cimpl_equalImgs( cimpl_imgf const img1, cimpl_imgf const img2 );
int cimpl_equalVols( cimpl_volf const vol1, cimpl_volf const vol2 );
void cimpl_freeImg( cimpl_imgf *out );
float cimpl_linInterp( unsigned int const N, float const * const x, float const * const y,
  float const outOfBounds, float const q );
void cimpl_linInterps( unsigned int const N, float const * const x, float const * const y,
  float const outOfBounds, unsigned int const M, float const * const q, float * const out );
void cimpl_linInterpImg( cimpl_imgf const img, unsigned int const N, float const * const xq,
  float const * const yq, float const outOfBounds, float * const out );
cimpl_imgf cimpl_mallocImg( unsigned int const h, unsigned int const w );
void cimpl_multiplyImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out );
void cimpl_multiplyImgByScalar( cimpl_imgf const in, float const scalar, cimpl_imgf * const out );
void cimpl_reshapeImg( unsigned int H, unsigned int W, cimpl_imgf * const out );
void cimpl_reshapeVol( unsigned int H, unsigned int W, unsigned int S, cimpl_volf * const out );
void cimpl_subImg( cimpl_imgf const in, unsigned int const h1, unsigned int const v1,
  cimpl_imgf * const out );
void cimpl_subtractImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out );
void cimpl_subtractScalarFromImg( cimpl_imgf const in, float const scalar, cimpl_imgf * const out );
float cimpl_sumImg( cimpl_imgf const * const in );


#endif /* cimpl_h */
