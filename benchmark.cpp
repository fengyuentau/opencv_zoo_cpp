#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>

#include <iostream>

// using namespace cv;
using namespace std;

int main(int argc, char ** argv)
{
    cv::CommandLineParser parser(argc, argv,
        "{help  h           |            | Print this message.}"
        "{input i           |            | Path to the input image. Omit for detecting on default camera.}"
        "{backend_id        | 0          | Backend to run on. 0: default, 1: Halide, 2: Intel's Inference Engine, 3: OpenCV, 4: VKCOM, 5: CUDA}"
        "{target_id         | 0          | Target to run on. 0: CPU, 1: OpenCL, 2: OpenCL FP16, 3: Myriad, 4: Vulkan, 5: FPGA, 6: CUDA, 7: CUDA FP16, 8: HDDL}"
        "{model m           | yunet.onnx | Path to the model. Download yunet.onnx in https://github.com/ShiqiYu/libfacedetection.train/tree/master/tasks/task1/onnx.}"
        "{score_threshold   | 0.9        | Filter out faces of score < score_threshold.}"
        "{nms_threshold     | 0.3        | Suppress bounding boxes of iou >= nms_threshold.}"
        "{top_k             | 5000       | Keep top_k bounding boxes before NMS.}"
        "{warmup            | 30         | Run given times to warmup before benchmark.}"
        "{repeat            | 10         | Repeat given times for benchmark.}"
    );
    if (argc == 1 || parser.has("help"))
    {
        parser.printMessage();
        return -1;
    }

    cv::String modelPath = parser.get<cv::String>("model");
    int backendId = parser.get<int>("backend_id");
    int targetId = parser.get<int>("target_id");

    float scoreThreshold = parser.get<float>("score_threshold");
    float nmsThreshold = parser.get<float>("nms_threshold");
    int topK = parser.get<int>("top_k");

    int warmup = parser.get<int>("warmup");
    int repeat = parser.get<int>("repeat");

    // Initialize FaceDetectorYN
    cv::Ptr<cv::FaceDetectorYN> detector = cv::FaceDetectorYN::create(modelPath, "", cv::Size(320, 320), scoreThreshold, nmsThreshold, topK, backendId, targetId);

    // If input is an image
    if (parser.has("input"))
    {
        cv::String input = parser.get<cv::String>("input");
        cv::Mat image = cv::imread(input);
        cv::resize(image, image, cv::Size(160, 120));

        detector->setInputSize(image.size());
        cv::Mat faces;

        // warmup
        for (int i = 0; i < warmup; i++)
        {
            detector->detect(image, faces);
        }
        // repeat
        cv::TickMeter tickmeter;
        std::vector<double> times;
        for (int i = 0; i < repeat; i++)
        {
            tickmeter.reset();
            tickmeter.start();
            detector->detect(image, faces);
            tickmeter.stop();
            times.push_back(tickmeter.getTimeMilli());
        }
        // print results (median)
        int l = times.size();
        int mid = int(l / 2);
        if (l % 2 == 0)
        {
            std::cout << (times[mid] + times[mid - 1]) / 2 << std::endl;
        }
        else
        {
            std::cout << times[mid] << std::endl;
        }
    }
    else
    {
        std::cout << "must have input" << std::endl; 
    }
}