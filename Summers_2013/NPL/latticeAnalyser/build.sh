#export PKG_CONFIG_PATH='/media/atul/a4c78183-78c2-4a82-9985-21937168d57f/Summer13/opencv/release2/unix-install'
# export LD_LIBRARY_PATH=/usr/local/src/opencv-2.4.5/release/lib
export LD_LIBRARY_PATH=/usr/local/src/opencv-2.4.5/opencvFFMPEGbuild/lib

g++ -std=c++0x -pthread -ggdb `pkg-config --cflags opencv` -o latticeAnalyser latticeAnalyser.cpp `pkg-config --libs opencv`
