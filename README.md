# lucien_wm
A simple compositing window manager written in c++ and opengl.

# Prerequisites
- make
- cmake
- clang
- pkg-config

# Build
```
git clone https://github.com/LinArcX/lucien_wm
cd lucien_wm
mkdir -p output/cmake output/debug output/release
cd output/cmake/
cmake -DCMAKE_BUILD_TYPE=Release ../..
make -j8
```

And finally run the program:

`../release/lucien_wm`


## Test
`./output/x64/release/lucien_wm -output ./assets/smaller.png -input ./assets/original.png -ratio 0.4 -quality 90 -algorithm "bicubic"`

## Inspiration
1. [opencl-resizer](https://github.com/vencabkk/opencl-resizer)

## License
![License](https://img.shields.io/github/license/LinArcX/lucien_wm.svg)
