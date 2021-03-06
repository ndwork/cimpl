//
//  cimpl.c
//  cimpl
//
//  Created by Nicholas Dwork starting on 9/18/16.
//  Copyright © 2016 Nicholas Dwork.
//

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cimpl.h"

#ifndef CIMPL_DONT_SIMD
#include <xmmintrin.h>
#endif

#if !defined(CIMPL_DONT_SSE4PT1) || !defined(CIMPL_DONT_SIMD)
#include "smmintrin.h"
#endif

#if !defined(CIMPL_DONT_AVX) || !defined(CIMPL_DONT_SIMD)
#include "immintrin.h"
#endif


// ----- Static Functions -----

static inline void cimpl_memcpy( void * const dst, void * const src, size_t N ){
#ifndef CIMPL_DONT_SIMD
  float* dstPtr = (float*) dst;
  float* srcPtr = (float*) src;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = N / 8;

  for( size_t i=0; i<simdIters; ++i, srcPtr+=8, dstPtr+=8 )
    _mm256_store_ps( dstPtr, _mm256_load_ps( srcPtr ) );

  memcpy( dst+8*simdIters, src+8*simdIters, N-8*simdIters );

#else  // #ifndef CIMPL_DONT_AVX

  size_t simdIters = N / 4;

  for( size_t i=0; i<simdIters; ++i, srcPtr+=4, dstPtr+=4 )
    _mm_store_ps( dstPtr, _mm_load_ps( srcPtr ) );

  memcpy( dst+4*simdIters, src+4*simdIters, N-4*simdIters );

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD
  memcpy(dst, src, N);
#endif  // #ifndef CIMPL_DONT_SIMD
}



// ----- Public Functions -----

void cimpl_absCmpImg( cimpl_cmpImg const in, cimpl_img * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );
  for( size_t i=0; i<in.nPix; ++i )
    out->data[i] = cabsf( in.data[i] );
}

void cimpl_absCmpVol( cimpl_cmpVol const in, cimpl_vol * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );  assert( out->s == in.s );
  for( size_t i=0; i<in.nVox; ++i )
    out->data[i] = cabsf( in.data[i] );
}

void cimpl_absImg( cimpl_img const in, cimpl_img * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );
  for( size_t i=0; i<in.nPix; ++i )
    out->data[i] = fabsf(in.data[i]);
}

void cimpl_absVol( cimpl_vol const in, cimpl_vol * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );  assert( out->s == in.s );
  for( size_t i=0; i<in.nVox; ++i )
    out->data[i] = fabsf(in.data[i]);
}

void cimpl_addCmpImgs( cimpl_cmpImg const img1, cimpl_cmpImg const img2,
                      cimpl_cmpImg * const out ){
  assert( out->h == img1.h );  assert( out->w == img1.w );
  assert( out->h == img2.h );  assert( out->w == img2.w );
#ifndef CIMPL_DONT_SIMD
  size_t i;
  size_t simdIters = img1.nPix / 2;

  __m128* l = (__m128*) img1.data;
  __m128* r = (__m128*) img2.data;
  float* outData = (float*) out->data;

  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=4 )
    _mm_store_ps(outData, _mm_add_ps(*l, *r));

  for( i=0; i < img1.nPix-(2*simdIters); ++i )
    out->data[i+2*simdIters] = img1.data[i+2*simdIters] + img2.data[i+2*simdIters];
#else
  for( size_t i=0; i<img1.nPix; ++i )
    out->data[i] = img1.data[i] + img2.data[i];
#endif
}

void cimpl_addImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out ){
  assert( out->w == img1.w );  assert( out->h == img1.h );
  assert( out->w == img2.w );  assert( out->h == img2.h );
#ifndef CIMPL_DONT_SIMD
  size_t i;

#ifndef CIMPL_DONT_AVX

  unsigned long simdIters = img1.nPix / 8;

  __m256* l = (__m256*) img1.data;
  __m256* r = (__m256*) img2.data;
  float* outData = out->data;

  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=8 )
    _mm256_store_ps(outData, _mm256_add_ps(*l, *r));

  for( i=0; i < img1.nPix-(8*simdIters); ++i )
    out->data[i+8*simdIters] = img1.data[i+8*simdIters] + img2.data[i+8*simdIters];

#else

  unsigned long simdIters = (img1.h*img1.w) / 4;

  __m128* l = (__m128*) img1.data;
  __m128* r = (__m128*) img2.data;
  float* outData = out->data;

  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=4 )
    _mm_store_ps(outData, _mm_add_ps(*l, *r));

  for( i=0; i < img1.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = img1.data[i+4*simdIters] + img2.data[i+4*simdIters];
#endif  // #ifndef CIMPL_DONT_AVX

#else
  for( size_t i=0; i<img1.nPix; ++i)
    out->data[i] = img1.data[i] + img2.data[i];
#endif
}

void cimpl_addVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out ){
  assert( out->w == vol1.w );  assert( out->h == vol1.h );  assert( out->s == vol1.s );
  assert( out->w == vol2.w );  assert( out->h == vol2.h );  assert( out->s == vol2.s );
#ifndef CIMPL_DONT_SIMD
  size_t i;
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = vol1.nVox / 8;
  
  __m256* l = (__m256*) vol1.data;
  __m256* r = (__m256*) vol2.data;
  
  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=8 )
    _mm256_store_ps(outData, _mm256_add_ps(*l, *r));
  
  for( i=0; i < vol1.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = vol1.data[i+8*simdIters] + vol2.data[i+8*simdIters];

#else

  size_t simdIters = vol1.nVox / 4;

  __m128* l = (__m128*) vol1.data;
  __m128* r = (__m128*) vol2.data;

  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=4 )
    _mm_store_ps(outData, _mm_add_ps(*l, *r));

  for( i=0; i < vol1.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = vol1.data[i+4*simdIters] + vol2.data[i+4*simdIters];

#endif  // #ifndef CIMPL_DONT_AVX

#else
  for( int i=0; i<vol1.nVox; ++i)
    out->data[i] = vol1.data[i] + vol2.data[i];
#endif
}

void cimpl_addScalar2Img( float const scalar, cimpl_img const in, cimpl_img * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );

#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = in.nPix / 8;
  const __m256 s = _mm256_set1_ps(scalar);
  __m256* inData = (__m256*) in.data;

  for(size_t i = 0; i<simdIters; ++i, ++inData, outData += 8)
    _mm256_store_ps(outData, _mm256_add_ps(*inData, s));

  for( int i=0; i < in.nPix-(8*simdIters); ++i )
    out->data[i+8*simdIters] = in.data[i+8*simdIters] + scalar;

#else

  size_t simdIters = in.nPix / 4;
  const __m128 s = _mm_set1_ps(scalar);
  __m128* inData = (__m128*) in.data;

  for(size_t i = 0; i<simdIters; ++i, ++inData, outData += 4)
    _mm_store_ps(outData, _mm_add_ps(*inData, s));

  for( int i=0; i < in.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = in.data[i+4*simdIters] + scalar;

#endif  // #ifdef CIMPL_DONT_AVX

#else
  for( size_t i=0; i<in.nPix; ++i )
    out->data[i] = in.data[i] + scalar;
#endif
}

void cimpl_addScalar2Vol( float const scalar, cimpl_vol const in, cimpl_vol * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );  assert( out->s == in.s );

#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = in.nVox / 8;
  const __m256 s = _mm256_set1_ps( scalar );
  __m256* inData = (__m256*) in.data;

  for(size_t i = 0; i<simdIters; ++i, ++inData, outData += 8)
    _mm256_store_ps(outData, _mm256_add_ps(*inData, s));

  for( int i=0; i < in.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = in.data[i+8*simdIters] + scalar;

#else

  size_t simdIters = in.nVox / 4;
  const __m128 s = _mm_set1_ps(scalar);
  __m128* inData = (__m128*) in.data;

  for(size_t i = 0; i<simdIters; ++i, ++inData, outData += 4)
    _mm_store_ps(outData, _mm_add_ps(*inData, s));

  for( int i=0; i < in.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = in.data[i+4*simdIters] + scalar;

#endif  // #ifdef CIMPL_DONT_AVX

#else
  for( size_t i=0; i<in.nVox; ++i )
    out->data[i] = in.data[i] + scalar;
#endif
}

void cimpl_argCmpImg( cimpl_cmpImg const in, cimpl_img * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );
  for( size_t i=0; i<in.nPix; ++i )
    out->data[i] = cargf( in.data[i] );
}

void cimpl_argCmpVol( cimpl_cmpVol const in, cimpl_vol * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );  assert( out->s == in.s );
  for( size_t i=0; i<in.nVox; ++i )
    out->data[i] = cargf( in.data[i] );
}

void cimpl_ceilImg( cimpl_img const img, cimpl_img * const out ){
  assert( out->h == img.h );  assert( out->w == img.w );
#if !defined(CIMPL_DONT_SSE4PT1) || !defined(CIMPL_DONT_SIMD)
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = img.nPix / 8;
  __m256* ptr = (__m256*)img.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 8){
    _mm256_store_ps(outData, _mm256_ceil_ps( *ptr ) );
  }

  for( int i=0; i < img.nPix-(8*simdIters); ++i )
    out->data[i+8*simdIters] = ceilf( img.data[i+8*simdIters] );

