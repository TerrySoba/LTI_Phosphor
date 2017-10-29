#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <tclap/CmdLine.h>

static const int SCREEN_WIDTH = 2880;
static const int SCREEN_HEIGHT = 2160;

static const std::string imagePath = "/home/yoshi252/program_sources/LTI_Phosphor/gfx";


void blit(const cv::Mat& src, cv::Mat& dest, int x, int y)
{
    volatile int srcType = src.type();
    volatile int dtsType = dest.type();
    src.copyTo(dest(cv::Rect(x,y, src.cols, src.rows)));
}

int main(int argc, char* argv[]) {

    TCLAP::CmdLine cmd("Upscale to 4k image", ' ', "0.1");

    TCLAP::ValueArg<std::string> inputFile(
        "i",
        "input",
        "Image file to upscale",
        true,
        "",
        "filename");
    cmd.add(inputFile);

    TCLAP::ValueArg<std::string> outputFile(
        "o",
        "output",
        "Output file",
        false,
        "",
        "filename");
    cmd.add(outputFile);

    cmd.parse(argc, argv);

    std::cout << "Doing phosphor simulation." << std::endl;

    auto subpixel = cv::imread(imagePath + "/subpixles_4k.png");

    cv::Mat phosphorPixels(SCREEN_HEIGHT, SCREEN_WIDTH, CV_8UC3, cv::Scalar(0,0,0));

    for (size_t x = 0; x < SCREEN_WIDTH - subpixel.cols; x += subpixel.cols) {
        for (size_t y = 0; y < SCREEN_HEIGHT - subpixel.rows; y += subpixel.rows) {
            blit(subpixel, phosphorPixels, x, y);
        }
    }
    // cv::imwrite(imagePath + "/out.png", phosphorPixels);



    // auto gameImage = cv::imread(imagePath + "/test.png");
    auto gameImage = cv::imread(inputFile.getValue());

    auto scanlineRgb = cv::imread(imagePath + "/scanline_mono.png", cv::IMREAD_GRAYSCALE);
    cv::Mat scanline;
    scanlineRgb.convertTo(scanline, CV_32FC1, 1 / 255.0);

    // scale scanline to correct size
    int scanlineWidth = SCREEN_WIDTH / gameImage.cols;
    int scanlineHeight = SCREEN_HEIGHT / gameImage.rows;

    cv::resize(scanline, scanline, cv::Size(scanlineWidth, scanlineHeight));
    std::cout << "w:" << scanline.cols << " h:" << scanline.rows << std::endl;

    cv::Mat scanlinesR(SCREEN_HEIGHT, SCREEN_WIDTH, CV_32FC1, cv::Scalar(0));
    cv::Mat scanlinesG(SCREEN_HEIGHT, SCREEN_WIDTH, CV_32FC1, cv::Scalar(0));
    cv::Mat scanlinesB(SCREEN_HEIGHT, SCREEN_WIDTH, CV_32FC1, cv::Scalar(0));

    for (size_t x = 0; x < gameImage.cols; ++x) {
        for (size_t y = 0; y < gameImage.rows; ++y) {

            cv::Vec3b intensity = gameImage.at<cv::Vec3b>(y, x);
            float red = intensity.val[2] / 255.0;
            float green = intensity.val[1] / 255.0;
            float blue = intensity.val[0] / 255.0;

            blit(scanline * red, scanlinesR, x * scanline.cols, y * scanline.rows);
            blit(scanline * green, scanlinesG, x * scanline.cols, y * scanline.rows);
            blit(scanline * blue, scanlinesB, x * scanline.cols, y * scanline.rows);
        }
    }

    // cv::imwrite(imagePath + "/scan_image.png", scanlinesR);

    std::vector<cv::Mat> phosphorBgr;

    phosphorPixels.convertTo(phosphorPixels, CV_32FC3, 1/255.0);
    cv::split(phosphorPixels, phosphorBgr);

    phosphorBgr[0] = scanlinesB.mul(phosphorBgr[0]);
    phosphorBgr[1] = scanlinesG.mul(phosphorBgr[1]);
    phosphorBgr[2] = scanlinesR.mul(phosphorBgr[2]);


    cv::Mat screen;
    cv::merge(phosphorBgr, screen);

    cv::GaussianBlur(screen, screen, cv::Size(9,5), 10);
    screen = screen * 8;
    screen.convertTo(screen, CV_8UC3, 255);

    if (outputFile.isSet()) {
        cv::imwrite(outputFile.getValue(), screen);
    } else {
        cv::imshow("result", screen);
        cv::waitKey();
    }

    std::cout << "Doing phosphor simulation. END" << std::endl;
    return 0;
}
