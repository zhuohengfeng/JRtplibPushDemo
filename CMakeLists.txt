cmake_minimum_required(VERSION 3.13)
project(JRtplibPushDemo)

set(CMAKE_CXX_STANDARD 11)

add_executable(JRtplibPushDemo main.cpp RtpSend.cpp RtpSend.h RtpPack.cpp RtpPack.h)