#else

  size_t simdIters = img.nPix / 4;
  __m128* ptr = (__m128*)img.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 4){
    _mm_store_ps(outData, _mm_ceil_ps( *ptr ) );
  }

  for( int i=0; i < img.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = ceilf( img.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else
  for( size_t i=0; i<img.nPix; ++i )
    out->data[i] = ceilf( img.data[i] );
#endif
}

void cimpl_ceilVol( cimpl_vol const vol, cimpl_vol * const out ){
  assert( out->h == vol.h );  assert( out->w == vol.w );  assert( out->s == vol.s );
#if !defined(CIMPL_DONT_SSE4PT1) || !defined(CIMPL_DONT_SIMD)
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = vol.nVox / 8;
  __m256* ptr = (__m256*)vol.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 8)
    _mm256_store_ps(outData, _mm256_ceil_ps( *ptr ) );

  for( int i=0; i < vol.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = ceilf( vol.data[i+8*simdIters] );

#else

  size_t simdIters = vol.nVox / 4;
  __m128* ptr = (__m128*)vol.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 4)
    _mm_store_ps(outData, _mm_ceil_ps( *ptr ) );

  for( int i=0; i < vol.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = ceilf( vol.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else
  for( size_t i=0; i<vol.nPix; ++i )
    out->data[i] = ceilf( vol.data[i] );
#endif
}

void cimpl_circShiftImg( cimpl_img const in, int hShift, int vShift, cimpl_img * const out ){
  assert( out->w == in.w );  assert( out->h == in.h );
  assert( out->data != in.data );
  int inX, inY;

  for( int x=0; x<in.w; ++x ){
    inX = x+hShift;
    inX = inX % in.w;
    for( int y=0; y<in.h; ++y ){
      inY = y+vShift;
      inY = inY % in.h;

      out->data[y+x*in.h] = in.data[inY+inX*in.h];
  } }
}

void cimpl_circShiftVol( cimpl_vol const in, int hShift, int vShift, int sShift,
  cimpl_vol * const out ){
  assert( out->w == in.w );  assert( out->h == in.h );  assert( out->s == in.s );
  assert( out->data != in.data );
  int inX, inY, inZ;
  for( int x=0; x<in.w; ++x ){
    inX = x + hShift;
    inX = inX % in.w;
    for( int y=0; y<in.h; ++y ){
      inY = y + vShift;
      inY = inY % in.h;
      for( int z=0; z<in.s; ++z ){
        inZ = z + sShift;
        inZ = inZ % in.s;

        out->data[y+x*in.h+z*in.h*in.w] = in.data[inY+inX*in.h+inZ*in.h*in.w];
  } } }
}

void cimpl_cmpEqImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out ){
  assert( out->h == img1.h );   assert( out->w == img1.w );
  assert( out->h == img2.h );   assert( out->w == img2.w );

#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = img1.nPix / 8;
  __m256* l = (__m256*) img1.data;
  __m256* r = (__m256*) img2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 8)
    _mm256_store_ps(outData, _mm256_cmp_ps( *l, *r, 16 ) );

  for( int i=0; i < img1.nPix-(8*simdIters); ++i )
    out->data[i+8*simdIters] = (float)( img1.data[i+8*simdIters] == img2.data[i+8*simdIters] );

#else  // #ifndef CIMPL_DONT_AVX

  size_t simdIters = img1.nPix / 4;
  __m128* l = (__m128*)img1.data;
  __m128* r = (__m128*)img2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 4)
    _mm_store_ps( outData, _mm_cmpeq_ps( *l, *r ) );

  for( int i=0; i < img1.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = (float)( img1.data[i+4*simdIters] == img2.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else  //  #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<img1.nPix; ++i )
    out->data[i] = ( img1.data[i] == img2.data[i] ? 1 : 0 );

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_cmpEqVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out ){
  assert( out->h == vol1.h );   assert( out->w == vol1.w );  assert( out->s == vol1.s );
  assert( out->h == vol2.h );   assert( out->w == vol2.w );  assert( out->s == vol2.s );
  
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int)( vol1.nVox / 8 );
  __m256* l = (__m256*) vol1.data;
  __m256* r = (__m256*) vol2.data;

  for( size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 8 )
    _mm256_store_ps(outData, _mm256_cmp_ps( *l, *r, 16 ) );

  for( int i=0; i < vol1.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = (float)( vol1.data[i+8*simdIters] == vol2.data[i+8*simdIters] );

#else  // #ifndef CIMPL_DONT_AVX

  int simdIters = (int)( vol1.nVox / 4 );
  __m128* l = (__m128*) vol1.data;
  __m128* r = (__m128*) vol2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 4)
    _mm_store_ps( outData, _mm_cmpeq_ps( *l, *r ) );

  for( int i=0; i < vol1.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = (float)( vol1.data[i+4*simdIters] == vol2.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else  //  #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<vol1.nVox; ++i )
    out->data[i] = ( vol1.data[i] == vol2.data[i] ? 1 : 0 );

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_cmpGeImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out ){
  assert( out->h == img1.h );   assert( out->w == img1.w );
  assert( out->h == img2.h );   assert( out->w == img2.w );

#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int) ((img1.h*img1.w) / 8);
  __m256* l = (__m256*) img1.data;
  __m256* r = (__m256*) img2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 8)
    _mm256_store_ps(outData, _mm256_cmp_ps( *l, *r, 13 ) );

  for( int i=0; i < img1.nPix-(8*simdIters); ++i )
    out->data[i+8*simdIters] = (float)( img1.data[i+8*simdIters] >= img2.data[i+8*simdIters] );

#else  // #ifndef CIMPL_DONT_AVX

  size_t simdIters = img1.nPix / 4;
  __m128* l = (__m128*)img1.data;
  __m128* r = (__m128*)img2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 4)
    _mm_store_ps( outData, _mm_cmpge_ps( *l, *r ) );

  for( int i=0; i < img1.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = (float)( img1.data[i+4*simdIters] >= img2.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else  //  #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<img1.nPix; ++i )
    out->data[i] = ( img1.data[i] >= img2.data[i] ? 1 : 0 );

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_cmpGeVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out ){
  assert( out->h == vol1.h );   assert( out->w == vol1.w );  assert( out->s == vol1.s );
  assert( out->h == vol2.h );   assert( out->w == vol2.w );  assert( out->s == vol2.s );

#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int)( vol1.nVox / 8 );
  __m256* l = (__m256*) vol1.data;
  __m256* r = (__m256*) vol2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 8)
    _mm256_store_ps( outData, _mm256_cmp_ps( *l, *r, 13 ) );

  for( int i=0; i < vol1.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = (float)( vol1.data[i+8*simdIters] >= vol2.data[i+8*simdIters] );

#else  // #ifndef CIMPL_DONT_AVX

  int simdIters = (int)( vol1.nVox / 4 );
  __m128* l = (__m128*) vol1.data;
  __m128* r = (__m128*) vol2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 4)
    _mm_store_ps( outData, _mm_cmpge_ps( *l, *r ) );

  for( int i=0; i < vol1.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = (float)( vol1.data[i+4*simdIters] >= vol2.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else  //  #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<vol1.nVox; ++i )
    out->data[i] = ( vol1.data[i] >= vol2.data[i] ? 1 : 0 );

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_cmpGtImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out ){
  assert( out->h == img1.h );   assert( out->w == img1.w );
  assert( out->h == img2.h );   assert( out->w == img2.w );

#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = img1.nPix / 8;
  __m256* l = (__m256*) img1.data;
  __m256* r = (__m256*) img2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 8)
    _mm256_store_ps(outData, _mm256_cmp_ps( *l, *r, 14 ) );

  for( int i=0; i < img1.nPix-(8*simdIters); ++i )
    out->data[i+8*simdIters] = (float)( img1.data[i+8*simdIters] > img2.data[i+8*simdIters] );

#else  // #ifndef CIMPL_DONT_AVX

  size_t simdIters = img1.nPix / 4;
  __m128* l = (__m128*) img1.data;
  __m128* r = (__m128*) img2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 4)
    _mm_store_ps( outData, _mm_cmpgt_ps( *l, *r ) );

  for( int i=0; i < img1.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = (float)( img1.data[i+4*simdIters] > img2.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else  //  #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<img1.nPix; ++i )
    out->data[i] = ( img1.data[i] > img2.data[i] ? 1 : 0 );

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_cmpGtVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out ){
  assert( out->h == vol1.h );   assert( out->w == vol1.w );  assert( out->s == vol1.s );
  assert( out->h == vol2.h );   assert( out->w == vol2.w );  assert( out->s == vol1.s );

#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int)( vol1.nVox / 8 );
  __m256* l = (__m256*) vol1.data;
  __m256* r = (__m256*) vol2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 8)
    _mm256_store_ps( outData, _mm256_cmp_ps( *l, *r, 14 ) );

  for( int i=0; i < vol1.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = (float)( vol1.data[i+8*simdIters] > vol2.data[i+8*simdIters] );

#else  // #ifndef CIMPL_DONT_AVX

  int simdIters = (int)( vol1.nVox / 4 );
  __m128* l = (__m128*) vol1.data;
  __m128* r = (__m128*) vol2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 4)
    _mm_store_ps( outData, _mm_cmpgt_ps( *l, *r ) );

  for( int i=0; i < vol1.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = (float)( vol1.data[i+4*simdIters] > vol2.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else  //  #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<vol1.nVox; ++i )
    out->data[i] = ( vol1.data[i] > vol2.data[i] ? 1 : 0 );

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_cmpLeImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out ){
  assert( out->h == img1.h );   assert( out->w == img1.w );
  assert( out->h == img2.h );   assert( out->w == img2.w );

#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int) ((img1.h*img1.w) / 8);
  __m256* l = (__m256*) img1.data;
  __m256* r = (__m256*) img2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 8)
    _mm256_store_ps(outData, _mm256_cmp_ps( *l, *r, 2 ) );

  for( int i=0; i < img1.nPix-(8*simdIters); ++i )
    out->data[i+8*simdIters] = (float)( img1.data[i+8*simdIters] <= img2.data[i+8*simdIters] );

#else  // #ifndef CIMPL_DONT_AVX

  int simdIters = (int) ( img1.nPix / 4);
  __m128* l = (__m128*) img1.data;
  __m128* r = (__m128*) img2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 4)
    _mm_store_ps( outData, _mm_cmple_ps( *l, *r ) );

  for( int i=0; i < img1.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = (float)( img1.data[i+4*simdIters] <= img2.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else  //  #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<img1.nPix; ++i )
    out->data[i] = ( img1.data[i] <= img2.data[i] ? 1 : 0 );

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_cmpLeVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out ){
  assert( out->h == vol1.h );   assert( out->w == vol1.w );  assert( out->s == vol1.s );
  assert( out->h == vol2.h );   assert( out->w == vol2.w );  assert( out->s == vol2.s );

#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int)( vol1.nVox / 8 );
  __m256* l = (__m256*) vol1.data;
  __m256* r = (__m256*) vol2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 8)
    _mm256_store_ps(outData, _mm256_cmp_ps( *l, *r, 2 ) );

  for( int i=0; i < vol1.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = (float)( vol1.data[i+8*simdIters] <= vol2.data[i+8*simdIters] );
  
#else  // #ifndef CIMPL_DONT_AVX

  int simdIters = (int)( vol1.nVox / 4 );
  __m128* l = (__m128*) vol1.data;
  __m128* r = (__m128*) vol2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 4)
    _mm_store_ps( outData, _mm_cmple_ps( *l, *r ) );

  for( int i=0; i < vol1.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = (float)( vol1.data[i+4*simdIters] <= vol2.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else  //  #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<vol1.nVox; ++i )
    out->data[i] = ( vol1.data[i] <= vol2.data[i] ? 1 : 0 );

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_cmpLtImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out ){
  assert( out->h == img1.h );   assert( out->w == img1.w );
  assert( out->h == img2.h );   assert( out->w == img2.w );

#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = img1.nPix / 8;
  __m256* l = (__m256*) img1.data;
  __m256* r = (__m256*) img2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 8)
    _mm256_store_ps(outData, _mm256_cmp_ps( *l, *r, 1 ) );

  for( int i=0; i < img1.nPix-(8*simdIters); ++i )
    out->data[i+8*simdIters] = (float)( img1.data[i+8*simdIters] < img2.data[i+8*simdIters] );

#else  // #ifndef CIMPL_DONT_AVX

  size_t simdIters = img1.nPix / 4;
  __m128* l = (__m128*) img1.data;
  __m128* r = (__m128*) img2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 4)
    _mm_store_ps( outData, _mm_cmplt_ps( *l, *r ) );

  for( int i=0; i < img1.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = (float)( img1.data[i+4*simdIters] < img2.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX
  
#else  //  #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<img1.nPix; ++i )
    out->data[i] = ( img1.data[i] < img2.data[i] ? 1 : 0 );

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_cmpLtVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_img * const out ){
  assert( out->h == vol1.h );   assert( out->w == vol1.w );
  assert( out->h == vol2.h );   assert( out->w == vol2.w );
    
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int)( vol1.nVox / 8 );
  __m256* l = (__m256*) vol1.data;
  __m256* r = (__m256*) vol2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 8)
    _mm256_store_ps(outData, _mm256_cmp_ps( *l, *r, 1 ) );

  for( int i=0; i < vol1.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = (float)( vol1.data[i+8*simdIters] < vol2.data[i+8*simdIters] );

#else  // #ifndef CIMPL_DONT_AVX

  int simdIters = (int)( vol1.nVox / 4 );
  __m128* l = (__m128*) vol1.data;
  __m128* r = (__m128*) vol2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 4)
    _mm_store_ps( outData, _mm_cmplt_ps( *l, *r ) );

  for( int i=0; i < vol1.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = (float)( vol1.data[i+4*simdIters] < vol2.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else  //  #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<vol1.nVox; ++i )
    out->data[i] = ( vol1.data[i] < vol2.data[i] ? 1 : 0 );

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_cmpNeqImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out ){
  assert( out->h == img1.h );   assert( out->w == img1.w );
  assert( out->h == img2.h );   assert( out->w == img2.w );

#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = img1.nPix / 8;
  __m256* l = (__m256*) img1.data;
  __m256* r = (__m256*) img2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 8)
    _mm256_store_ps(outData, _mm256_cmp_ps( *l, *r, 28 ) );

  for( int i=0; i < img1.nPix-(8*simdIters); ++i )
    out->data[i+8*simdIters] = (float)( img1.data[i+8*simdIters] != img2.data[i+8*simdIters] );

#else  // #ifndef CIMPL_DONT_AVX

  size_t simdIters = img1.nPix / 4;
  __m128* l = (__m128*) img1.data;
  __m128* r = (__m128*) img2.data;

  for( size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 4 )
    _mm_store_ps( outData, _mm_cmpneq_ps( *l, *r ) );

  for( int i=0; i < img1.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = (float)( img1.data[i+4*simdIters] != img2.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else  //  #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<img1.nPix; ++i )
    out->data[i] = ( img1.data[i] != img2.data[i] ? 1 : 0 );

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_cmpNeqVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out ){
  assert( out->h == vol1.h );   assert( out->w == vol1.w );  assert( out->s == vol1.s );
  assert( out->h == vol2.h );   assert( out->w == vol2.w );  assert( out->s == vol2.s );

#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int)( vol1.nVox / 8 );
  __m256* l = (__m256*) vol1.data;
  __m256* r = (__m256*) vol2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 8)
    _mm256_store_ps(outData, _mm256_cmp_ps( *l, *r, 28 ) );

  for( int i=0; i < vol1.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = (float)( vol1.data[i+8*simdIters] != vol2.data[i+8*simdIters] );

#else  // #ifndef CIMPL_DONT_AVX

  int simdIters = (int)( vol1.nVox / 4 );
  __m128* l = (__m128*) vol1.data;
  __m128* r = (__m128*) vol2.data;

  for(size_t i = 0; i<simdIters; ++i, ++l, ++r, outData += 4)
    _mm_store_ps( outData, _mm_cmpneq_ps( *l, *r ) );

  for( int i=0; i < vol1.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = (float)( vol1.data[i+4*simdIters] != vol2.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else  //  #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<vol1.nVox; ++i )
    out->data[i] = ( vol1.data[i] != vol2.data[i] ? 1 : 0 );

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_concatCmpImgsH( cimpl_cmpImg const img1, cimpl_cmpImg const img2, cimpl_cmpImg * const out ){
  assert( img1.h == img2.h );  assert( out->h == img1.h );
  assert( out->w == img1.w + img2.w );

  for( size_t x=0; x<out->w; ++x ){
    cimpl_memcpy( out->data+x*out->w, img1.data+x*img1.h, img1.h*sizeof(complex float) );
    cimpl_memcpy( out->data+x*out->w, img2.data+x*img2.h, img2.h*sizeof(complex float) );
  }
}

void cimpl_concatCmpImgsW( cimpl_cmpImg const img1, cimpl_cmpImg const img2, cimpl_cmpImg * const out ){
  assert( img1.h == img2.h );  assert( out->h == img1.h );
  assert( out->w == img1.w + img2.w );

  cimpl_memcpy( out->data, img1.data, img1.nPix*sizeof(complex float) );
  cimpl_memcpy( out->data, img2.data, img2.nPix*sizeof(complex float) );
}

void cimpl_concatImgsH( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out ){
  assert( out->w == img1.w );  assert( out->w == img2.w );
  assert( out->h == img1.h + img2.h );

  for( size_t x=0; x<out->w; ++x ){
    cimpl_memcpy( out->data+x*out->w, img1.data+x*img1.h, img1.h*sizeof(float) );
    cimpl_memcpy( out->data+x*out->w+img1.h, img2.data+x*img2.h, img2.h*sizeof(float) );
  }
}

void cimpl_concatImgsW( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out ){
  assert( img1.h == img2.h );  assert( out->h == img1.h );
  assert( out->w == img1.w + img2.w );

  cimpl_memcpy( out->data, img1.data, img1.nPix*sizeof(float) );
  cimpl_memcpy( out->data + img1.nPix, img2.data, img2.nPix*sizeof(float) );
}

void cimpl_concatVolsH( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out ){
  assert( out->w == vol1.w );  assert( out->s == vol1.s );
  assert( out->w == vol2.w );  assert( out->s == vol2.s );
  assert( out->h == vol1.h + vol2.h );
  
  size_t sliceSize1 = vol1.h * vol1.w;
  size_t sliceSize2 = vol2.h * vol2.w;
  size_t sliceSizeOut = out->h * out->w;
  
  for( size_t x=0; x<out->w; ++x ){
    for( size_t s=0; s<out->s; ++s ){
      cimpl_memcpy( out->data + x*out->w + s*sliceSizeOut, vol1.data + x*vol1.w + s*sliceSize1,
        vol1.h*sizeof(float) );
      cimpl_memcpy( out->data + x*out->w + s*sliceSizeOut + vol2.h, vol2.data + x*vol2.w + s*sliceSize2,
        vol2.h*sizeof(float) );
  } }
}

void cimpl_concatVolsS( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out ){
  assert( vol1.h == vol2.h );  assert( vol1.w == vol2.w );
  assert( out->h == vol1.h );  assert( out->w == vol1.w );
  assert( out->s == vol1.s + vol2.s );
  
  cimpl_memcpy( out->data, vol1.data, vol1.nVox*sizeof(float) );
  cimpl_memcpy( out->data+vol1.nVox, vol2.data, vol2.nVox*sizeof(float) );
}

void cimpl_concatCmpVolsW( cimpl_cmpVol const vol1, cimpl_cmpVol const vol2, cimpl_cmpVol * const out ){
  assert( out->h == vol1.h );  assert( out->s == vol1.s );
  assert( out->h == vol2.h );  assert( out->s == vol2.s );
  assert( out->w == vol1.w + vol2.w );
  
  size_t sliceSize1 = vol1.h*vol1.w;
  size_t sliceSize2 = vol2.h*vol2.w;
  
  for( size_t s=0; s<out->h*out->w; ++s ){
    cimpl_memcpy( out->data+s*out->h*out->w, vol1.data+s*sliceSize1, sizeof(complex float)*sliceSize1 );
    cimpl_memcpy( out->data+s*out->h*out->w + sliceSize1*sizeof(complex float), vol2.data+s*sliceSize2,
           sizeof(complex float)*sliceSize2 );
  }
}

void cimpl_concatCmpVolsH( cimpl_cmpVol const vol1, cimpl_cmpVol const vol2, cimpl_cmpVol * const out ){
  assert( out->w == vol1.w );  assert( out->s == vol1.s );
  assert( out->w == vol2.w );  assert( out->s == vol2.s );
  assert( out->h == vol1.h + vol2.h );

  size_t sliceSize1 = vol1.h * vol1.w;
  size_t sliceSize2 = vol2.h * vol2.w;
  size_t sliceSizeOut = out->h * out->w;

  for( size_t x=0; x<out->w; ++x ){
    for( size_t s=0; s<out->s; ++s ){
      cimpl_memcpy( out->data + x*out->w + s*sliceSizeOut, vol1.data + x*vol1.w + s*sliceSize1,
        vol1.h*sizeof(complex float) );
      cimpl_memcpy( out->data + x*out->w + s*sliceSizeOut + vol2.h, vol2.data + x*vol2.w + s*sliceSize2,
        vol2.h*sizeof(complex float) );
  } }
}

void cimpl_concatCmpVolsS( cimpl_cmpVol const vol1, cimpl_cmpVol const vol2, cimpl_cmpVol * const out ){
  assert( out->h == vol1.h );  assert( out->w == vol1.w );
  assert( out->h == vol2.h );  assert( out->w == vol2.w );
  assert( out->s == vol1.s + vol2.s );

  size_t sizeVol1 = vol1.h*vol1.s*vol1.w;
  size_t sizeVol2 = vol2.h*vol2.s*vol2.w;

  cimpl_memcpy( out->data, vol1.data, sizeVol1*sizeof(complex float) );
  cimpl_memcpy( out->data+sizeVol1, vol2.data, sizeVol2*sizeof(complex float) );
}

void cimpl_concatVolsW( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out ){
  assert( out->h == vol1.h );  assert( out->s == vol1.s );
  assert( out->h == vol2.h );  assert( out->s == vol2.s );
  assert( out->w == vol1.w + vol2.w );

  size_t sliceSize1 = vol1.h*vol1.w;
  size_t sliceSize2 = vol2.h*vol2.w;

  for( size_t s=0; s<out->h*out->w; ++s ){
    cimpl_memcpy( out->data+s*out->h*out->w, vol1.data+s*sliceSize1, sizeof(float)*sliceSize1 );
    cimpl_memcpy( out->data+s*out->h*out->w + sliceSize1*sizeof(float), vol2.data+s*sliceSize2,
      sizeof(float)*sliceSize2 );
  }
}

void cimpl_conjCmpImg( cimpl_cmpImg const in, cimpl_cmpImg * const out ){
  assert( out->h = in.h );  assert( out->w = in.w );
  for( size_t i =0; i<in.h*in.w; ++i )
    out->data[i] = conjf( in.data[i] );
}

void cimpl_cropImg( cimpl_img const in, cimpl_img * const out ){
  // Crops data to size of out
  assert( out->h <= in.h );  assert( out->w <= in.w );
  size_t halfH, minH;
  size_t halfW, minW;
  size_t colOffset, minColOffset;

  if( in.h % 2 == 0 )
    halfH = in.h/2;
  else
    halfH = (in.h-1)/2;
  if( in.w % 2 == 0 )
    halfW = in.w/2;
  else
    halfW = (in.w-1)/2;

  if( out->h % 2 == 0 )
    minH = halfH - out->h/2;
  else
    minH = halfH - (out->h-1)/2;
  if( out->w % 2 == 0 )
    minW = halfW - out->w/2;
  else
    minW = halfW - (out->w-1)/2;

  for( size_t x=0; x<out->w; ++x ){
    colOffset = x*out->h;
    minColOffset = (minW+x)*in.h;

    cimpl_memcpy( out->data+colOffset, in.data+minH+minColOffset, out->h*sizeof(float) );
  }
}

void cimpl_cropVol( cimpl_vol const in, cimpl_vol * const out ){
  // Crops data to size of out
  assert( out->w <= in.w );  assert( out->h <= in.h );  assert( out->s <= in.s );
  size_t halfH, minH;
  size_t halfW, minW;
  size_t halfS, minS;
  size_t minColOffset, minSliceOffset;
  size_t colOffset, sliceOffset;

  if( in.h % 2 == 0 )
    halfH = in.h/2;
  else
    halfH = (in.h-1)/2;
  if( in.w % 2 == 0 )
    halfW = in.w/2;
  else
    halfW = (in.w-1)/2;
  if( in.s % 2 == 0 )
    halfS = in.s/2;
  else
    halfS = (in.s-1)/2;

  if( out->h % 2 == 0 )
    minH = halfH - out->h/2;
  else
    minH = halfH - (out->h-1)/2;
  if( out->w % 2 == 0 )
    minW = halfW - out->w/2;
  else
    minW = halfW - (out->w-1)/2;
  if( out->s % 2 == 0 )
    minS = halfS - out->s/2;
  else
    minS = halfS - (out->s-1)/2;

  for( size_t z=0; z<out->s; ++z ){
    sliceOffset = z*out->h*out->w;
    minSliceOffset = (minS+z)*in.h*in.w;

    for( size_t x=0; x<out->w; ++x ){
      colOffset = x*out->h;
      minColOffset = (minW+x)*in.h;

      cimpl_memcpy(out->data+colOffset+sliceOffset, in.data+minH+minColOffset+minSliceOffset,
        out->h*sizeof(float) );
  } }
}

void cimpl_d1hImg( cimpl_img const in, cimpl_img * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int) ((in.h*in.w) / 8);
  const __m256 neg1 = _mm256_set1_ps(-1);
  __m256* inData = (__m256*) in.data;
  __m256* nextInData = (__m256*) in.data+1;

  for( size_t i=0; i<simdIters; ++i, ++inData, ++nextInData, outData+=8 )
    _mm256_store_ps( outData, _mm256_add_ps( *nextInData, _mm256_mul_ps(*inData,neg1) ) );

  for( int i=0; i < (in.h*in.w)-(8*simdIters); ++i )
    out->data[i+8*simdIters] = sqrt( in.data[i+8*simdIters] );

#else  // #ifndef CIMPL_DONT_AVX

  int simdIters = (int) ((in.h*in.w) / 4);
  const __m128 neg1 = _mm_set1_ps(-1);
  __m128* inData = (__m128*) in.data;
  __m128* nextInData = (__m128*) in.data+1;

  for( size_t i=0; i<simdIters; ++i, ++inData, ++nextInData, outData+=4 )
    _mm_store_ps( outData, _mm_add_ps( *nextInData, _mm_mul_ps(*inData,neg1) ) );

  for( int i=0; i < (in.h*in.w)-(4*simdIters); ++i )
    out->data[i+4*simdIters] = sqrt( in.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

  for( size_t x=0; x<in.w; ++x )
    out->data[(in.h-1)+x*in.h] = out->data[(in.h-2)+x*in.h];

#else  // #ifndef CIMPL_DONT_SIMD

  float *next, *current;
  current = in.data;
  next = in.data + 1;
  for( size_t x=0; x<in.w; ++x, ++current, ++next ){
    for( size_t y=0; y<in.h-1; ++y, ++current, ++next )
      out->data[y+x*in.h] = *next - *current;

    out->data[(in.h-1)+x*in.h] = out->data[(in.h-2)+x*in.h];
  }

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_d1hVol( cimpl_vol const in, cimpl_vol * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );  assert( out->s == in.s );
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int) ((in.h*in.w*in.s) / 8);
  const __m256 neg1 = _mm256_set1_ps(-1);
  __m256* inData = (__m256*) in.data;
  __m256* nextInData = (__m256*) in.data+1;

  for( size_t i=0; i<simdIters; ++i, ++inData, ++nextInData, outData+=8 )
    _mm256_store_ps( outData, _mm256_add_ps( *nextInData, _mm256_mul_ps(*inData,neg1) ) );

  for( int i=0; i < (in.h*in.w*in.s)-(8*simdIters)-1; ++i )
    out->data[i+8*simdIters] = in.data[i+8*simdIters+1] - in.data[i+8*simdIters];

#else  // #ifndef CIMPL_DONT_AVX

  int simdIters = (int) ((in.h*in.w*in.s) / 4);
  const __m128 neg1 = _mm_set1_ps(-1);
  __m128* inData = (__m128*) in.data;
  __m128* nextInData = (__m128*) in.data+1;

  for( size_t i=0; i<simdIters; ++i, ++inData, ++nextInData, outData+=4 )
    _mm_store_ps( outData, _mm_add_ps( *nextInData, _mm_mul_ps(*inData,neg1) ) );

  for( int i=0; i < (in.h*in.w*in.s)-(4*simdIters)-1; ++i )
    out->data[i+4*simdIters] = in.data[i+4*simdIters+1] - in.data[i+4*simdIters];

#endif  // #ifndef CIMPL_DONT_AVX

  for( size_t x=0; x<in.w; ++x ){
    for( size_t z=0; z<in.s; ++z )
      out->data[(in.h-1)+x*in.h+z*in.h*in.w] = out->data[(in.h-2)+x*in.h+z*in.w];
  }

#else  // #ifndef CIMPL_DONT_SIMD

  float *next, *current;
  current = in.data;
  next = in.data + 1;
  for( size_t z=0; z<in.s; ++z ){
    for( size_t x=0; x<in.w; ++x, ++current, ++next ){
      for( size_t y=0; y<in.h-1; ++y, ++current, ++next )
        out->data[y+x*in.h] = *next - *current;

      out->data[(in.h-1)+x*in.h] = out->data[(in.h-2)+x*in.h];
    }
  }

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_d1wImg( cimpl_img const in, cimpl_img * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );
#ifndef CIMPL_DONT_SIMD
  float* inColData = in.data;
  float* outColData = out->data;
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int) (in.h / 8);
  const __m256 neg1 = _mm256_set1_ps(-1);

  for( size_t x=0; x<in.w-1; ++x, inColData+=in.h, outColData+=in.h ){
    __m256* inData = (__m256*) inColData;
    __m256* nextInData = (__m256*) inColData + in.h;
    outData = outColData;

    for( size_t i=0; i<simdIters; ++i, ++inData, ++nextInData, outData+=8 )
      _mm256_store_ps( outData, _mm256_add_ps( *nextInData, _mm256_mul_ps(*inData,neg1) ) );

    for( size_t i=0; i < (in.h - 8*simdIters); ++i )
      out->data[i+8*simdIters] = in.data[i+8*simdIters+in.h] - in.data[i+8*simdIters];
  }

#else  // #ifndef CIMPL_DONT_AVX
  
  int simdIters = (int) (in.h / 4);
  const __m128 neg1 = _mm_set1_ps(-1);

  for( size_t x=0; x<in.w-1; ++x, inColData+=in.h, outColData+=in.h ){
    __m128* inData = (__m128*) inColData;
    __m128* nextInData = (__m128*) inColData + in.h;
    outData = outColData;

    for( size_t i=0; i<simdIters; ++i, ++inData, ++nextInData, outData+=4 )
      _mm_store_ps( outData, _mm_add_ps( *nextInData, _mm_mul_ps(*inData,neg1) ) );
    
    for( size_t i=0; i < (in.h - 4*simdIters); ++i )
      out->data[i+4*simdIters] = in.data[i+4*simdIters+in.h] - in.data[i+4*simdIters];
  }

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  float *next, *current;
  current = in.data;
  next = in.data + in.h;
  for( size_t x=0; x<in.w-1; ++x, ++current, ++next ){
    for( size_t y=0; y<in.h; ++y, ++current, ++next )
      out->data[y+x*in.h] = *next - *current;
  }
  
  memcpy( out->data+(in.w-1)*in.h, out->data+(in.w-2)*in.h, in.h*sizeof(float) );

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_d1wVol( cimpl_vol const in, cimpl_vol * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );  assert( out->s == in.s );
#ifndef CIMPL_DONT_SIMD
  float* inColData = in.data;
  float* outColData = out->data;
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int) (in.h / 8);
  const __m256 neg1 = _mm256_set1_ps(-1);

  for( size_t z=0; z<in.s; ++z ){
    for( size_t x=0; x<in.w-1; ++x, inColData+=in.h, outColData+=in.h ){
      __m256* inData = (__m256*) inColData;
      __m256* nextInData = (__m256*) inColData + in.h;
      outData = outColData;

      for( size_t i=0; i<simdIters; ++i, ++inData, ++nextInData, outData+=8 )
        _mm256_store_ps( outData, _mm256_add_ps( *nextInData, _mm256_mul_ps(*inData,neg1) ) );

      for( size_t i=0; i < (in.h - 8*simdIters); ++i )
        out->data[i+8*simdIters] = in.data[i+8*simdIters+in.h] - in.data[i+8*simdIters];
    }

    memcpy( out->data+(in.w-1)*in.h+z*in.h*in.s, out->data+(in.w-2)*in.h+z*in.h*in.s,
      in.h*sizeof(float) );
  }

#else  // #ifndef CIMPL_DONT_AVX

  int simdIters = (int) (in.h / 4);
  const __m128 neg1 = _mm_set1_ps(-1);

  for( size_t z=0; z<in.s; ++z ){
    for( size_t x=0; x<in.w-1; ++x, inColData+=in.h, outColData+=in.h ){
      __m128* inData = (__m128*) inColData;
      __m128* nextInData = (__m128*) inColData + in.h;
      outData = outColData;

      for( size_t i=0; i<simdIters; ++i, ++inData, ++nextInData, outData+=4 )
        _mm_store_ps( outData, _mm_add_ps( *nextInData, _mm_mul_ps(*inData,neg1) ) );

      for( size_t i=0; i < (in.h - 4*simdIters); ++i )
        out->data[i+4*simdIters] = in.data[i+4*simdIters+in.h] - in.data[i+4*simdIters];
    }

    memcpy( out->data+(in.w-1)*in.h+z*in.h*in.s, out->data+(in.w-2)*in.h+z*in.h*in.s,
      in.h*sizeof(float) );
  }

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  float *next, *current;
  for( size_t z=0; z<in.s; ++z ){
    current = in.data + z*in.h*in.w;
    next = current + in.h;

    for( size_t x=0; x<in.w-1; ++x, ++current, ++next ){
      for( size_t y=0; y<in.h; ++y, ++current, ++next )
        out->data[y+x*in.h] = *next - *current;
    }

    memcpy( out->data+(in.w-1)*in.h+z*in.h*in.s, out->data+(in.w-2)*in.h+z*in.h*in.s,
      in.h*sizeof(float) );
  }

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_d1sVol( cimpl_vol const in, cimpl_vol * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );  assert( out->s == in.s );
#ifndef CIMPL_DONT_SIMD
  float* inColData = in.data;
  float* outColData = out->data;
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int) (in.h / 8);
  const __m256 neg1 = _mm256_set1_ps(-1);

  for( size_t z=0; z<in.s-1; ++z ){
    for( size_t x=0; x<in.w; ++x, inColData+=in.h, outColData+=in.h ){
      __m256* inData = (__m256*) inColData;
      __m256* nextInData = (__m256*) inColData + in.h*in.s;
      outData = outColData;

      for( size_t i=0; i<simdIters; ++i, ++inData, ++nextInData, outData+=8 )
        _mm256_store_ps( outData, _mm256_add_ps( *nextInData, _mm256_mul_ps(*inData,neg1) ) );

      for( size_t i=0; i < (in.h - 8*simdIters); ++i )
        out->data[i+8*simdIters] = in.data[i+8*simdIters+in.h*in.w] - in.data[i+8*simdIters];
  } }

#else  // #ifndef CIMPL_DONT_AVX

  int simdIters = (int) (in.h / 4);
  const __m128 neg1 = _mm_set1_ps(-1);

  for( size_t z=0; z<in.s-1; ++z ){
    for( size_t x=0; x<in.w; ++x, inColData+=in.h, outColData+=in.h ){
      __m128* inData = (__m128*) inColData;
      __m128* nextInData = (__m128*) inColData + in.h*in.s;
      outData = outColData;

      for( size_t i=0; i<simdIters; ++i, ++inData, ++nextInData, outData+=4 )
        _mm_store_ps( outData, _mm_add_ps( *nextInData, _mm_mul_ps(*inData,neg1) ) );

      for( size_t i=0; i < (in.h - 4*simdIters); ++i )
        out->data[i+4*simdIters] = in.data[i+4*simdIters+in.h*in.w] - in.data[i+4*simdIters];
  } }

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  float *next, *current;
  for( size_t z=0; z<in.s-1; ++z ){
    current = in.data + z*in.h*in.w;
    next = current + in.h*in.w;

    for( size_t x=0; x<in.w; ++x, ++current, ++next ){
      for( size_t y=0; y<in.h; ++y, ++current, ++next )
        out->data[y+x*in.h+z*in.h*in.w] = *next - *current;
  } }

#endif  // #ifndef CIMPL_DONT_SIMD

  memcpy( out->data+(in.s-1)*in.h*in.w, in.data+(in.s-1)*in.h*in.w, in.h*in.w*sizeof(float) );
}

void cimpl_divideImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out ){
  assert( out->w == img1.w );  assert( out->h == img1.h );
  assert( out->w == img2.w );  assert( out->h == img2.h );
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  unsigned long simdIters = (img1.h*img2.w) / 8;

  __m256* l = (__m256*) img1.data;
  __m256* r = (__m256*) img2.data;

  for( size_t i=0; i<simdIters; ++i, ++l, ++r, ++outData)
    _mm256_store_ps(outData, _mm256_div_ps(*l, *r));

  for( int i=0; i < (img1.h*img1.w)-(8*simdIters); ++i )
    out->data[i+8*simdIters] = img1.data[i+8*simdIters] / img2.data[i+8*simdIters];

#else

  unsigned long simdIters = (img1.h*img2.w) / 4;

  __m128* l = (__m128*) img1.data;
  __m128* r = (__m128*) img2.data;

  for( size_t i=0; i<simdIters; ++i, ++l, ++r, ++outData)
    _mm_store_ps(outData, _mm_div_ps(*l, *r));

  for( int i=0; i < (img1.h*img1.w)-(4*simdIters); ++i )
    out->data[i+4*simdIters] = img1.data[i+4*simdIters] / img2.data[i+4*simdIters];

#endif  // #ifndef CIMPL_DONT_AVX

#else
  for( int i=0; i<img1.h*img1.w; ++i)
    out->data[i] = img1.data[i] / img2.data[i];
#endif
}

void cimpl_divideVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out ){
  assert( out->h == vol1.h );  assert( out->w == vol1.w );  assert( out->s == vol1.s );
  assert( out->h == vol2.h );  assert( out->w == vol2.w );  assert( out->s == vol2.s );
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  unsigned long simdIters = (vol1.h*vol1.w*vol1.s) / 8;

  __m256* l = (__m256*) vol1.data;
  __m256* r = (__m256*) vol2.data;

  for( size_t i=0; i<simdIters; ++i, ++l, ++r, ++outData)
    _mm256_store_ps(outData, _mm256_div_ps(*l, *r));

  for( int i=0; i < (vol1.h*vol1.w*vol1.s)-(8*simdIters); ++i )
    out->data[i+8*simdIters] = vol1.data[i+8*simdIters] / vol2.data[i+8*simdIters];

#else

  unsigned long simdIters = (vol1.h*vol1.w*vol1.s) / 4;
  
  __m128* l = (__m128*) vol1.data;
  __m128* r = (__m128*) vol2.data;
  
  for( size_t i=0; i<simdIters; ++i, ++l, ++r, ++outData)
    _mm_store_ps(outData, _mm_div_ps(*l, *r));
  
  for( int i=0; i < (vol1.h*vol1.w*vol1.s)-(4*simdIters); ++i )
    out->data[i+4*simdIters] = vol1.data[i+4*simdIters] / vol2.data[i+4*simdIters];

#endif  // #ifdef CIMPL_DONT_AVX

#else
  for( int i=0; i<vol1.h*vol1.w*vol1.s; ++i)
    out->data[i] = vol1.data[i] / vol2.data[i];
#endif
}

void cimpl_divideImgByScalar( cimpl_img const in, float const scalar, cimpl_img * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );
  for( size_t i=0; i<in.w*in.h; ++i )
    out->data[i] = in.data[i] / scalar;
}

float cimpl_dotImgs( cimpl_img const img1, cimpl_img const img2 ){
  assert( img1.w == img2.w );  assert( img1.h == img2.h );
  float out = 0;
  for( int i=0; i<img1.h*img1.w; ++i )
    out += img1.data[i] * img2.data[i];

  return out;
}

int cimpl_equalImgs( cimpl_img const img1, cimpl_img const img2 ){
  if( img1.h != img2.h )
    return 0;
  if( img1.w != img2.w )
    return 0;
  for( size_t i=0; i<img1.h*img1.w; ++i){
    if( img1.data[i] != img2.data[i] ){
      return 0;
  } }
  return 1;
}

int cimpl_equalVols( cimpl_vol const vol1, cimpl_vol const vol2 ){
  if( vol1.h != vol2.h )
    return 0;
  if( vol1.w != vol2.w )
    return 0;
  if( vol1.s != vol2.s )
    return 0;
  for( size_t i=0; i<vol1.h*vol1.w*vol1.s; ++i){
    if( vol1.data[i] != vol2.data[i] ){
      return 0;
  } }
  return 1;
}

void cimpl_flipImgLR( cimpl_img const in, cimpl_img * const out ){
  // Flip image left/right
  assert( out->h == in.h );  assert( out->w == in.w );
  assert( out->data != in.data );
  for( int x=0; x<in.w; ++x ){
    for( int y=0; y<in.h; ++y ){
      out->data[y+x*in.h] = in.data[y+(in.w-x-1)*in.h];
  } }
}

void cimpl_flipImgUD( cimpl_img const in, cimpl_img * const out ){
  // Flip image up/down
  assert( out->h == in.h );  assert( out->w == in.w );
  assert( out->data != in.data );
  for( int x=0; x<in.w; ++x ){
    for( int y=0; y<in.h; ++y ){
      out->data[y+x*in.h] = in.data[(in.h-y-1)+x*in.h];
  } }
}

void cimpl_floorImg( cimpl_img const img, cimpl_img * const out ){
  assert( out->h == img.h );  assert( out->w == img.w );
#if !defined(CIMPL_DONT_SSE4PT1) || !defined(CIMPL_DONT_SIMD)
  float* outData = out->data;
  int simdIters = (int) ((img.h*img.w) / 4);
  __m128* ptr = (__m128*)img.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 4)
    _mm_store_ps(outData, _mm_floor_ps(*ptr));

  for( int i=0; i < (img.h*img.w)-(4*simdIters); ++i )
    out->data[i+4*simdIters] = floorf( img.data[i+4*simdIters] );

#else
  for( size_t i=0; i<img.h*img.w; ++i )
    out->data[i] = floorf( img.data[i] );
#endif
}

void cimpl_floorVol( cimpl_vol const vol, cimpl_vol * const out ){
  assert( out->h == vol.h );  assert( out->w == vol.w );  assert( out->s == vol.s );
#if !defined(CIMPL_DONT_SSE4PT1) || !defined(CIMPL_DONT_SIMD)
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int) ((vol.h*vol.w*vol.s) / 8);
  __m256* ptr = (__m256*)vol.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 8)
    _mm256_store_ps(outData, _mm256_floor_ps(*ptr));

  for( int i=0; i < (vol.h*vol.w)-(8*simdIters); ++i )
    out->data[i+8*simdIters] = floorf( vol.data[i+8*simdIters] );

