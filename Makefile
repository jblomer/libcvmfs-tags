all: libcvmfs_tags

LIBCVMFS = cvmfs/build/cvmfs/libcvmfs.a
LIBS = -lcrypto -lssl -luuid -lrt


cvmfs:
	git clone https://github.com/cvmfs/cvmfs.git
	cd cvmfs && git checkout libcvmfs-stable

cvmfs/build: | cvmfs
	cd cvmfs && mkdir build

$(LIBCVMFS): | cvmfs/build
	cd cvmfs/build && cmake -DBUILD_CLIENT=no -DBUILD_SERVER=no ../
	cd cvmfs/build && make -j4

libcvmfs_tags: libcvmfs_tags.cc $(LIBCVMFS)
	g++ -O2 -Wall -pthread -o libcvmfs_tags $(LIBS) libcvmfs_tags.cc $(LIBCVMFS)

clean:
	rm -f libcvmfs_tags cvmfschecksum.*


