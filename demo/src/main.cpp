/*! @file
 *  @brief Entry point of the Demo Application
 *  @author Ivashkov Stas
 *  @date 17 May 2023
 */

#include <iostream>
#include <filesystem>
#include <opencv2/highgui.hpp>
#include "idemo.hpp"

int main(int argc, char** argv)
{
    std::filesystem::path resource_dir = "../../../../image"; // т.к. мой проект собирается из ..\lab\out\build\x64-Debug\demo

    if (argc > 1)
        resource_dir = argv[1];

    auto& reg = idemo::get_global_registry();

    std::cout << "Demo Application contains " << reg.size() << " demos:\n";
    unsigned int idx = 0;
    for (auto item : reg)
        std::cout << idx++ << ") " << item->to_string() << std::endl;

    std::cout << "Please enter index of the demo to run: ";
    std::cin >> idx;

    auto demo = reg.at(idx);

    demo->prepare(resource_dir);

    std::cout << "Press Enter or Spacebar to apply parameters. Press Esc to exit...\n";
    do
    {
        demo->execute_opencv();
        demo->execute_openvx();
        demo->make_diff();
    } while (cv::waitKey(0) != 27);

    return 0;
}