#else

  int simdIters = (int) ((vol.h*vol.w*vol.s) / 4);
  __m128* ptr = (__m128*)vol.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 4)
    _mm_store_ps(outData, _mm_floor_ps(*ptr));

  for( int i=0; i < (vol.h*vol.w)-(4*simdIters); ++i )
    out->data[i+4*simdIters] = floorf( vol.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else
  for( size_t i=0; i<vol.h*vol.w*vol.s; ++i )
    out->data[i] = floorf( vol.data[i] );
#endif
}

void cimpl_freeCmpImg( cimpl_cmpImg * const in ){
  in->h = in->w = 0;
  free( in->data );  in->data = NULL;
}

void cimpl_freeImg( cimpl_img *in ){
  in->h = in->w = 0;
  free( in->data );
  in->data = NULL;
}

void cimpl_freeVol( cimpl_vol * const in ){
  in->h = in->w = in->s = 0;
  free( in->data );
  in->data = NULL;
}

void cimpl_getImagImg( cimpl_cmpImg const in, cimpl_img * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );
  for( size_t i=0; i<out->h*out->w; ++i )
    in.data[i] = cimagf( out->data[i] );
}

void cimpl_getImagVol( cimpl_cmpVol const in, cimpl_vol * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );  assert( out->s == in.s );
  for( size_t i=0; i<out->h*out->w*out->s; ++i )
    in.data[i] = cimagf( out->data[i] );
}

