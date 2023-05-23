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
        // -------fitLine2d------- //
        auto pathToTXT = resource_dir / std::filesystem::path("Points2D.txt");
        if (!std::filesystem::exists(pathToTXT))
            throw std::invalid_argument("Cannot find file with datas");

        std::ifstream fin(pathToTXT.string());
        
        int size; fin >> size; fin.get();
        points2D.resize(size);
        float x, y; char sep;

        for (int i = 0; i < size; i++) {
            fin >> x >> sep >> y;
            points2D[i] = cv::Point2f(x, y);
        }

        fin.close();
        // -------fitLine2d------- //
        
        // -------fitLine3d------- //
        pathToTXT = resource_dir / std::filesystem::path("Points3D.txt");
        if (!std::filesystem::exists(pathToTXT))
            throw std::invalid_argument("Cannot find file with datas" + pathToTXT.string());

        std::ifstream fin3D(pathToTXT.string());

        fin3D >> size; fin3D.get();
        points3D.resize(size);
        float z;

        for (int i = 0; i < size; i++) {
            fin3D >> x >> sep >> y >> sep >> z;
            points3D[i] = cv::Point3f(x, y, z);
        }

        fin3D.close();

        // -------fitLine3d------- //
    }

    //! @see idemo::execute_opencv
    void execute_opencv() override
    {

        // -------fitLine2d------- //
        cv::Vec4f coeffs;
        cv::fitLine(points2D, coeffs, cv::DIST_L1, 0, 0.1, 0.1);

        float vx = coeffs[0], vy = coeffs[1],
              x0 = coeffs[2], y0 = coeffs[3];

        cv_k = vy / vx;
        cv_m = y0 - x0 * cv_k;

        std::cout << "-------fitLine2D CV-------" << std::endl;
        std::cout << "cv_k: " << cv_k << "; cv_m: " << cv_m << std::endl;
        // -------fitLine2d------- //

        // -------fitLine3d------- //
        cv::fitLine(points3D, cv_coeffs3D, cv::DIST_L1, 0, 0.1, 0.1);
        std::cout << "-------fitLine3D CV-------" << std::endl;
        std::cout << "vx: " << cv_coeffs3D[0] << "; vy: " << cv_coeffs3D[1] << "; vz: " << cv_coeffs3D[2] << std::endl;
        std::cout << "x0: " << cv_coeffs3D[3] << "; y0: " << cv_coeffs3D[4] << "; z0: " << cv_coeffs3D[5] << std::endl;
        // -------fitLine3d------- //
    }

    void execute_openvx() override
    {
        size_t N = points2D.size();
        // -------fitLine2d------- //
        float *x_arr, *y_arr;
        x_arr = new float [N];
        y_arr = new float[N];

        for (int i = 0; i < N; i++) {
            x_arr[i] = points2D[i].x;
            y_arr[i] = points2D[i].y;
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

        

        std::cout << "-------fitLine2D VX-------" << std::endl;
        std::cout << "vx_k: " << vx_k << "; vx_m: " << vx_m << std::endl;
        delete[] x_arr, y_arr;
        // -------fitLine2d------- //

        // -------fitLine3d------- //
        float *z_arr;
        x_arr = new float[N];
        y_arr = new float[N];
        z_arr = new float[N];
        for (int i = 0; i < N; i++) {
            x_arr[i] = points3D[i].x;
            y_arr[i] = points3D[i].y;
            z_arr[i] = points3D[i].z;
        }

        _vx_array x_arr_vx_3D = {
            x_arr,
            uint32_t(N),
            VX_TYPE_FLOAT64
        };
        _vx_array y_arr_vx_3D = {
            y_arr,
            uint32_t(N),
            VX_TYPE_FLOAT64
        };
        _vx_array z_arr_vx_3D = {
            z_arr,
            uint32_t(N),
            VX_TYPE_FLOAT64
        };

        _lineParams3d vx_params;

        if (auto status = fitLine3D(&x_arr_vx_3D, &y_arr_vx_3D, &z_arr_vx_3D, &vx_params); status != VX_SUCCESS)
            throw std::runtime_error("OpenVX: fitLine failed: " + std::to_string(status));

        std::cout << "-------fitLine3D VX-------" << std::endl;
        std::cout << "vx: " << vx_params.a << "; vy: " << vx_params.b << "; vz: " << vx_params.c << std::endl;
        std::cout << "x0: " << vx_params.x0 << "; y0: " << vx_params.y0 << "; z0: " << vx_params.z0 << std::endl;

        vx_coeffs3D = { vx_params.a,  vx_params.b , vx_params.c , vx_params.x0 , vx_params.y0 , vx_params.z0 };

        delete[] x_arr, y_arr, z_arr;
        // -------fitLine3d------- //

        
    }

    //! @see idemo::make_diff
    void make_diff() override
    {
        std::cout << "-------fitLine2D DIFF-------" << std::endl;
        std::cout << "cv_k - vx_k = " << cv_k - vx_k << std::endl;
        std::cout << "cv_m - vx_m = " << cv_m - vx_m << std::endl;

        std::cout << "-------fitLine3D DIFF-------" << std::endl;
        std::cout << "cv_vx - vx_vx = " << cv_coeffs3D[0] - vx_coeffs3D[0] << std::endl;
        std::cout << "cv_vy - vx_vy = " << cv_coeffs3D[1] - vx_coeffs3D[1] << std::endl;
        std::cout << "cv_vz - vx_vz = " << cv_coeffs3D[2] - vx_coeffs3D[2] << std::endl;
        std::cout << "cv_x0 - vx_x0 = " << cv_coeffs3D[3] - vx_coeffs3D[3] << std::endl;
        std::cout << "cv_y0 - vx_y0 = " << cv_coeffs3D[4] - vx_coeffs3D[4] << std::endl;
        std::cout << "cv_z0 - vx_z0 = " << cv_coeffs3D[5] - vx_coeffs3D[5] << std::endl;
    }

private:
    std::vector<cv::Point2f> points2D;
    std::vector<cv::Point3f> points3D;

    float cv_k, cv_m; // parameters k, m in y = kx + m, from cv::fitLine2D
    float vx_k, vx_m; // parameters k, m in y = kx + m, from vx fitLine2D

    cv::Vec6f cv_coeffs3D;
    cv::Vec6f vx_coeffs3D;
};

// Register the demo.
static demo_fitLine demo_fitLine_instance;
