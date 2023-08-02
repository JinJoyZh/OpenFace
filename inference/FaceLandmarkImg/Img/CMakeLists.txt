# Local libraries
include_directories(${LandmarkDetector_SOURCE_DIR}/include)


SET(SOURCE
        FaceAnalyzerForImgsExe.cpp
        )

add_executable(FaceAnalyzerForImgs ${SOURCE})

target_link_libraries(FaceAnalyzerForImgs LandmarkDetector)
target_link_libraries(FaceAnalyzerForImgs FaceAnalyser)
target_link_libraries(FaceAnalyzerForImgs GazeAnalyser)
target_link_libraries(FaceAnalyzerForImgs Utilities)

install (TARGETS FaceAnalyzerForImgs DESTINATION bin)