void cimpl_getRealImg( cimpl_cmpImg const in, cimpl_img * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  for( size_t i=0; i<out->h*out->w; ++i )
    in.data[i] = crealf( out->data[i] );
}

void cimpl_getRealVol( cimpl_cmpVol const in, cimpl_vol * const out ){
  assert( out->h == in.h );
  assert( out->w == in.w );
  assert( out->s == in.s );
  for( size_t i=0; i<out->h*out->w*out->s; ++i )
    in.data[i] = crealf( out->data[i] );
}

float cimpl_linInterp( size_t const N, float const * const x, float const * const y,
  float const outOfBounds, float const q ){
  // N is the number of values in the x and y arrays
  // x is an array of inputs (or domain values) in ascending order
  // y is an array of outputs (or range values) where y[i] corresponds to x[i]
  // outOfBounds is the value to return if extrapolating
  // q is the query domain value.

  float out=outOfBounds;
  size_t lowIndx=0;
  size_t highIndx=N-1;
  size_t midIndx;

  if( q < x[0] || q > x[N-1] ) return outOfBounds;

  // Perform a binary search
  while( highIndx - lowIndx > 1 ){
    midIndx = lowIndx + (highIndx-lowIndx)/2;
    if( q < x[midIndx] ){
      highIndx = midIndx;
    } else {
      lowIndx = midIndx;
    }
  }
  out = y[lowIndx] + (y[highIndx]-y[lowIndx])/(x[highIndx]-x[lowIndx]) * (q-x[lowIndx]);

  return out;
}

