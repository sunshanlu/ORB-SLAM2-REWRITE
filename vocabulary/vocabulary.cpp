#include "Config.h"
#include "ORBDetector.h"
#include "common_include.h"

using namespace ORB_SLAM2_REWRITE;
using namespace boost;
int main(int argc, char **argv) {
    int num = 0, process = 0, process_percent = 0;
    auto config = Config::getInstance();
    auto detector = ORBDetector::getInstance();
    std::vector<std::vector<Descriptor>> descsDataset;
    Vocabulary vocab;
    filesystem::path pathDataset0(Config::m_DATASET_PATH + "image_0");
    filesystem::path pathDataset1(Config::m_DATASET_PATH + "image_1");

    if (filesystem::exists(pathDataset0) && filesystem::is_directory(pathDataset0)) {
        for (filesystem::directory_iterator it(pathDataset0); it != filesystem::directory_iterator(); ++it) {
            num += 1;
        }
        num *= 2;
    } else {
        return 1;
    }

    for (filesystem::directory_iterator it(pathDataset0); it != filesystem::directory_iterator(); ++it) {
        auto image = cv::imread(it->path().string(), cv::IMREAD_GRAYSCALE);
        KeyPoints keyPoints;
        Descriptors descriptors;
        detector->detectORB(image, 500, Config::m_FAST_THRESHOLD_MAX, Config::m_FAST_THRESHOLD_MIN, keyPoints);
        detector->computeDescriptor(image, keyPoints, descriptors);
        descsDataset.push_back(descriptors);
        process += 1;
        process_percent = 100 * process / num;

        std::string block = "";
        std::string bar = "";
        for (int i = 0; i < process_percent; ++i)
            block += "#";
        for (int i = 0; i < 100 - process_percent; ++i)
            bar += " ";
        fmt::print("process: {}{}\t{:03d}\r", bar, block, process_percent);
    }
    for (filesystem::directory_iterator it(pathDataset1); it != filesystem::directory_iterator(); ++it) {
        auto image = cv::imread(it->path().string(), cv::IMREAD_GRAYSCALE);
        KeyPoints keyPoints;
        Descriptors descriptors;
        detector->detectORB(image, 500, Config::m_FAST_THRESHOLD_MAX, Config::m_FAST_THRESHOLD_MIN, keyPoints);
        detector->computeDescriptor(image, keyPoints, descriptors);
        descsDataset.push_back(descriptors);

        process += 1;
        process_percent = 100 * process / num;
        std::string block = "";
        std::string bar = "";
        for (int i = 0; i < process_percent; ++i)
            block += "#";
        for (int i = 0; i < 100 - process_percent; ++i)
            bar += " ";
        fmt::print("process: {}{}\t{:03d}\r", bar, block, process_percent);
    }
    vocab.create(descsDataset, 10, 5, DBoW2::WeightingType::TF_IDF, DBoW2::ScoringType::L1_NORM);
    vocab.save(Config::m_VOCAB_SAVE_PATH);

    return 0;
}
