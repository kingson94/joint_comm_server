if [[ "$*" == *"-clean"* ]]
then
	cd build
	make clean
	exit 0
fi

if [[ "$*" == *"-rebuild"* ]]
then
	echo "[1/3] Cleaning current files"
	cd build
	make clean
	echo "[2/3] Building project"
	cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++ -DCMAKE_CC_COMPILER=/usr/bin/gcc ..
	make -j8
	echo "[3/3] Build finished"
	exit 0
fi

mkdir -p build
cd build
make clean
cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++ -DCMAKE_CC_COMPILER=/usr/bin/gcc ..
make -j8