# lucien_wm
A simple compositing window manager written in c++ and opengl.

# Prerequisites
- make
- cmake
- clang

# Build
```
git clone https://github.com/LinArcX/lucien_wm
cd lucien_wm
mkdir -p output/cmake output/debug output/release
cd output/cmake/
cmake -DCMAKE_BUILD_TYPE=Release ../..
make -j8
```

## Test with Xephyr
`./run.sh`

## License
![License](https://img.shields.io/github/license/LinArcX/lucien_wm.svg)
