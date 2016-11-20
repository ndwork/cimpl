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

#include <complex.h>
#include <stddef.h>


typedef struct {
  size_t h;  // height
  size_t w;  // width
  float* data;  // column major ordering
} cimpl_img;

typedef struct {
  size_t h;  // height
  size_t w;  // width
  float complex* data;
} cimpl_cmpImg;

typedef struct {
  size_t h;  // height
  size_t w;  // width
  size_t s;  // number of slices
  float* data;  // column major ordering
} cimpl_vol;

typedef struct {
  size_t h;  // height
  size_t w;  // width
  size_t s;  // number of slices
  float complex* data;
} cimpl_cmpVol;


void cimpl_absImg( cimpl_img const in, cimpl_img * const out );
void cimpl_absCmpImg( cimpl_cmpImg const in, cimpl_img * const out );
void cimpl_absCmpVol( cimpl_cmpVol const in, cimpl_vol * const out );
void cimpl_absVol( cimpl_vol const in, cimpl_vol * const out );
void cimpl_addCmpImgs( cimpl_cmpImg const img1, cimpl_cmpImg const img2,
  cimpl_cmpImg * const out );
void cimpl_addImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out );
void cimpl_addVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out );
void cimpl_addScalar2Img( float const scalar, cimpl_img const in, cimpl_img * const out );
void cimpl_addScalar2Vol( float const scalar, cimpl_vol const in, cimpl_vol * const out );
void cimpl_argCmpImg( cimpl_cmpImg const in, cimpl_img * const out );
void cimpl_argCmpVol( cimpl_cmpVol const in, cimpl_vol * const out );
void cimpl_ceilImg( cimpl_img const img, cimpl_img * const out );
void cimpl_ceilVol( cimpl_vol const vol, cimpl_vol * const out );
void cimpl_circShiftImg( cimpl_img const in, int hShift, int vShift, cimpl_img * const out );
void cimpl_circShiftVol( cimpl_vol const in, int hShift, int vShift, int sShift,
  cimpl_vol * const out );
void cimpl_concatCmpImgsH( cimpl_cmpImg const img1, cimpl_cmpImg const img2, cimpl_cmpImg * const out );
void cimpl_concatCmpImgsW( cimpl_cmpImg const img1, cimpl_cmpImg const img2, cimpl_cmpImg * const out );
void cimpl_concatImgsH( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out );
void cimpl_concatImgsW( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out );
void cimpl_concatVolsH( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out );
void cimpl_concatVolsS( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out );
void cimpl_concatVolsW( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out );
void cimpl_concatCmpVolsH( cimpl_cmpVol const vol1, cimpl_cmpVol const vol2, cimpl_cmpVol * const out );
void cimpl_concatCmpVolsS( cimpl_cmpVol const vol1, cimpl_cmpVol const vol2, cimpl_cmpVol * const out );
void cimpl_concatCmpVolsW( cimpl_cmpVol const vol1, cimpl_cmpVol const vol2, cimpl_cmpVol * const out );
void cimpl_conjCmpImg( cimpl_cmpImg const in, cimpl_cmpImg * const out );
void cimpl_cropImg( cimpl_img const in, cimpl_img * const out );
void cimpl_cropVol( cimpl_vol const in, cimpl_vol * const out );
void cimpl_divideImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out );
void cimpl_divideVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out );
void cimpl_divideImgByScalar( cimpl_img const in, float const scalar, cimpl_img * const out );
float cimpl_dotImgs( cimpl_img const img1, cimpl_img const img2 );
int cimpl_equalImgs( cimpl_img const img1, cimpl_img const img2 );
int cimpl_equalVols( cimpl_vol const vol1, cimpl_vol const vol2 );
void cimpl_flipImgLR( cimpl_img const in, cimpl_img * const out );
void cimpl_flipImgUD( cimpl_img const in, cimpl_img * const out );
void cimpl_floorImg( cimpl_img const img, cimpl_img * const out );
void cimpl_floorVol( cimpl_vol const vol, cimpl_vol * const out );
void cimpl_freeCmpImg( cimpl_cmpImg * const in );
void cimpl_freeImg( cimpl_img * const in );
void cimpl_freeVol( cimpl_vol * const in );
void cimpl_getImagImg( cimpl_cmpImg const in, cimpl_img * const out );
void cimpl_getRealImg( cimpl_cmpImg const in, cimpl_img * const out );
void cimpl_getImagVol( cimpl_cmpVol const in, cimpl_vol * const out );
void cimpl_getRealVol( cimpl_cmpVol const in, cimpl_vol * const out );
float cimpl_linInterp( size_t const N, float const * const x, float const * const y,
  float const outOfBounds, float const q );
