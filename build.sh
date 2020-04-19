if [[ "$*" == *"clean"* ]]
then
	cd build
	make clean
	exit 0
fi

if [[ "$*" == *"rebuild"* ]]
then
	cd build
	make clean
	cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++ -DCMAKE_CC_COMPILER=/usr/bin/gcc ..
	make -j8
	exit 1
fi

mkdir -p build
cd build
make clean
cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++ -DCMAKE_CC_COMPILER=/usr/bin/gcc ..
make -j8
exit 100