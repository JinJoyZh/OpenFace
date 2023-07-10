# Local libraries
include_directories(${LandmarkDetector_SOURCE_DIR}/include)



add_executable(FaceLandmarkForImg FaceLandmarkForImg.cpp)
target_link_libraries(FaceLandmarkForImg LandmarkDetector)
target_link_libraries(FaceLandmarkForImg FaceAnalyser)
target_link_libraries(FaceLandmarkForImg GazeAnalyser)
target_link_libraries(FaceLandmarkForImg Utilities)

install (TARGETS FaceLandmarkForImg DESTINATION bin)