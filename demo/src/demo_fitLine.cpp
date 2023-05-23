/*! @file
 *  @brief Contains demonstration of fitLine function in comparing with OpenCV.
 *  @author Stas Ivashkov
 *  @date 21 May 2023
 */

#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <kernels.h>
#include <detail/types.h>

#include "idemo.hpp"

 //! Demonstration of threshold function.
class demo_fitLine : public idemo
{
public:
    demo_fitLine() : idemo("fitLine")
    {
    }

    //! @see idemo::prepeare
    void prepare(const std::filesystem::path& resource_dir) override
    {
        
        auto pathToTXT = resource_dir / std::filesystem::path("Points2D.txt");
        if (!std::filesystem::exists(pathToTXT))
            throw std::invalid_argument("Cannot find file with datas");

        std::ifstream fin(pathToTXT.string());
        
        int size; fin >> size; fin.get();
        points.resize(size);
        float x, y; char sep;

        for (int i = 0; i < size; i++) {
            fin >> x >> sep >> y;
            points[i] = cv::Point2f(x, y);
        }

        fin.close();
    }

    //! @see idemo::execute_opencv
    void execute_opencv() override
    {

        // -------fitLine2d-------
        cv::Vec4f coeffs;
        cv::fitLine(points, coeffs, cv::DIST_L1, 0, 0.1, 0.1);

        float vx = coeffs[0], vy = coeffs[1],
              x0 = coeffs[2], y0 = coeffs[3];

        cv_k = vy / vx;
        cv_m = y0 - x0 * cv_k;

        std::cout << "-------fitLine2D CV-------" << std::endl;
        std::cout << "cv_k: " << cv_k << "; cv_m: " << cv_m << std::endl;
        // -------fitLine2d-------
    }

    void execute_openvx() override
    {
        size_t N = points.size();
        // -------fitLine2d-------
        

        float *x_arr, *y_arr;
        x_arr = new float [N];
        y_arr = new float[N];

        for (int i = 0; i < N; i++) {
            x_arr[i] = points[i].x;
            y_arr[i] = points[i].y;
        }

        _vx_array x_arr_vx = {
            x_arr,
            uint32_t(N),
            VX_TYPE_FLOAT64
        };

        _vx_array y_arr_vx = {
            y_arr,
            uint32_t(N),
            VX_TYPE_FLOAT64
        };

        if (auto status = fitLine2D(&x_arr_vx, &y_arr_vx, &vx_k, &vx_m); status != VX_SUCCESS)
            throw std::runtime_error("OpenVX: fitLine failed: " + std::to_string(status));

        delete[] x_arr, y_arr;

        std::cout << "-------fitLine2D VX-------" << std::endl;
        std::cout << "vx_k: " << vx_k << "; vx_m: " << vx_m << std::endl;
        // -------fitLine2d-------
    }

    //! @see idemo::make_diff
    void make_diff() override
    {
        std::cout << "-------fitLine2D DIFF-------" << std::endl;
        std::cout << "cv_k - vx_k = " << cv_k - vx_k << std::endl;
        std::cout << "cv_m - vx_m = " << cv_m - vx_m << std::endl;
    }

private:
    std::vector<cv::Point2f> points;

    float cv_k, cv_m; // parameters k, m in y = kx + m, from cv::fitLine
    float vx_k, vx_m; // parameters k, m in y = kx + m, from vx fitLine
};

// Register the demo.
static demo_fitLine demo_fitLine_instance;
