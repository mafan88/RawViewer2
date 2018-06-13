/*
**	********************************************************************************
**
**                          Digital Video Recoder xp
**
**   (c) Copyright 1992-2012, ZheJiang Dahua Information Technology Stock CO.LTD.
**                            All Rights Reserved
**
**	File		: EncodingImage.cpp
**	Description	: 
**  Create		: 18-6-6      LDL		Create the file
**
**	********************************************************************************
*/

#include "EncodingImage.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>


CEncodingImage::CEncodingImage(std::string filename)
{
    m_filename = filename;
}
bool CEncodingImage::open()
{
    m_mat = cv::imread(m_filename.c_str());
    if( !m_mat.empty() )
    {
        m_frameCount = 1;
    }
    else
    {
        m_capture.open(m_filename.c_str());
        int frames = m_capture.get(CV_CAP_PROP_FRAME_COUNT);
        if( frames == 0 )
        {
            return false;
        }
        m_frameCount = frames;
        printf("frames:%d\n", frames);
        m_capture.read(m_mat);
    }

    printf("img width:%d height:%d\n", m_mat.cols, m_mat.rows);
    m_frame = CFrame(m_mat.cols, m_mat.rows, AV_PIX_FMT_BGR24);

    return true;
}
bool CEncodingImage::close()
{
    return false;
}
bool CEncodingImage::getFrameInfo(FrameInfo &info)
{
    info = m_frame.info;

    return true;
}
int CEncodingImage::getFrameCount()
{
    return m_frameCount;
}
int CEncodingImage::getFrame(int nframe, CFrame &frame)
{
    if( nframe >= m_frameCount )
        return -1;

    if( m_frameCount > 1 )
    {
//        printf("[%s] nframe��%d frameCount:%d\n", __FUNCTION__, nframe, m_frameCount);
        m_capture.set(CV_CAP_PROP_POS_FRAMES, nframe);
        m_capture.read(m_mat);
    }
    memcpy(m_frame.data[0], m_mat.data, m_mat.cols*m_mat.rows*3);

    frame = m_frame;
    return 0;
}