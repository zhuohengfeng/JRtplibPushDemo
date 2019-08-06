cmake_minimum_required(VERSION 3.13)
project(JRtplibPushDemo)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-g -Wall")

#--------------- FFMpeg ----------------------
set(FFMPEG_PATH "/usr/local/Cellar/ffmpeg/4.1.1")
INCLUDE_DIRECTORIES(${FFMPEG_PATH}/include)
LINK_DIRECTORIES(${FFMPEG_PATH}/lib)

add_executable(JRtplibPushDemo
            main.cpp
            RtpSend.cpp
            RtpPack.cpp sample/main_ps_rtp.cpp)

TARGET_LINK_LIBRARIES(JRtplibPushDemo
        avcodec avdevice avfilter avformat avresample avutil postproc swresample swscale)