//
// Created by JerryZhang on 2023/7/20.
//

#include "FaceAnalyzerForImgs.h"

int main(int argc, char **argv){
    char* input_dir = "/home/keyan/workspace/DepressionRec/OpenFace/samples";
    char* output_dir = "/home/keyan/workspace/DepressionRec/OpenFace/output";
    ProcessImgs(input_dir,output_dir, false);
    return 0;
}

