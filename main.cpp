#include <filesystem>
#include <iostream>
#include <vector>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/core/mat.hpp>
#include <opencv4/opencv2/core/types.hpp>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/core/matx.hpp>

int main() {
    std::string path = "../resources/hand_only.MP4";
    cv::VideoCapture cap(path);

    while (true) {
        cv::Mat frame;
        cap.read(frame);

        cv::imshow("image", frame);
        if (cv::waitKey(30) == 27)
            return 0;
    }

    return 0;
}