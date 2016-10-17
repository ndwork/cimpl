//
//  main.c
//  cimpl
//
//  Created by Nicholas Dwork on 9/18/16.
//  Copyright © 2016 Nicholas Dwork.
//

#include <stdio.h>
#include "cimpl.h"


int main(int argc, const char * argv[]) {

  cimpl_imgf myImg, subImg, subImg2;
  cimpl_imgf tmp;

  // mallocImg
  printf("Testing mallocImg \n");
  myImg = cimpl_mallocImg( 5, 4 );
  for( unsigned int i=0; i<myImg.w*myImg.h; ++i ){
    myImg.data[i] = i;
  }

  // subImg
  printf("Testing subImg and display \n");
  subImg = cimpl_extractSubImg(myImg,0,3,0,2);
  cimpl_displayImg(subImg);

  // add
  printf("Testing add \n");
  subImg2 = cimpl_extractSubImg(myImg,1,4,1,3);
  tmp = cimpl_mallocImg( subImg2.w, subImg2.h );
  cimpl_addImgs( subImg, subImg2, &tmp );
  cimpl_displayImg(tmp);
  cimpl_freeImg(&tmp);

  // circShift
  printf("Testing circshift \n");
  tmp = cimpl_mallocImg( myImg.w, myImg.h );
  cimpl_circShiftImg(myImg, 1, -1, &tmp);
  cimpl_circShiftImg(myImg, -1, 1, &tmp);
  cimpl_displayImg(tmp);
  cimpl_freeImg(&tmp);
  
  // free
  printf("Testing free \n");
  cimpl_freeImg( &myImg );
  cimpl_freeImg( &subImg );
  cimpl_freeImg( &subImg2 );
  return 0;
}