void cimpl_linInterps( size_t const N, float const * const x, float const * const y,
                      float const outOfBounds, size_t const M, float const * const q,
                      float * const out ){
  // This function takes advantage of order of q for greater efficiency than calling
  //   cimpl_linInterp multiple times.
  // N is the number of values in the x and y arrays
  // x is an array of inputs (or domain values) in ascending order
  // y is an array of outputs (or range values) where y[i] corresponds to x[i]
  // outOfBounds is the value to return if extrapolating
  // M is the number of query values
  // q are the query domain values in ascending order.

  size_t qIndx=0;
  size_t xIndx;

  while( q[qIndx] < x[0] ){
    out[qIndx] = outOfBounds;
    qIndx++;
    if( qIndx >= M )
      return;
  }

  for( xIndx=0; xIndx<N-1; ++xIndx ){
    while( q[qIndx] > x[xIndx] && q[qIndx] < x[xIndx+1] ){
      out[qIndx] = (y[xIndx+1]-y[xIndx])/(x[xIndx+1]-x[xIndx]) * (q[qIndx]-x[xIndx]);
      ++qIndx;
  } }

  while( qIndx < M ){
    out[qIndx] = outOfBounds;
    ++qIndx;
  }

  return;
}

void cimpl_linInterpImg( cimpl_img const img, size_t const N, float const * const xq,
  float const * const yq, float const outOfBounds, float * const out ){
  size_t x1, x2, y1, y2;
  float tmpX1, tmpX2;
  for( size_t i=0; i<N; ++i ){
    assert( xq[i] >= 0 );  assert( xq[i] <= img.w );
    assert( yq[i] >= 0 );  assert( yq[i] <= img.h );
    if( xq[i]<0 || xq[i]>=img.w || yq[i]<0 || yq[i]>=img.h ){
      out[i] = outOfBounds;
    } else {
      x1 = floorf( xq[i] );  x2 = ceilf( xq[i] );
      y1 = floorf( yq[i] );  y2 = ceilf( yq[i] );
      tmpX1 = (y2-yq[i])*img.data[y1+x1*img.h] + (yq[i]-y1)*img.data[y2+x1*img.h];
      tmpX2 = (y2-yq[i])*img.data[y1+x2*img.h] + (yq[i]-y1)*img.data[y2+x2*img.h];
      out[i] = (x2-xq[i])*tmpX1 + (xq[i]-x1)*tmpX2;
  } }
}

cimpl_cmpImg cimpl_mallocCmpImg( size_t const h, size_t const w ){
  cimpl_cmpImg out;
  out.h = h;
  out.w = w;
  out.nPix = h*w;
  
#ifndef CIMPL_DONT_SIMD
  
#ifndef CIMPL_DONT_AVX

#ifdef _MSC_VER
  out.data = (complex float*) aligned_alloc( 32, sizeof(complex float) * w * h );
#else
  posix_memalign( (void**) &out.data, 32, sizeof(complex float) * w * h );
#endif

#else  // #ifndef CIMPL_DONT_AVX

#ifdef _MSC_VER
  out.data = (complex float*) aligned_alloc( 16, sizeof(complex float) * w * h );
#else
  posix_memalign( (void**) &out.data, 16, sizeof(complex float) * w * h );
#endif
  
#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  out.data = (complex float*) malloc( sizeof(complex float) * w * h );

#endif  // #ifndef CIMPL_DONT_SIMD
  return out;
}

cimpl_cmpVol cimpl_mallocCmpVol( size_t const h, size_t const w , size_t const s ){
  cimpl_cmpVol out;
  out.h = h;
  out.w = w;
  out.s = s;
  out.nVox = h*w*s;

#ifndef CIMPL_DONT_SIMD

#ifndef CIMPL_DONT_AVX

#ifdef _MSC_VER
  out.data = (complex float*) aligned_alloc( 32, sizeof(complex float) * w * h * s );
#else
  posix_memalign( (void**) &out.data, 32, sizeof(complex float) * w * h * s );
#endif

#else  // #ifndef CIMPL_DONT_AVX

#ifdef _MSC_VER
  out.data = (complex float*) aligned_alloc( 16, sizeof(complex float) * w * h * s );
#else
  posix_memalign( (void**) &out.data, 16, sizeof(complex float) * w * h * s );
#endif

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  out.data = (complex float*) malloc( sizeof(complex float) * w * h * s );

#endif  // #ifndef CIMPL_DONT_SIMD
  return out;
}

cimpl_img cimpl_mallocImg( size_t const h, size_t const w ){
  cimpl_img out;
  out.h = h;
  out.w = w;
  out.nPix = h*w;
#ifndef CIMPL_DONT_SIMD
  
#ifndef CIMPL_DONT_AVX
  
#ifdef _MSC_VER
  out.data = (float*) aligned_alloc( 32, sizeof(float) * w * h );
#else
  posix_memalign( (void**) &out.data, 32, sizeof(float) * w * h );
#endif

#else  // ifndef CIMPL_DONT_AVX

#ifdef _MSC_VER
  out.data = (float*) aligned_alloc( 16, sizeof(float) * w * h );
#else
  posix_memalign( (void**) &out.data, 16, sizeof(float) * w * h );
#endif
  
#endif  // ifndef CIMPL_DONT_AVX

#else  // ifndef CIMPL_DONT_SIMD

  out.data = (float*) malloc( sizeof(float) * w * h );

#endif  // ifndef CIMPL_DONT_SIMD
  return out;
}

