//
//  myGrid.cpp
//  gridExample
//
//  Created by Eric Gunther on 5/29/14.
//
//

#include "myGrid.h"

// Pass the parameters of your grid to the baseclass contructor
// Then, you can create a new instance of your grid without worrying about the paramater args
// eg myGrid grid; or myGrid *grid = new myGrid();
myGrid::myGrid():ofxGridSystem(1280,  //float iPageWidth,
                               720,   //float iPageHeight,
                               8,     //int iNumColumns,
                               16,    //float iBaselineLeading,
                               80,    //float iLeft (margin)
                               80,    //float iRight (margin
                               6*16,  //float iHead (top margin, specified as multiple of leading)
                               5*16,  //float iTail (bottom margin, as multiple of leading)
                               1      //int iGutterMult (iGutterMult x iBaselineLeading = gutter width)
                               ){}
