#include <algorithm>
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

void find_fingers(cv::Mat& frame) {
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    cv::Canny(gray, gray, 70, 85);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(gray, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<std::vector<cv::Point>> hull(contours.size());
    std::vector<std::vector<cv::Vec4i>> defects(contours.size());

    if (!contours.empty()) {
        int max_index = 0;
        double max_area = 0.0;
        for (int i = 0; i < contours.size(); i++) {
            double area = cv::contourArea(contours[i]);
            if (area > max_area) {
                max_area = area;
                max_index = i;
            }
        }

        std::vector<cv::Point> hand_contour = contours[max_index];
        std::vector<int> hull;
        cv::convexHull(hand_contour, hull, false, false);
        std::ranges::sort(hull);
        if (hull.size() > 3) {
            std::vector<cv::Vec4i> defects;
            cv::convexityDefects(hand_contour, hull, defects);

            int finger_count = 0;
            for (const auto& defect : defects) {
                const cv::Point start = hand_contour[defect[0]];
                const cv::Point end = hand_contour[defect[1]];
                const cv::Point far = hand_contour[defect[2]];
                float depth = defect[3] / 256.0;

                cv::circle(frame, far, 5, cv::Scalar(0, 255, 0), -1);
                cv::line(frame, start, far, cv::Scalar(255, 0, 0), 2);
                cv::line(frame, far, end, cv::Scalar(255, 0, 0), 2);

                // Filter out shallow defects (adjust threshold if needed)
                std::cout << "depth: " << depth << std::endl;
                if (depth > 56) {
                    finger_count += 1;
                }
            }

            putText(frame, "Fingers: " + std::to_string(finger_count), cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0), 2);
        }
    }
}

int threshold1 = 255, threshold2 = 255;

void on_trackbar(int, void*) {
    // Empty function needed for trackbar updates
}

int main() {
    std::string path = "../resources/hand.MP4";
    cv::VideoCapture cap(path);

    cv::namedWindow("fingers", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("threshold 1", "fingers", &threshold1, 255, on_trackbar);
    cv::createTrackbar("threshold 2", "fingers", &threshold2, 255, on_trackbar);

    while (true) {
        cv::Mat frame;
        cap.read(frame);

        if (frame.empty()) {
            break;
        }

        // cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        // cv::Canny(frame, frame, threshold1, threshold2);

        // Rotate frame
        cv::Point2f center(frame.cols / 2, frame.rows / 2);
        cv::Mat rotMatrix = cv::getRotationMatrix2D(center, 180, 1.0);
        cv::warpAffine(frame, frame, rotMatrix, frame.size());

        // Resize frame
        cv::resize(frame, frame, cv::Size(), 0.5, 0.5);

        // Crop frame
        std::cout << frame.cols << " " << frame.rows << std::endl;
        cv::Rect roi(30, 100, 300, 480);
        frame = frame(roi);

        // Find fingers
        find_fingers(frame);

        // Display frame
        cv::imshow("image", frame);
        if (cv::waitKey(30) == 27)
            break;
    }

    return 0;
}