cimpl_vol cimpl_mallocVol( size_t const h, size_t const w, size_t const s ){
  cimpl_vol out;
  out.h = h;
  out.w = w;
  out.s = s;
  out.nVox = h*w*s;
#ifndef CIMPL_DONT_SIMD
  
#ifndef CIMPL_DONT_AVX

#ifdef _MSC_VER
  out.data = (float*) aligned_alloc( 32, sizeof(float) * w * h * s );
#else
  posix_memalign( (void**) &out.data, 32, sizeof(float) * w * h * s );
#endif

#else  // #ifndef CIMPL_DONT_AVX

#ifdef _MSC_VER
  out.data = (float*) aligned_alloc( 16, sizeof(float) * w * h * s );
#else
  posix_memalign( (void**) &out.data, 16, sizeof(float) * w * h * s );
#endif

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  out.data = (float*) malloc( sizeof(float) * w * h * s );

#endif  // #ifndef CIMPL_DONT_SIMD
  return out;
}

void cimpl_maxImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out ){
  assert( out->h == img1.h );  assert( out->w == img1.w );
  assert( out->h == img2.h );  assert( out->w == img2.w );
#ifndef CIMPL_DONT_SIMD
  size_t i;

#ifndef CIMPL_DONT_AVX

  unsigned long simdIters = (img1.h*img1.w) / 8;

  __m256* l = (__m256*) img1.data;
  __m256* r = (__m256*) img2.data;
  float* outData = out->data;

  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=8 )
    _mm256_store_ps(outData, _mm256_max_ps(*l, *r));

  for( i=0; i < (img1.h*img1.w)-(8*simdIters); ++i )
    out->data[i+8*simdIters] = fmaxf( img1.data[i+8*simdIters], img2.data[i+8*simdIters] );

#else

  unsigned long simdIters = (img1.h*img1.w) / 4;

  __m128* l = (__m128*) img1.data;
  __m128* r = (__m128*) img2.data;
  float* outData = out->data;

  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=4 )
    _mm_store_ps(outData, _mm_max_ps(*l, *r));

  for( i=0; i < (img1.h*img1.w)-(4*simdIters); ++i )
    out->data[i+4*simdIters] = fmaxf( img1.data[i+4*simdIters], img2.data[i+4*simdIters] );
#endif  // #ifndef CIMPL_DONT_AVX
  
#else
  for( size_t i=0; i<out->h*out->w; ++i )
    out->data[i] = fmaxf( img1.data[i], img2.data[i] );
#endif
}

void cimpl_maxVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out ){
  assert( out->h == vol1.h );  assert( out->w == vol1.w );  assert( out->s == vol1.s );
  assert( out->h == vol2.h );  assert( out->w == vol2.w );  assert( out->s == vol2.s );
#ifndef CIMPL_DONT_SIMD
  size_t i;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = vol1.nVox / 8;

  __m256* l = (__m256*) vol1.data;
  __m256* r = (__m256*) vol2.data;
  float* outData = out->data;

  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=8 )
    _mm256_store_ps(outData, _mm256_max_ps(*l, *r));

  for( i=0; i < (vol1.h*vol1.w)-(8*simdIters); ++i )
    out->data[i+8*simdIters] = fmaxf( vol1.data[i+8*simdIters], vol2.data[i+8*simdIters] );

#else

  size_t simdIters = vol1.nVox / 4;

  __m128* l = (__m128*) vol1.data;
  __m128* r = (__m128*) vol2.data;
  float* outData = out->data;

  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=4 )
    _mm_store_ps(outData, _mm_max_ps(*l, *r));

  for( i=0; i < (vol1.h*vol1.w)-(4*simdIters); ++i )
    out->data[i+4*simdIters] = fmaxf( vol1.data[i+4*simdIters], vol2.data[i+4*simdIters] );
#endif  // #ifndef CIMPL_DONT_AVX

#else
  for( size_t i=0; i<out->h*out->w; ++i )
    out->data[i] = fmaxf( vol1.data[i], vol2.data[i] );
#endif
}

void cimpl_minImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out ){
  assert( out->h == img1.h );  assert( out->w == img1.w );
  assert( out->h == img2.h );  assert( out->w == img2.w );
#ifndef CIMPL_DONT_SIMD
  size_t i;
  
#ifndef CIMPL_DONT_AVX
  
  unsigned long simdIters = (img1.h*img1.w) / 8;
  
  __m256* l = (__m256*) img1.data;
  __m256* r = (__m256*) img2.data;
  float* outData = out->data;
  
  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=8 )
    _mm256_store_ps(outData, _mm256_min_ps(*l, *r));
  
  for( i=0; i < (img1.h*img1.w)-(8*simdIters); ++i )
    out->data[i+8*simdIters] = fminf( img1.data[i+8*simdIters], img2.data[i+8*simdIters] );
  
#else
  
  unsigned long simdIters = (img1.h*img1.w) / 4;
  
  __m128* l = (__m128*) img1.data;
  __m128* r = (__m128*) img2.data;
  float* outData = out->data;
  
  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=4 )
    _mm_store_ps(outData, _mm_min_ps(*l, *r));
  
  for( i=0; i < (img1.h*img1.w)-(4*simdIters); ++i )
    out->data[i+4*simdIters] = fminf( img1.data[i+4*simdIters], img2.data[i+4*simdIters] );
#endif  // #ifndef CIMPL_DONT_AVX
  
#else
  for( size_t i=0; i<out->h*out->w; ++i )
    out->data[i] = fminf( img1.data[i], img2.data[i] );
#endif
}

void cimpl_minVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out ){
  assert( out->h == vol1.h );  assert( out->w == vol1.w );  assert( out->s == vol1.s );
  assert( out->h == vol2.h );  assert( out->w == vol2.w );  assert( out->s == vol2.s );
#ifndef CIMPL_DONT_SIMD
  size_t i;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = vol1.nVox / 8;

  __m256* l = (__m256*) vol1.data;
  __m256* r = (__m256*) vol2.data;
  float* outData = out->data;

  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=8 )
    _mm256_store_ps(outData, _mm256_min_ps(*l, *r));

  for( i=0; i < vol1.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = fminf( vol1.data[i+8*simdIters], vol2.data[i+8*simdIters] );

#else

  size_t simdIters = vol1.nVox / 4;

  __m128* l = (__m128*) vol1.data;
  __m128* r = (__m128*) vol2.data;
  float* outData = out->data;

  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=4 )
    _mm_store_ps(outData, _mm_max_ps(*l, *r));

  for( i=0; i < vol1.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = fminf( vol1.data[i+4*simdIters], vol2.data[i+4*simdIters] );
#endif  // #ifndef CIMPL_DONT_AVX

#else
  for( size_t i=0; i<out->nVox; ++i )
    out->data[i] = fminf( vol1.data[i], vol2.data[i] );
#endif
}

void cimpl_multiplyImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out ){
  assert( out->w == img1.w );  assert( out->h == img1.h );
  assert( out->w == img2.w );  assert( out->h == img2.h );
#ifndef CIMPL_DONT_SIMD
float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = img1.nPix / 8;

  __m256* l = (__m256*) img1.data;
  __m256* r = (__m256*) img2.data;

  for( size_t i=0; i<simdIters; ++i, ++l, ++r, outData+=8 )
    _mm256_store_ps(outData, _mm256_mul_ps(*l, *r));

  for( int i=0; i < img1.nPix-(4*simdIters); ++i )
    out->data[i+8*simdIters] = img1.data[i+8*simdIters] * img2.data[i+8*simdIters];

#else

  size_t simdIters = img1.nPix / 4;

  __m128* l = (__m128*) img1.data;
  __m128* r = (__m128*) img2.data;

  for( size_t i=0; i<simdIters; ++i, ++l, ++r, outData+=4 )
    _mm_store_ps(outData, _mm_mul_ps(*l, *r));

  for( int i=0; i < img1.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = img1.data[i+4*simdIters] * img2.data[i+4*simdIters];

#endif  // #ifndef CIMPL_DONT_SIMD

#else
  for( int i=0; i<img1.nPix; ++i)
    out->data[i] = img1.data[i] * img2.data[i];
#endif
}

void cimpl_multiplyVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out ){
  assert( out->h == vol1.h );  assert( out->w == vol1.w );  assert( out->s == vol1.s );
  assert( out->h == vol2.h );  assert( out->w == vol2.w );  assert( out->s == vol2.s );
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = vol1.nVox / 8;

  __m256* l = (__m256*) vol1.data;
  __m256* r = (__m256*) vol2.data;

  for( size_t i=0; i<simdIters; ++i, ++l, ++r, ++outData)
    _mm256_store_ps(outData, _mm256_mul_ps(*l, *r));

  for( int i=0; i < vol1.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = vol1.data[i+8*simdIters] * vol2.data[i+8*simdIters];

#else

  unsigned long simdIters = vol1.nVox / 4;

  __m128* l = (__m128*) vol1.data;
  __m128* r = (__m128*) vol2.data;

  for( size_t i=0; i<simdIters; ++i, ++l, ++r, ++outData)
    _mm_store_ps(outData, _mm_mul_ps(*l, *r));

  for( int i=0; i < vol1.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = vol1.data[i+4*simdIters] * vol2.data[i+4*simdIters];

#endif  // #ifndef CIMPL_DONT_AVX

#else
  for( int i=0; i<vol1.nVox; ++i)
    out->data[i] = vol1.data[i] * vol2.data[i];
#endif
}

void cimpl_multiplyImgByScalar( cimpl_img const in, float const scalar, cimpl_img * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );

#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int)( in.nPix / 8 );
  const __m256 s = _mm256_set1_ps(scalar);
  __m256* inData = (__m256*) in.data;

  for(size_t i = 0; i<simdIters; ++i, ++inData, outData += 8)
    _mm256_store_ps(outData, _mm256_mul_ps(*inData, s));

  for( int i=0; i < in.nPix-(8*simdIters); ++i )
    out->data[i+8*simdIters] = in.data[i+8*simdIters] * scalar;

#else

  int simdIters = (int)( in.nPix / 4 );
  const __m128 s = _mm_set1_ps(scalar);
  __m128* inData = (__m128*) in.data;

  for(size_t i = 0; i<simdIters; ++i, ++inData, outData += 4)
    _mm_store_ps(outData, _mm_mul_ps(*inData, s));

  for( int i=0; i < in.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = in.data[i+4*simdIters] * scalar;

#endif  // #ifndef CIMPL_DONT_AVX

#else

  for( size_t i=0; i<in.nPix; ++i )
    out->data[i] = in.data[i] * scalar;

#endif
}

void cimpl_multiplyVolByScalar( cimpl_vol const in, float const scalar, cimpl_vol * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );  assert( out->s == in.s );

#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int)( in.nVox / 8 );
  const __m256 s = _mm256_set1_ps(scalar);
  __m256* inData = (__m256*) in.data;

  for(size_t i = 0; i<simdIters; ++i, ++inData, outData += 8)
    _mm256_store_ps(outData, _mm256_div_ps(*inData, s));

  for( int i=0; i < in.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = in.data[i+8*simdIters] * scalar;

#else  // #ifndef CIMPL_DONT_AVX

  int simdIters = (int)( in.nVox / 4 );
  const __m128 s = _mm_set1_ps(scalar);
  __m128* inData = (__m128*) in.data;

  for(size_t i = 0; i<simdIters; ++i, ++inData, outData += 4)
    _mm_store_ps(outData, _mm_div_ps(*inData, s));

  for( int i=0; i < in.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = in.data[i+4*simdIters] * scalar;

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<in.nVox; ++i )
    out->data[i] = in.data[i] * scalar;

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_printImg( cimpl_img const in ){
  for( size_t y=0; y<in.h; ++y ){
    for( size_t x=0; x<in.w; ++x ){
      printf( "%f ", in.data[y+x*in.h] );
    }
    printf( "\n" );
  }
}

void cimpl_rcpImg( cimpl_img const in, cimpl_img * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;
  
#ifndef CIMPL_DONT_AVX
  
  int simdIters = (int)( in.nPix / 8 );
  __m256* ptr = (__m256*)in.data;
  
  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 8)
    _mm256_store_ps(outData, _mm256_rcp_ps(*ptr));
  
  for( int i=0; i < in.nPix-(8*simdIters); ++i )
    out->data[i+8*simdIters] = 1 / in.data[i+8*simdIters];
  
#else

  int simdIters = (int)( in.nPix / 4 );
  __m128* ptr = (__m128*)in.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 4)
    _mm_store_ps(outData, _mm_rcp_ps(*ptr));

  for( int i=0; i < in.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = 1 / in.data[i+4*simdIters];

#endif

#else
  for( size_t i=0; i<in.nPix; ++i )
    out->data[i] = 1 / in.data[i];
#endif
}

void cimpl_rcpVol( cimpl_vol const in, cimpl_vol * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );  assert( out->s == in.s );
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int)( in.nVox / 8 );
  __m256* ptr = (__m256*)in.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 8)
    _mm256_store_ps(outData, _mm256_rcp_ps(*ptr));

  for( int i=0; i < in.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = 1 / in.data[i+8*simdIters];

#else

  int simdIters = (int)( in.nVox / 4 );
  __m128* ptr = (__m128*)in.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 4)
    _mm_store_ps(outData, _mm_rcp_ps(*ptr));

  for( int i=0; i < in.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = 1 / in.data[i+4*simdIters];

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<in.nVox; ++i )
    out->data[i] = 1 / in.data[i];

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_reshapeCmpImg( size_t H, size_t W, cimpl_cmpImg * const img ){
  assert( H*W == img->h*img->w );
  img->h = H;  img->w = W;
}

