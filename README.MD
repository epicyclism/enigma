Build for Linux.

Requires g++-9 for the parallel STL implementation, which in turn depends on Intel's TBB.

1. Make g++-9.x default compiler, assuming Ubuntu 19 with g++-8 default, install g++-9 packages
   and set up alternatives,

	sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9 --slave /usr/bin/gcov gcov /usr/bin/gcov-9
	sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 80 --slave /usr/bin/g++ g++ /usr/bin/g++-8 --slave /usr/bin/gcov gcov /usr/bin/gcov-8
	

   Switch back as required.

	sudo update-alternatives --config gcc

   etc.

2. Get Intel TBB from github

   	wget https://github.com/intel/tbb/archive/2019_U9.tar.gz
	tar zxvf 2019_U9.tar.gz
	rm 2019_U9.tar.gz

3. CMake should now work,

	mkdir Release
	cd Release
	cmake -DTBB_ROOT=${HOME}/tbb-2019_U9 -DCMAKE_BUILD_TYPE=Release ..

	
