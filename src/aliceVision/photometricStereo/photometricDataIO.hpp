// This file is part of the AliceVision project.
// Copyright (c) 2021 AliceVision contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.
#pragma once

#include <aliceVision/image/Image.hpp>
#include <aliceVision/types.hpp>
#include <aliceVision/system/Logger.hpp>

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <string>
#include <vector>
#include <array>

namespace aliceVision {
namespace photometricStereo {

void loadLightIntensities(const std::string& intFileName, std::vector<std::array<float, 3>>& intList);

void loadLightDirections(const std::string& dirFileName, const Eigen::MatrixXf& convertionMatrix, Eigen::MatrixXf& lightMat);

void loadLightHS(const std::string& dirFileName, Eigen::MatrixXf& lightMat);

void buildLigtMatFromJSON(const std::string& fileName, const std::vector<std::string>& imageList, Eigen::MatrixXf& lightMat, std::vector<std::array<float, 3>>& intList);

void loadMask(std::string const& maskName, image::Image<float>& mask);

void getIndMask(image::Image<float> const& mask, std::vector<int>& indexes);

void intensityScaling(std::array<float, 3> const& intensities, image::Image<image::RGBfColor>& imageToScale);

void image2PsMatrix(const image::Image<image::RGBfColor>& imageIn, const image::Image<float>& mask, Eigen::MatrixXf& imageOut);

void image2PsMatrix(const image::Image<float>& imageIn, const image::Image<float>& mask, Eigen::VectorXf& imageOut);

void reshapeInImage(const Eigen::MatrixXf& matrixIn, image::Image<image::RGBfColor>& imageOut);

void convertNormalMap2png(const image::Image<image::RGBfColor>& normalsIm, image::Image<image::RGBColor>& normalsImPNG);

void readMatrix(const std::string& fileName, Eigen::MatrixXf& matrix);

void writePSResults(const std::string& outputPath, const image::Image<image::RGBfColor>& normals, const image::Image<image::RGBfColor>& albedo);

void writePSResults(const std::string& outputPath, const image::Image<image::RGBfColor>& normals, const image::Image<image::RGBfColor>& albedo, const IndexT poseId);

}
}
