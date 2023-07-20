
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inference/FaceLandmarkImg)

add_executable(testexe test.cpp)

target_link_libraries(testexe FaceAnalyzerForImgs)

install (TARGETS testexe DESTINATION bin)
