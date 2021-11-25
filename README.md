# OpenCV Zoo (C++ version)

This is the C++ version of OpenCV Zoo, which currently provides a demo and benchmark on [YuNet](https://github.com/opencv/opencv_zoo/tree/master/models/face_detection_yunet).

## Build for RISC-V arch

### Xuantie C906 CPU (Allwinner D1 board)

The Allwinner D1 development board [[CN]](https://d1.docs.aw-ol.com/) [[EN]](https://d1.docs.aw-ol.com/en/) is equiped with the Xuantie C906 CPU. [Xuantie C906](https://www.t-head.cn/product/C906?spm=a2ouz.12986968.0.0.7bfc1384XPXDMm) is a RISC-V architecture CPU with RVV intrinsic v0.7.1 support, which is developed by T-Head (平头哥半导体有限公司).

Steps to build OpenCV Zoo for Xuantie C906 CPU (Allwinner D1 board):
1. Follow [this wiki on OpenCV](https://github.com/opencv/opencv/wiki/OpenCV-RISC-V) to build OpenCV with RVV support. If you have trouble downloading toolchains, visit and download from T-Head: https://occ.t-head.cn/community/download?id=3927429448189939712 (toolchain of version 1.12.0 is recommanded).
2. Cross compile this repo using the downloaded toolchain:
    ```shell
    cd $opencv_zoo_cpp
    mkdir build && cd build
    cmake -D CMAKE_BUILD_TYPE=RELEASE \
          -D CMAKE_PREFIX_PATH=/path/to/opencv_riscv_install \
          -D CMAKE_TOOLCHAIN_FILE=/path/to/opencv/platforms/linux/riscv64-071-gcc.toolchain.cmake ..
    make # After this you will get two binary files: detect, benchmark
    ```
3. Download YuNet from [OpenCV Zoo](https://github.com/opencv/opencv_zoo/blob/master/models/face_detection_yunet/face_detection_yunet_2021sep.onnx).
4. Send `detect`, `benchmark`, the model `face_detection_yunet_2021sep.onnx` and some test images to the board using adb:
    ```shell
    adb push detect /root/
    adb push benchmark /root/
    adb push face_detection_yunet_2021sep.onnx /root/
    adb push /path/to/image /root/
    ```
5. Run `detect` and `benchmark` ***on the board***:
    ```shell
    # Shell on the board
    cd /root/
    ./detect -i=<image_name> -m=face_detection_yunet_2021sep.onnx # Run ./detect to get help
    ./benchmark -i=<image_name> -m=face_detection_yunet_2021sep.onnx # Run ./benchmark to get help
    ```