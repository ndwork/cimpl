//
//  cimpl.h
//  cimpl
//
//  Created by Nicholas Dwork starting on 9/18/16.
//  Copyright © 2016 Nicholas Dwork.
//

#ifndef cimpl_h
#define cimpl_h

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  unsigned int h;  // height
  unsigned int w;  // width
  float* data;  // column major ordering
} cimpl_imgf;

typedef struct {
  unsigned int h;  // height
  unsigned int w;  // width
  float* rData;  // real
  float* iData;  // imaginary
} cimpl_cmpImgf;

typedef struct {
  unsigned int h;  // height
  unsigned int w;  // width
  unsigned int s;  // number of slices
  float* data;  // column major ordering
} cimpl_volf;

void cimpl_absImg( cimpl_imgf const in, cimpl_imgf * const out );
void cimpl_absCmpImg( cimpl_cmpImgf const in, cimpl_imgf * const out );
void cimpl_absVol( cimpl_volf const in, cimpl_volf * const out );
void cimpl_addCmpImgs( cimpl_cmpImgf const img1, cimpl_cmpImgf const img2,
  cimpl_cmpImgf * const out );
void cimpl_addImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out );
void cimpl_addVols( cimpl_volf const vol1, cimpl_volf const vol2, cimpl_volf * const out );
void cimpl_addScalar2Img( float const scalar, cimpl_imgf const in, cimpl_imgf * const out );
void cimpl_addScalar2Vol( float const scalar, cimpl_volf const in, cimpl_volf * const out );
void cimpl_circShiftImg( cimpl_imgf const in, int hShift, int vShift, cimpl_imgf * const out );
void cimpl_circShiftVol( cimpl_volf const in, int hShift, int vShift, int sShift,
  cimpl_volf * const out );
void cimpl_conjCmpImg( cimpl_cmpImgf const in, cimpl_cmpImgf * const out );
void cimpl_cropImg( cimpl_imgf const in, cimpl_imgf * const out );
void cimpl_cropVol( cimpl_volf const in, cimpl_volf * const out );
void cimpl_divideImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out );
void cimpl_divideImgByScalar( cimpl_imgf const in, float const scalar, cimpl_imgf * const out );
float cimpl_dotImgs( cimpl_imgf const img1, cimpl_imgf const img2 );
int cimpl_equalImgs( cimpl_imgf const img1, cimpl_imgf const img2 );
int cimpl_equalVols( cimpl_volf const vol1, cimpl_volf const vol2 );
void cimpl_flipImgLR( cimpl_imgf const in, cimpl_imgf * const out );
void cimpl_flipImgUD( cimpl_imgf const in, cimpl_imgf * const out );
void cimpl_freeCmpImg( cimpl_cmpImgf * const in );
void cimpl_freeImg( cimpl_imgf * const in );
void cimpl_freeVol( cimpl_volf * const in );
float cimpl_linInterp( unsigned int const N, float const * const x, float const * const y,
  float const outOfBounds, float const q );
void cimpl_linInterps( unsigned int const N, float const * const x, float const * const y,
  float const outOfBounds, unsigned int const M, float const * const q, float * const out );
void cimpl_linInterpImg( cimpl_imgf const img, unsigned int const N, float const * const xq,
  float const * const yq, float const outOfBounds, float * const out );
cimpl_cmpImgf cimpl_mallocCmpImg( unsigned int const h, unsigned int const w );
cimpl_imgf cimpl_mallocImg( unsigned int const h, unsigned int const w );
cimpl_volf cimpl_mallocVol( unsigned int const h, unsigned int const w, unsigned int s );
void cimpl_multiplyImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out );
void cimpl_multiplyImgByScalar( cimpl_imgf const in, float const scalar, cimpl_imgf * const out );
void cimpl_printImg( cimpl_imgf const in );
void cimpl_reshapeCmpImg( unsigned int H, unsigned int W, cimpl_cmpImgf * const img );
void cimpl_reshapeImg( unsigned int H, unsigned int W, cimpl_imgf * const out );
void cimpl_reshapeVol( unsigned int H, unsigned int W, unsigned int S, cimpl_volf * const out );
//void cimpl_rot( cimpl_imgf const in, float const angle, cimpl_imgf * const out );
void cimpl_rot90( cimpl_imgf const in, cimpl_imgf * const out );
void cimpl_rot180( cimpl_imgf const in, cimpl_imgf * const out );
void cimpl_rot270( cimpl_imgf const in, cimpl_imgf * const out );
void cimpl_sliceX( cimpl_volf const in, unsigned int xIndx, cimpl_imgf * const out );
void cimpl_sliceXZ( cimpl_volf const in, unsigned int xIndx, unsigned int zIndx, float * const out );
void cimpl_sliceY( cimpl_volf const in, unsigned int yIndx, cimpl_imgf * const out );
void cimpl_sliceYX( cimpl_volf const in, unsigned int yIndx, unsigned int xIndx, float * const out );
void cimpl_sliceYZ( cimpl_volf const in, unsigned int yIndx, unsigned int zIndx, float * const out );
void cimpl_sliceZ( cimpl_volf const in, unsigned int zIndx, cimpl_imgf * const out );
void cimpl_subImg( cimpl_imgf const in, unsigned int const h1, unsigned int const v1,
  cimpl_imgf * const out );
void cimpl_subtractImgs( cimpl_imgf const img1, cimpl_imgf const img2, cimpl_imgf * const out );
void cimpl_subtractImgFromScalar( cimpl_imgf const in, float const scalar, cimpl_imgf * const out );
void cimpl_subtractScalarFromImg( cimpl_imgf const in, float const scalar, cimpl_imgf * const out );
float cimpl_sumImg( cimpl_imgf const * const in );
float cimpl_sumVol( cimpl_volf const * const in );


#ifdef __cplusplus
}
#endif

#endif /* cimpl_h */
