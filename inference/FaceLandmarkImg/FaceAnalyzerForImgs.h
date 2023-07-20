//
// Created by JerryZhang on 2023/7/19.
//

#ifndef OPENFACE_FACEANALYZERFORIMGS_H
#define OPENFACE_FACEANALYZERFORIMGS_H

#ifdef __cplusplus
extern "C" {
#endif


namespace FaceAnalysis{

    int ProcessImgs(char const * input_dir, char const * output_dir, bool log_mode);

}

#ifdef __cplusplus
}
#endif

#endif
