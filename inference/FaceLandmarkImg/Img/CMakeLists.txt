# Local libraries
include_directories(${LandmarkDetector_SOURCE_DIR}/include)


SET(SOURCE
        FaceAnalyzerImgsEXE.cpp
        )

add_executable(FaceAnalyzerImg ${SOURCE})

target_link_libraries(FaceAnalyzerImg LandmarkDetector)
target_link_libraries(FaceAnalyzerImg FaceAnalyser)
target_link_libraries(FaceAnalyzerImg GazeAnalyser)
target_link_libraries(FaceAnalyzerImg Utilities)

install (TARGETS FaceAnalyzerImg DESTINATION bin)
