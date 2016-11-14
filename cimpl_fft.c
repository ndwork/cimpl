//
//  cimpl_fft.c
//  cimpl
//
//  Created by Nicholas Dwork on 11/12/16.
//  Copyright © 2016 Nicholas Dwork. All rights reserved.
//

#include <assert.h>
#include "cimpl_fft.h"


#define CIMPL_USE_FFTW

#ifdef CIMPL_USE_FFTW
#include <complex.h>
#include <fftw3.h>

void cimpl_prefftCmpImg( cimpl_cmpImg const fftIn, cimpl_cmpImg const fftOut, void* out ){
  fftwf_plan fftPlan;
  fftPlan = fftwf_plan_dft_2d( (int) fftIn.h, (int) fftIn.w,
    (fftwf_complex*) fftIn.data, (fftwf_complex*) fftOut.data,
    FFTW_FORWARD, FFTW_MEASURE );
}

void cimpl_prefftCmpVol( cimpl_cmpVol const fftIn, cimpl_cmpVol const fftOut, void* out ){
  fftwf_plan fftPlan;
  fftPlan = fftwf_plan_dft_3d( (int) fftIn.h, (int) fftIn.w, (int) fftIn.s,
    (fftwf_complex*) fftIn.data, (fftwf_complex*) fftOut.data,
    FFTW_FORWARD, FFTW_MEASURE );
}

void cimpl_fftCmpImg( void* in ){
  fftwf_plan* tmp = in;
  fftwf_execute( *tmp );
}

void cimpl_fftCmpVol( void* in ){
  fftwf_plan* tmp = in;
  fftwf_execute( *tmp );
}

void cimpl_postfftCmpImg( void* in ){
  fftwf_plan* tmp = in;
  fftwf_destroy_plan( *tmp );
}

void cimpl_postfftCmpVol( void* in ){
  fftwf_plan* tmp = in;
  fftwf_destroy_plan( *tmp );
}


#endif  /* CIMPL_USE_FFTW */
