
default: all

all: gpgpu_pb2.py gpgpu.pb.cc libcecl_pb2.py libcecl.pb.cc libcecl.so

gpgpu_pb2.py: gpgpu.proto
	protoc -I=./ --python_out=./ ./gpgpu.proto

gpgpu.pb.cc: gpgpu.proto
	protoc -I=./ --cpp_out=./ ./gpgpu.proto

libcecl_pb2.py: libcecl.proto
	protoc -I=./ --python_out=./ ./libcecl.proto

libcecl.pb.cc: libcecl.proto
	protoc -I=./ --cpp_out=./ ./libcecl.proto

libcecl.so: libcecl.c libcecl.h 
	${CC} -O3 -fPIC -I/opt/intel/system_studio_2020/opencl/SDK/include libcecl.c -shared -o libcecl.so -Wl,--whole-archive -Wl,--no-whole-archive -lOpenCL -lpapi

gpgpu.pb.o: gpgpu.pb.cc gpgpu.pb.h libcecl.pb.h
	${CXX} -std=c++17 -march=native -O3 -c $< -o $@

libcecl.pb.o: libcecl.pb.cc libcecl.pb.h gpgpu.pb.h
	${CXX} -std=c++17 -march=native -O3 -c $< -o $@

clean:
	rm -f gpgpu_pb2.py  libcecl_pb2.py  gpgpu.pb.cc gpgpu.pb.h libcecl.pb.cc libcecl.pb.h libcecl.so *~ *.o
