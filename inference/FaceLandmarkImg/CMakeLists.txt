# Local libraries
include_directories(${LandmarkDetector_SOURCE_DIR}/include)

add_library(FaceAnalyzerForImgs FaceAnalyzerForImgs.cpp)

target_include_directories(FaceAnalyzerForImgs PUBLIC ./FaceAnalyzerForImgs.h)

target_link_libraries(FaceAnalyzerForImgs LandmarkDetector)
target_link_libraries(FaceAnalyzerForImgs FaceAnalyser)
target_link_libraries(FaceAnalyzerForImgs GazeAnalyser)
target_link_libraries(FaceAnalyzerForImgs Utilities)

install (TARGETS FaceAnalyzerForImgs DESTINATION bin)