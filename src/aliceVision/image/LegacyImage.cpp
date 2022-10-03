// This file is part of the AliceVision project.
// Copyright (c) 2019 AliceVision contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "LegacyImage.hpp"

#include <aliceVision/image/io.hpp>
#include <aliceVision/image/imageAlgo.hpp>


namespace aliceVision{


void imageDiff(const ImageRGBf& inImg, const ImageRGBf& inImgDownscaled, ImageRGBf& outImg, unsigned int downscale)
{
    outImg.resize(inImg.Width(), inImg.Height());
    for(int i = 0; i < inImg.Width()*inImg.Height(); ++i)
    {
        Point2d pix(i%inImg.Width(), static_cast<int>(i/ inImg.Width()));
        Point2d pixd = pix/downscale;

        outImg[i] = inImg[i] - inImgDownscaled.getInterpolateColor(pixd);
    }
}

void laplacianPyramid(std::vector<ImageRGBf>& out_pyramidL, const ImageRGBf& image, int nbBand, unsigned int downscale)
{
    assert(nbBand >= 1);

    ImageRGBf img(image);
    int outW = static_cast<int>(img.Width()/downscale);
    int outH = static_cast<int>(img.Height()/downscale);

    ImageRGBf imgDownscaled(outW, outH);
    out_pyramidL.resize(nbBand);

    //Create Laplacian pyramid
    for(int b = 0; b < nbBand-1; ++b)
    {
        imageAlgo::resizeImage(static_cast<int>(downscale), img, imgDownscaled, "gaussian");
        imageDiff(img, imgDownscaled, out_pyramidL[b], downscale);
        img.swap(imgDownscaled);
/*
        outW = static_cast<int>(outW/downscale);
        outH = static_cast<int>(outH/downscale);
        imgDownscaled.resize(outW, outH);
*/
    }
    out_pyramidL[nbBand-1] = img;

    for(std::size_t i = 0; i < out_pyramidL.size(); ++i)
        ALICEVISION_LOG_DEBUG("laplacianDownscalePyramid: Size level " << i << " : " << out_pyramidL[i].Width() << "x" << out_pyramidL[i].Height());
}



}
