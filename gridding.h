//
//  gridding.h
//  cimpl
//
//  Created by Nicholas Dwork starting on 10/21/16.
//  Copyright © 2016 Nicholas Dwork. All rights reserved.
//

#ifndef gridding_h
#define gridding_h

#ifdef __cplusplus
extern "C" {
#endif

void applyC_2D( size_t nF, float const * const realF, float const * const imagF,
  size_t nTraj, float const * const realTraj, float const * const imagTraj,
  size_t const sX, size_t const sY,
  size_t const nK, float const * const kCy, float const * const kCx,
  float const * const Cy, float const * const Cx,
  cimpl_img * const realGridded, cimpl_img * const imagGridded );


#ifdef __cplusplus
}
#endif

#endif /* gridding_h */
