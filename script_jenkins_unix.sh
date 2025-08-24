rm -rf build
mkdir build
cd build
cmake ..
NBRP=$(cat /proc/cpuinfo | grep processor | wc -l)
make -j$NBRP
make install
cd ..
chmod +x ./auxiliary-binaries/siftpp_tgi.LINUX
chmod +x ./auxiliary-binaries/ann_mec_filtre.LINUX

