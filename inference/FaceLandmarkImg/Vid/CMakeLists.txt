# Local libraries
include_directories(${LandmarkDetector_SOURCE_DIR}/include)
	
add_executable(FaceAnalyzerVid FaceLandmarkVidEXE.cpp)
target_link_libraries(FaceAnalyzerVid LandmarkDetector)
target_link_libraries(FaceAnalyzerVid FaceAnalyser)
target_link_libraries(FaceAnalyzerVid GazeAnalyser)
target_link_libraries(FaceAnalyzerVid Utilities)

install (TARGETS FaceAnalyzerVid DESTINATION bin)