void cimpl_reshapeCmpVol( size_t H, size_t W, size_t S, cimpl_vol * const out ){
  assert( H*W*S == out->h*out->w*out->s );
  out->h = H;  out->w = W;  out->s = S;
}

void cimpl_reshapeImg( size_t H, size_t W, cimpl_img * const out ){
  assert( H*W == out->h*out->w );
  out->h = H;  out->w = W;
}

void cimpl_reshapeVol( size_t H, size_t W, size_t S, cimpl_vol * const out ){
  assert( H*W*S == out->h*out->w*out->s );
  out->h = H;  out->w = W;  out->s = S;
}

//void cimpl_rot( cimpl_img const in, float const angle, cimpl_img * const out ){
//  // angle has units of radians
//  float sAngle, cAngle;
//  size_t cx, cy;
//
//  sAngle = sin( angle );
//  cAngle = cos( angle );
//  cx = ceil( (float) out->w / 2 );
//  cy = ceil( (float) out->h / 2 );
//
//  for( int x=0; x<out->w; ++x){
//    for( int y=0; y<out->h; ++y){
//
//  } }
//}

void cimpl_rot90( cimpl_img const in, cimpl_img * const out ){
  assert( out->h == in.w );  assert( out->w == in.h );
  assert( out->data != in.data );
  for( size_t x=0; x<out->w; ++x ){
    for( size_t y=0; y<out->h; ++y )
      out->data[y+x*out->h] = in.data[x+(out->h-y-1)*in.h];
  }
}

void cimpl_rot180( cimpl_img const in, cimpl_img * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );
  assert( out->data != in.data );
  for( size_t i=0; i<in.nPix; ++i )
    out->data[i] = in.data[in.nPix-i-1];
}

void cimpl_rot270( cimpl_img const in, cimpl_img * const out ){
  assert( out->h == in.w );  assert( out->w == in.h );
  assert( out->data != in.data );
  for( size_t x=0; x<out->w; ++x ){
    for( size_t y=0; y<out->h; ++y )
      out->data[y+x*out->h] = in.data[(out->w-x-1)+y*in.h];
  }
}

void cimpl_roundImg( cimpl_img const img, cimpl_img * const out ){
  assert( out->h == img.h );  assert( out->w == img.w );
#if !defined(CIMPL_DONT_SSE4PT1) || !defined(CIMPL_DONT_SIMD)
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int)( img.nPix / 8 );
  __m256* ptr = (__m256*)img.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 8){
    _mm256_store_ps(outData,
      _mm256_round_ps( *ptr, (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) )
    );
  }

  for( int i=0; i < (img.h*img.w)-(8*simdIters); ++i )
    out->data[i+8*simdIters] = roundf( img.data[i+8*simdIters] );

#else
  int simdIters = (int)( img.nPix / 4 );
  __m128* ptr = (__m128*)img.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 4){
    _mm_store_ps(outData,
     _mm_round_ps( *ptr, (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) )
    );
  }
  
  for( int i=0; i < img.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = roundf( img.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX
  
#else
  for( size_t i=0; i<img.nPix; ++i )
    out->data[i] = roundf( img.data[i] );
#endif
}

void cimpl_roundVol( cimpl_vol const vol, cimpl_vol * const out ){
  assert( out->h == vol.h );  assert( out->w == vol.w );  assert( out->s == vol.s );
#if !defined(CIMPL_DONT_SSE4PT1) || !defined(CIMPL_DONT_SIMD)
  float* outData = out->data;
  
#ifndef CIMPL_DONT_AVX

  int simdIters = (int)( vol.nVox / 8 );
  __m256* ptr = (__m256*) vol.data;
  
  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 8){
    _mm256_store_ps(outData,
      _mm256_round_ps( *ptr, (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) )
    );
  }

  for( int i=0; i < vol.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = roundf( vol.data[i+8*simdIters] );

#else

  int simdIters = (int)( vol.nVox / 4 );
  __m128* ptr = (__m128*)vol.data;

  for( size_t i = 0; i<simdIters; ++i, ++ptr, outData += 4 ){
    _mm_store_ps(outData,
      _mm_round_ps( *ptr, (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) )
    );
  }

  for( int i=0; i < vol.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = roundf( vol.data[i+4*simdIters] );

#endif

#else
  for( size_t i=0; i<vol.nVox; ++i )
    out->data[i] = roundf( vol.data[i] );
#endif
}

void cimpl_sliceImgX( cimpl_img const in, size_t xIndx, float * const out ){
  assert( xIndx < in.w );
  cimpl_memcpy( out, in.data+xIndx*in.h, in.h*sizeof(float) );
}

void cimpl_sliceImgY( cimpl_img const in, size_t yIndx, float * const out ){
  assert( yIndx < in.h );
  for( size_t x=0; x<in.h; ++x )
    out[x] = in.data[yIndx+x*in.h];
}

void cimpl_sliceX( cimpl_vol const in, size_t xIndx, cimpl_img * const out ){
  assert( in.h == out->h );  assert( in.s == out->w );
  assert( xIndx < in.w );
  for( size_t x=0; x<out->w; ++x )
    cimpl_memcpy(out->data+x*out->h, in.data+xIndx*in.h+x*out->h*out->w, out->h*sizeof(float) );
}

void cimpl_sliceXZ( cimpl_vol const in, size_t xIndx, size_t zIndx,
  float * const out ){
  assert( xIndx < in.w );  assert( zIndx < in.s );
  cimpl_memcpy( out, in.data+xIndx*in.h+zIndx*in.h*in.w, in.h*sizeof(float) );
}

void cimpl_sliceY( cimpl_vol const in, size_t yIndx, cimpl_img * const out ){
  assert( in.w == out->h );  assert( in.s == out->w );
  assert( yIndx < in.w );
  for( size_t x=0; x<out->w; ++x ){
    for( size_t y=0; y<out->h; ++y )
      out->data[y+x*out->h] = in.data[ yIndx+x*in.h+y*out->h*out->w ];
  }
}

void cimpl_sliceYX( cimpl_vol const in, size_t yIndx, size_t xIndx,
  float * const out ){
  assert( yIndx < in.h );
  assert( xIndx < in.w );
  for( size_t z=0; z<in.s; ++z )
    out[z] = in.data[yIndx+xIndx*in.h+z*in.h*in.w];
}

void cimpl_sliceYZ( cimpl_vol const in, size_t yIndx, size_t zIndx,
  float * const out ){
  assert( yIndx < in.h );
  assert( zIndx < in.s );
  for( size_t x=0; x<in.w; ++x )
    out[x] = in.data[yIndx+x*in.h+zIndx*in.h*in.w];
}

void cimpl_sliceZ( cimpl_vol const in, size_t zIndx, cimpl_img * const out ){
  assert( in.h == out->h );  assert( in.w == out->w );
  assert( zIndx < in.s );
  for( size_t x=0; x<out->w; ++x )
    cimpl_memcpy(out->data+x*out->h, in.data+x*out->h+zIndx*in.h*in.w, out->h*sizeof(float) );
}

void cimpl_spaceConvImgTemplate( cimpl_img const img1, cimpl_img const t, cimpl_img * const out ){
  assert( out->h == img1.h );  assert( out->w == img1.w );
  assert( t.h % 2 == 1 );
  assert( t.w % 2 == 1 );

  float tmp;
  long hh = floorf( t.h / 2 );
  long hw = floorf( t.w / 2 );

  for( long x=0; x<out->w; ++x ){
    for( long y=0; y<out->h; ++y ){
      tmp = 0;

      for( long tx=-hw; tx <= hw; ++tx ){
        if( x+tx < 0 || x+tx > out->w ) continue;

        for( long ty=-hh; ty <= hh; ++ty ){
          if( y+ty < 0 || y+ty > out->h ) continue;

          tmp += img1.data[(y+ty)+(x+tx)*img1.w] * t.data[(ty+hh)+(tx+hw)*t.w];
      } }

      out->data[y+x*out->w] = tmp;
  } }
}

void cimpl_sqrtImg( cimpl_img const in, cimpl_img * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int)( in.nPix / 8 );
  __m256* ptr = (__m256*)in.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 8)
    _mm256_store_ps(outData, _mm256_sqrt_ps(*ptr));

  for( int i=0; i < in.nPix-(8*simdIters); ++i )
    out->data[i+8*simdIters] = sqrt( in.data[i+8*simdIters] );

#else

  int simdIters = (int)( in.nPix / 4 );
  __m128* ptr = (__m128*)in.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 4)
    _mm_store_ps(outData, _mm_sqrt_ps(*ptr));

  for( int i=0; i < in.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = sqrt( in.data[i+4*simdIters] );

#endif

#else
  for( size_t i=0; i<in.nPix; ++i )
    out->data[i] = sqrt( in.data[i] );
#endif
}

