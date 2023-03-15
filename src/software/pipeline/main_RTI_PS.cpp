#include <aliceVision/image/all.hpp>
#include <aliceVision/image/io.hpp>

// SFMData
#include <aliceVision/sfmData/SfMData.hpp>
#include <aliceVision/sfmDataIO/sfmDataIO.hpp>

// Photometric Stereo
#include <aliceVision/photometricStereo/photometricDataIO.hpp>
#include <aliceVision/photometricStereo/photometricStereo.hpp>

// Command line parameters
#include <aliceVision/system/main.hpp>
#include <aliceVision/system/Logger.hpp>
#include <aliceVision/system/cmdline.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

// Eigen
#include <Eigen/Dense>
#include <Eigen/Core>

//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/core/eigen.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <array>
#include <sstream>
#include <fstream>

// These constants define the current software version.
// They must be updated when the command line is changed.
#define ALICEVISION_SOFTWARE_VERSION_MAJOR 0
#define ALICEVISION_SOFTWARE_VERSION_MINOR 1

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace aliceVision;

int aliceVision_main(int argc, char **argv)
{
    std::string inputPath;
    std::string maskPath;
    std::string outputPath;
    std::string pathToLightData;
    size_t HS_order;
    bool removeAmbiant;
    bool  isRobust;

    // image downscale factor during process
    int downscale = 1;

    po::options_description allParams("AliceVision photometricStereo");
    po::options_description requiredParams("Required parameters");
    requiredParams.add_options()
    ("inputPath,i", po::value<std::string>(&inputPath)->required(), "Path to input; could be SfMData file or folder with pictures");

    po::options_description optionalParams("Optional parameters");
    optionalParams.add_options()
    ("maskPath,m", po::value<std::string>(&maskPath)->default_value(""), "Path to mask folder/file.")
    ("pathToJSONLightFile,l", po::value<std::string>(&pathToLightData)->default_value("defaultJSON.txt"), "Path to light file (JSON). If empty, expects txt files in picture folder")
    ("HSOrder,h", po::value<size_t>(&HS_order)->default_value(0), "HS order, 0 = directional, 1 = directional + ambiant")
    ("removeAmbiant,a", po::value<bool>(&removeAmbiant)->default_value(false), "Do we need to remove ambiant light on PS pictures ?")
    ("isRobust,r", po::value<bool>(&isRobust)->default_value(false), "Robust algorithm ?")
    ("outputPath,o", po::value<std::string>(&outputPath)->default_value(""), "output path")
    ("downscale, d", po::value<int>(&downscale)->default_value(downscale), "Downscale factor for faster results" );

    allParams.add(requiredParams).add(optionalParams);

    po::variables_map vm;
    try
    {
        po::store(po::parse_command_line(argc, argv, allParams), vm);

        if(vm.count("help") || (argc == 1))
        {
          ALICEVISION_COUT(allParams);
          return EXIT_SUCCESS;
        }
        po::notify(vm);
    }
    catch(po::required_option& e)
    {
      ALICEVISION_CERR("ERROR: " << e.what());
      ALICEVISION_COUT("Usage:\n\n" << allParams);
      return EXIT_FAILURE;
    }
    catch(po::error& e)
    {
      ALICEVISION_CERR("ERROR: " << e.what());
      ALICEVISION_COUT("Usage:\n\n" << allParams);
      return EXIT_FAILURE;
    }

    ALICEVISION_COUT("Program called with the following parameters:");
    ALICEVISION_COUT(vm);

    // If the path to light data is empty, set it to inputPath :
    if(pathToLightData.compare("") && fs::is_directory(inputPath))
    {
        std::cout << "Warning : path to light data has been set to inputpath folder" << std::endl;
        pathToLightData = inputPath;
    }

    image::Image<image::RGBfColor> normalsIm;
    image::Image<image::RGBfColor> albedoIm;

    sfmData::SfMData sfmData;
    if(!sfmDataIO::Load(sfmData, inputPath, sfmDataIO::ESfMData(sfmDataIO::VIEWS|sfmDataIO::INTRINSICS)))
    {
        ALICEVISION_LOG_ERROR("The input file '" + inputPath + "' cannot be read");
        return EXIT_FAILURE;
    }

    photometricStereo::RTI(sfmData, pathToLightData, maskPath, outputPath, normalsIm, albedoIm);

    return 0;
}
