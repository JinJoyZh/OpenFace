export OMP_NUM_THREADS=1
export VECLIB_MAXIMUM_THREADS=1
cd build
rm -rf ./*
cmake -D CMAKE_CXX_COMPILER=g++-8 -D CMAKE_C_COMPILER=gcc-8 -D CMAKE_BUILD_TYPE=RELEASE ..
make -j8
cp -r  /home/zjy/workspace/DepressionRec/models/OpenFace/* /home/zjy/workspace/DepressionRec/alg/OpenFace/build/bin/model/patch_experts/
./bin/FaceLandmarkVid -f /home/zjy/workspace/DepressionRec/alg/OpenFace/samples/2015-10-15-15-14.avi -out_dir /home/zjy/workspace/DepressionRec/alg/OpenFace/build/output/