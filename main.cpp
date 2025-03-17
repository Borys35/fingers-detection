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

        // Rotate frame
        cv::Point2f center(frame.cols / 2, frame.rows / 2);
        cv::Mat rotMatrix = cv::getRotationMatrix2D(center, 180, 1.0);
        cv::warpAffine(frame, frame, rotMatrix, frame.size());

        // Resize frame
        cv::resize(frame, frame, cv::Size(), 0.5, 0.5);

        // Display frame
        cv::imshow("image", frame);
        if (cv::waitKey(30) == 27)
            return 0;
    }

    return 0;
}