void cimpl_sqrtVol( cimpl_vol const in, cimpl_vol * const out ){
  assert( out->h == in.h );  assert( out->w == in.w );  assert( out->s == in.s );
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  int simdIters = (int)( in.nVox / 8 );
  __m256* ptr = (__m256*)in.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 8)
    _mm256_store_ps(outData, _mm256_sqrt_ps(*ptr));

  for( int i=0; i < in.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = sqrt( in.data[i+8*simdIters] );

#else

  int simdIters = (int)( in.nVox / 4 );
  __m128* ptr = (__m128*) in.data;

  for(size_t i = 0; i<simdIters; ++i, ++ptr, outData += 4)
    _mm_store_ps( outData, _mm_sqrt_ps(*ptr) );

  for( int i=0; i < in.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = sqrt( in.data[i+4*simdIters] );

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<in.nVox; ++i )
    out->data[i] = sqrt( in.data[i] );

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_subImg( cimpl_img const in, size_t const h1, size_t const w1,
  cimpl_img * const out ){
  assert( h1+out->h < in.h );  assert( w1+out->w < in.w );

  for( size_t x=0; x<out->w; ++x )
    cimpl_memcpy( out->data+x*out->h, in.data+w1+(h1+x)*in.h, out->h*sizeof(float) );
}

void cimpl_subtractCmpImgs( cimpl_cmpImg const img1, cimpl_cmpImg const img2,
  cimpl_cmpImg * const out ){
  assert( out->h == img1.h );  assert( out->w == img1.w );
  assert( out->h == img2.h );  assert( out->w == img2.w );
#ifndef CIMPL_DONT_SIMD
  size_t i;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = img1.nPix / 4;

  __m256* l = (__m256*) img1.data;
  __m256* r = (__m256*) img2.data;
  float* outData = (float*) out->data;
  
  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=8 )
    _mm256_store_ps(outData, _mm256_sub_ps(*l, *r));

  for( i=0; i < img1.nPix-(2*simdIters); ++i )
    out->data[i+2*simdIters] = img1.data[i+2*simdIters] - img2.data[i+2*simdIters];

#else  // #ifndef CIMPL_DONT_AVX

  size_t simdIters = img1.nPix / 2;

  __m128* l = (__m128*) img1.data;
  __m128* r = (__m128*) img2.data;
  float* outData = (float*) out->data;

  for( i=0; i<simdIters; ++i, ++l, ++r, outData+=4 )
    _mm_store_ps(outData, _mm_sub_ps(*l, *r));

  for( i=0; i < img1.nPix-(2*simdIters); ++i )
    out->data[i+2*simdIters] = img1.data[i+2*simdIters] - img2.data[i+2*simdIters];

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<img1.nPix; ++i )
    out->data[i] = img1.data[i] - img2.data[i];

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_subtractImgFromScalar( cimpl_img const in, float const scalar, cimpl_img * const out ){
  assert( out->w == in.w );  assert( out->h == in.h );
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  const __m256 neg1 = _mm256_set1_ps(-1);
  const __m256 s = _mm256_set1_ps(scalar);
  int simdIters = (int)( in.nPix / 8 );
  __m256* inData = (__m256*) in.data;

  for(size_t i = 0; i<simdIters; ++i, ++inData, outData += 8)
    _mm256_store_ps( outData, _mm256_add_ps( s, _mm256_mul_ps(*inData, neg1)) );

  for( int i=0; i < (in.h*in.w)-(8*simdIters); ++i )
    out->data[i+8*simdIters] = scalar - in.data[i+8*simdIters];

#else

  const __m128 neg1 = _mm_set1_ps(-1);
  const __m128 s = _mm_set1_ps(scalar);
  int simdIters = (int)( in.nPix / 4 );
  __m128* inData = (__m128*) in.data;

  for(size_t i = 0; i<simdIters; ++i, ++inData, outData += 4)
    _mm_store_ps( outData, _mm_add_ps( s, _mm_mul_ps(*inData, neg1)) );

  for( int i=0; i < in.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = scalar - in.data[i+4*simdIters];

#endif  // #ifdef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<in.nPix; ++i )
    out->data[i] = scalar - in.data[i];

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_subtractImgs( cimpl_img const img1, cimpl_img const img2, cimpl_img * const out ){
  assert( out->w == img1.w );  assert( out->h == img1.h );
  assert( out->w == img2.w );  assert( out->h == img2.h );
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = img1.nPix / 8;

  __m256* l = (__m256*) img1.data;
  __m256* r = (__m256*) img2.data;

  for( size_t i=0; i<simdIters; ++i, ++l, ++r, outData+=8 )
    _mm256_store_ps(outData, _mm256_sub_ps(*l, *r));

  for( int i=0; i < img1.nPix-(8*simdIters); ++i )
    out->data[i+8*simdIters] = img1.data[i+8*simdIters] - img2.data[i+8*simdIters];

#else  // #ifndef CIMPL_DONT_AVX

  unsigned long simdIters = img1.nPix / 4;

  __m128* l = (__m128*) img1.data;
  __m128* r = (__m128*) img2.data;

  for( size_t i=0; i<simdIters; ++i, ++l, ++r, outData+=4 )
    _mm_store_ps(outData, _mm_sub_ps(*l, *r));

  for( int i=0; i < img1.nPix-(4*simdIters); ++i )
    out->data[i+4*simdIters] = img1.data[i+4*simdIters] - img2.data[i+4*simdIters];

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  for( int i=0; i<img1.nPix; ++i)
    out->data[i] = img1.data[i] - img2.data[i];

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_subtractVolFromScalar( cimpl_vol const in, float const scalar, cimpl_vol * const out ){
  assert( out->w == in.w );  assert( out->h == in.h );
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  const __m256 neg1 = _mm256_set1_ps(-1);
  const __m256 s = _mm256_set1_ps(scalar);
  int simdIters = (int)( in.nVox / 8 );
  __m256* inData = (__m256*) in.data;

  for(size_t i = 0; i<simdIters; ++i, ++inData, outData += 8)
    _mm256_store_ps( outData, _mm256_add_ps( s, _mm256_mul_ps(*inData, neg1)) );

  for( int i=0; i < in.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = scalar - in.data[i+8*simdIters];

#else

  const __m128 neg1 = _mm_set1_ps(-1);
  const __m128 s = _mm_set1_ps(scalar);
  int simdIters = (int)( in.nVox / 4 );
  __m128* inData = (__m128*) in.data;

  for(size_t i = 0; i<simdIters; ++i, ++inData, outData += 4)
    _mm_store_ps( outData, _mm_add_ps( s, _mm_mul_ps(*inData, neg1)) );

  for( int i=0; i < in.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = scalar - in.data[i+4*simdIters];

#endif  // #ifdef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<in.nVox; ++i )
    out->data[i] = scalar - in.data[i];

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_subtractVols( cimpl_vol const vol1, cimpl_vol const vol2, cimpl_vol * const out ){
  assert( out->h == vol1.h );  assert( out->w == vol1.w );  assert( out->s == vol1.s );
  assert( out->h == vol2.h );  assert( out->w == vol2.w );  assert( out->s == vol2.s );
#ifndef CIMPL_DONT_SIMD
  float* outData = out->data;

#ifndef CIMPL_DONT_AVX

  size_t simdIters = vol1.nVox / 8;

  __m256* l = (__m256*) vol1.data;
  __m256* r = (__m256*) vol2.data;

  for( size_t i=0; i<simdIters; ++i, ++l, ++r, outData+=8 )
    _mm256_store_ps( outData, _mm256_sub_ps(*l, *r) );

  for( int i=0; i < vol1.nVox-(8*simdIters); ++i )
    out->data[i+8*simdIters] = vol1.data[i+8*simdIters] - vol2.data[i+8*simdIters];

#else  // #ifndef CIMPL_DONT_AVX

  size_t simdIters = vol1.nVox / 4;

  __m128* l = (__m128*) vol1.data;
  __m128* r = (__m128*) vol2.data;

  for( size_t i=0; i<simdIters; ++i, ++l, ++r, outData+=4 )
    _mm_store_ps(outData, _mm_sub_ps(*l, *r));

  for( int i=0; i < vol1.nVox-(4*simdIters); ++i )
    out->data[i+4*simdIters] = vol1.data[i+4*simdIters] - vol2.data[i+4*simdIters];

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  for( int i=0; i<vol1.nVox; ++i )
    out->data[i] = vol1.data[i] - vol2.data[i];

#endif  // #ifndef CIMPL_DONT_SIMD
}

void cimpl_subtractScalarFromImg( cimpl_img const in, float const scalar, cimpl_img * const out ){
  cimpl_addScalar2Img( -scalar, in, out );
}

void cimpl_subtractScalarFromVol( cimpl_vol const in, float const scalar, cimpl_vol * const out ){
  cimpl_addScalar2Vol( -scalar, in, out );
}

float cimpl_sumImg( cimpl_img const * const in ){

  float out;

#ifndef CIMPL_DONT_SIMD
#ifndef CIMPL_DONT_AVX

  size_t nIters = in->nPix / 8;
  
  __m256 sum = _mm256_set1_ps(0.0f);
  for (size_t i=0; i < nIters; ++i)
    sum = _mm256_add_ps(sum, _mm256_load_ps( in->data + 8*i ) );

  for( int i=0; i<4; ++i )
    sum = _mm256_hadd_ps(sum, sum);
  _mm_store_ss( &out, _mm256_extractf128_ps(sum, 0) );

  for( size_t i=8*nIters; i<in->nPix; ++i )
    out += in->data[i];

#else // #ifndef CIMPL_DONT_AVX

  size_t nIters = in->nPix / 4;

  __m128 sum = _mm_set1_ps( 0.0f );
  for (size_t i=0; i < nIters; ++i)
    sum = _mm_add_ps( sum, _mm_load_ps( in->data + 4*i ) );

  sum = _mm_hadd_ps( sum, sum );
  sum = _mm_hadd_ps( sum, sum );
  _mm_store_ss( &out, sum );

  for( size_t i=4*nIters; i<in->nPix; ++i )
    out += in->data[i];

#endif  // #ifndef CIMPL_DONT_AVX

#else

  for( int i=0; i<in->nPix; ++i)
    out += in->data[i];

#endif  // #ifndef CIMPL_DONT_SIMD
  return out;
}

float cimpl_sumVol( cimpl_vol const * const in ){
  float out=0;
  for( int i=0; i<in->nVox; ++i)
    out += in->data[i];
  return out;
}

void cimpl_transposeImg( cimpl_img const in, cimpl_img * const out ){
  assert( out->h == in.w );  assert( out->w == in.h );
  for( size_t x=0; x<out->w; ++x ){
    for( size_t y=0; y<out->h; ++y )
      out->data[y+x*out->h] = in.data[x+y*in.h];
  }
}

//void cimpl_xcorr( cimpl_img const img, cimpl_img const template, cimpl_img * const out ){
//  // Creates a cross correlation result the same size as as img assuming zero boundary
//  // conditions
//  assert( out->h == img.h );
//  assert( out->w == img.w );
//  assert( template.h % 2 == 1 );
//  assert( template.w % 2 == 1 );
//
//  size_t leftX, rightX, lowY, highY;
//  float htw = floorf( template.w / 2 );
//  float hth = floorf( template.h / 2 );
//
//  for( size_t x=0; x<out->w; ++x ){
//    for( size_t y=0; y<out->h; ++y ){
//    }
//  }
//}

void cimpl_zeroCmpImg( cimpl_cmpImg * const img  ){
  // Sets all values of complex image to 0
  assert( img->data != NULL );
#ifndef CIMPL_DONT_SIMD
  float* data = (float*) img->data;

#ifndef CIMPL_DONT_AVX

  __m256 sseZeros = _mm256_setzero_ps();

  size_t simdIters = img->nPix / 4;
  for( size_t i=0; i<simdIters; ++i, data+=8 )
    _mm256_store_ps( data, sseZeros );

  for( int i=0; i<img->nPix-(4*simdIters); ++i )
    img->data[i] = 0;

#else  // #ifndef CIMPL_DONT_AVX

  __m128 sseZeros = _mm_setzero_ps();

  size_t simdIters = img->nPix / 2;
  for( size_t i=0; i<simdIters; ++i, data+=4 )
    _mm_store_ps( data, sseZeros );

  for( int i=0; i<img->nPix-(2*simdIters); ++i )
    img->data[i] = 0;

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<img->nPix; ++i )
    img->data[i] = 0;

#endif // #ifndef CIMPL_DONT_SIMD
}

void cimpl_zeroImg( cimpl_img * const img  ){
  // Sets all values of image to 0
  assert( img->data != NULL );
#ifndef CIMPL_DONT_SIMD
  float* data = img->data;

#ifndef CIMPL_DONT_AVX

  __m256 sseZeros = _mm256_setzero_ps();

  size_t simdIters = img->nPix / 8;
  for( size_t i=0; i<simdIters; ++i, data+=8 )
    _mm256_store_ps( data, sseZeros );

  for( int i=0; i<img->nPix-(8*simdIters); ++i )
    img->data[i] = 0;

#else  // #ifndef CIMPL_DONT_AVX

  __m128 sseZeros = _mm_setzero_ps();

  size_t simdIters = img->nPix / 4;
  for( size_t i=0; i<simdIters; ++i, data+=4 )
    _mm_store_ps( data, sseZeros );

  for( int i=0; i<img->nPix-(4*simdIters); ++i )
    img->data[i] = 0;

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<img->nPix; ++i )
    img->data[i] = 0;

#endif //  #ifndef CIMPL_DONT_SIMD
}

void cimpl_zeroVol( cimpl_vol * const vol  ){
  // Sets all values of volume to 0
  assert( vol->data != NULL );
#ifndef CIMPL_DONT_SIMD
  float* data = vol->data;

#ifndef CIMPL_DONT_AVX

  __m256 sseZeros = _mm256_setzero_ps();

  size_t simdIters = vol->nVox / 8;
  for( size_t i=0; i<simdIters; ++i, data+=8 )
    _mm256_store_ps( data, sseZeros );

  for( int i=0; i<(vol->h*vol->w)-(8*simdIters); ++i )
    vol->data[i] = 0;

#else  // #ifndef CIMPL_DONT_AVX

  __m128 sseZeros = _mm_setzero_ps();

  size_t simdIters = vol->nVox / 4;
  for( size_t i=0; i<simdIters; ++i, data+=4 )
    _mm_store_ps( data, sseZeros );

  for( int i=0; i<vol->nVox-(4*simdIters); ++i )
    vol->data[i] = 0;

#endif  // #ifndef CIMPL_DONT_AVX

#else  // #ifndef CIMPL_DONT_SIMD

  for( size_t i=0; i<vol->nPix; ++i )
    vol->data[i] = 0;

#endif // #ifndef CIMPL_DONT_SIMD
}