void cimpl_linInterps( size_t const N, float const * const x, float const * const y,
  float const outOfBounds, size_t const M, float const * const q, float * const out );
void cimpl_linInterpImg( cimpl_img const img, size_t const N, float const * const xq,
  float const * const yq, float const outOfBounds, float * const out );
cimpl_cmpImg cimpl_mallocCmpImg( size_t const h, size_t const w );
cimpl_img cimpl_mallocImg( size_t const h, size_t const w );
cimpl_vol cimpl_mallocVol( size_t const h, size_t const w, size_t s );
void cimpl_multiplyImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out );
void cimpl_multiplyVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out );
void cimpl_multiplyImgByScalar( cimpl_img const in, float const scalar, cimpl_img * const out );
void cimpl_multiplyVolByScalar( cimpl_vol const in, float const scalar, cimpl_vol * const out );
void cimpl_printImg( cimpl_img const in );
void cimpl_reshapeCmpImg( size_t H, size_t W, cimpl_cmpImg * const img );
void cimpl_reshapeImg( size_t H, size_t W, cimpl_img * const out );
void cimpl_reshapeVol( size_t H, size_t W, size_t S, cimpl_vol * const out );
//void cimpl_rot( cimpl_img const in, float const angle, cimpl_img * const out );
void cimpl_rot90( cimpl_img const in, cimpl_img * const out );
void cimpl_rot180( cimpl_img const in, cimpl_img * const out );
void cimpl_rot270( cimpl_img const in, cimpl_img * const out );
void cimpl_roundImg( cimpl_img const img, cimpl_img * const out );
void cimpl_roundVol( cimpl_vol const vol, cimpl_vol * const out );
void cimpl_sliceX( cimpl_vol const in, size_t xIndx, cimpl_img * const out );
void cimpl_sliceXZ( cimpl_vol const in, size_t xIndx, size_t zIndx, float * const out );
void cimpl_sliceY( cimpl_vol const in, size_t yIndx, cimpl_img * const out );
void cimpl_sliceYX( cimpl_vol const in, size_t yIndx, size_t xIndx, float * const out );
void cimpl_sliceYZ( cimpl_vol const in, size_t yIndx, size_t zIndx, float * const out );
void cimpl_sliceZ( cimpl_vol const in, size_t zIndx, cimpl_img * const out );
void cimpl_spaceConvImgTemplate( cimpl_img const img1, cimpl_img const t,
  cimpl_img * const out );
void cimpl_sqrtImg( cimpl_img const in, cimpl_img * const out );
void cimpl_sqrtVol( cimpl_vol const in, cimpl_vol * const out );
void cimpl_subImg( cimpl_img const in, size_t const h1, size_t const v1,
  cimpl_img * const out );
void cimpl_subtractCmpImgs( cimpl_cmpImg const img1, cimpl_cmpImg const img2,
  cimpl_cmpImg * const out );
void cimpl_subtractImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out );
void cimpl_subtractVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out );
void cimpl_subtractImgFromScalar( cimpl_img const in, float const scalar, cimpl_img * const out );
void cimpl_subtractScalarFromImg( cimpl_img const in, float const scalar, cimpl_img * const out );
void cimpl_subtractScalarFromVol( cimpl_vol const in, float const scalar, cimpl_vol * const out );
float cimpl_sumImg( cimpl_img const * const in );
float cimpl_sumVol( cimpl_vol const * const in );
void cimpl_zeroCmpImg( cimpl_cmpImg * const img  );
void cimpl_zeroImg( cimpl_img * const in  );
void cimpl_zeroVol( cimpl_vol * const in  );


#ifdef __cplusplus
}
#endif

#endif /* cimpl_h */
