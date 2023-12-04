#include <opencv2/opencv.hpp>
#include <vector>

const int HALF_PATCH_SIZE = 15;

int main() {
    cv::Mat image(31, 31, CV_8UC1, cv::Scalar(0));
    std::vector<int> umax;
    umax.resize(HALF_PATCH_SIZE + 1);

    int v, v0;
    int vmax = cvFloor(HALF_PATCH_SIZE * sqrt(2.f) / 2 + 1);
    int vmin = cvCeil(HALF_PATCH_SIZE * sqrt(2.f) / 2);

    const double hp2 = HALF_PATCH_SIZE * HALF_PATCH_SIZE;

    for (v = 0; v <= vmax; ++v)
        umax[v] = cvRound(sqrt(hp2 - v * v));

    for (v = HALF_PATCH_SIZE, v0 = 0; v >= vmin; --v) {
        while (umax[v0] == umax[v0 + 1])
            ++v0;
        umax[v] = v0;
        ++v0;
    }
    for (int deltaRow = 0; deltaRow < HALF_PATCH_SIZE; ++deltaRow) {
        image.at<uchar>(15 + deltaRow, 15 + umax[0]) = 1;
    }
    cv::imshow("Circle Test", image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
