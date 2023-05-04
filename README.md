A classical CV detector approach to the RoboMaster rune game CV detection problem. [Website](https://matthewmarnold.github.io/CSE-455-Final-Project-Website/).

## Dependencies:

Designed and tested on Ubuntu 20.04 OS.

CMake/gcc/g++ (probably some other gcc/g++ related tools I'm forgetting).

    sudo apt-get install g++ gcc cmake

OpenCV (You must install opencv from scratch)

- Clone opencv and opencv_contrib:

    git clone https://github.com/opencv/opencv.git git clone
    https://github.com/opencv/opencv_contrib.git

- Follow [this](https://stackoverflow.com/questions/31040746/cant-open-video-using-opencv) stack
  overflow post on how to install opencv with MMPEG CODAC support (so you can play videos in
  opencv).

## Running the detector in C++

```bash
mkdir build
cd build
cmake ..
./rune_game <-i|-v> [path/to/masterConfiguration.yml]
```

<!-- ## Running rune game detector as python module

While this code is written in C++, there is support to interact with it in python. To do this, you
must rebuild opencv with our rune game module appended on as an extra cmake flag. In 


`path/to/masterConfiguration.yml` must be a path relative to the path you are running `./rune_game`
in. If no path is passed, it is assumed you are running `./rune_game` from `build`.

`-DOPENCV_EXTRA_MODULES_PATH=<some-path>/cse-455-classical-cv/` -->
<!-- 
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D BUILD_NEW_PYTHON_SUPPORT=ON
-D WITH_QT=OFF -D WITH_V4L=ON -D CMAKE_SHARED_LINKER_FLAGS=-Wl,-Bsymbolic .. -->

<!-- In case when you need interoperability between native libraries hidden behind python or java I'd
recommend to wrap your library into personal OpenCV module and build them together. Different
compilers, flags or standard libraries can cause more problems than benefits.

For example, you have a library Foo which uses OpenCV and you want to call it from python, then:

    add minimal module stuff: CMakeLists.txt, directory structure (Foo/ocv/modules/foo)
    add wrapper C++ headers with CV_EXPORTS_W directives to <module>/include/opencv2 folder
    add manual converters to the <module>/misc/python if needed
    build OpenCV with your module and python support: cmake -DOPENCV_EXTRA_MODULES_PATH=<some-path>/Foo/ocv ../opencv && make

BTW, related update to the python wrapping process has been introduced recently: #7056 - now each
module can provide manual-written conversion routines in <module>/misc/python folder.

I did not try the described scheme and could miss something, but developing standalone automatic
universal C++-to-python wrapping mechanism from hdr_parser.py and gen2.py looks like quite hard
task. -->
