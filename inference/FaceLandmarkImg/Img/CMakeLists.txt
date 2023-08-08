# Local libraries
include_directories(${LandmarkDetector_SOURCE_DIR}/include)


SET(SOURCE
        FaceAnalyzerImgsEXE.cpp
        )

add_executable(FaceAnalyzerImgs ${SOURCE})

target_link_libraries(FaceAnalyzerImgs LandmarkDetector)
target_link_libraries(FaceAnalyzerImgs FaceAnalyser)
target_link_libraries(FaceAnalyzerImgs GazeAnalyser)
target_link_libraries(FaceAnalyzerImgs Utilities)

install (TARGETS FaceAnalyzerImgs DESTINATION bin)
