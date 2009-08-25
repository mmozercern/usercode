#include "Model.h"
#include "DataSetHelper.h"
#include "ResultCollection.h"
#include "RooPolynomial.h"
#include <iostream>
#include <sstream>

using namespace RooFit;

void test(char* file1,char* file2){
   RooCmdArg arg1=Minos(kFALSE);  
   RooCmdArg arg2=Hesse(kFALSE);
   RooCmdArg arg3=PrintLevel(-1);
   RooCmdArg arg4=Save(kTRUE);
   RooCmdArg arg5=Extended(kTRUE);
   RooCmdArg arg6=Optimize(kFALSE);

   test(0,&arg1,&arg2,&arg3,&arg4,&arg5,&arg6,file1,file2);

}

void rundm(int it,float peak,float ne, float nmu){
   RooCmdArg arg1=Minos(kFALSE);  
   RooCmdArg arg2=Hesse(kFALSE);
   RooCmdArg arg3=PrintLevel(-1);
   RooCmdArg arg4=Save(kTRUE);
   RooCmdArg arg5=Extended(kTRUE);
   RooCmdArg arg6=Optimize(kFALSE);

   test(it,&arg1,&arg2,&arg3,&arg4,&arg5,&arg6,peak,ne,nmu);

}
