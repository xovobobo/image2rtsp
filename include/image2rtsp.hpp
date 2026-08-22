#ifndef IMAGE2RTSP_IMAGE2RTSP_HPP
#define IMAGE2RTSP_IMAGE2RTSP_HPP

#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <rclcpp/rclcpp.hpp>
#include <string>
#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"

using namespace std;

class Image2rtsp : public rclcpp::Node{
public:
    Image2rtsp();
    GstRTSPServer *rtsp_server;
    void rtsp_media_prepared(GstRTSPMedia *media);
    void rtsp_media_unprepared();

private:
    string source;
    string topic;
    bool compressed_image;
    string mountpoint;
    string bitrate;
    string framerate;
    string caps_1;
    string caps_2;
    string port;
    string pipeline;
    string pipeline_head;
    string pipeline_tail;
    string custom_pipeline;
    bool local_only;
    bool camera;
    bool override_pipeline;
    bool pipeline_initialized;
    bool stream_active;
    GstAppSrc *appsrc;

    void initialize_pipeline(bool bayer);
    void video_mainloop_start();
    void rtsp_server_add_url(const char *url, const char *sPipeline, Image2rtsp *node);
    void topic_callback(const sensor_msgs::msg::Image::SharedPtr msg);
    void topic_compressed_callback(const sensor_msgs::msg::CompressedImage::SharedPtr msg);
    GstRTSPServer *rtsp_server_create(const string &port, const bool local_only);
    GstCaps *gst_caps_new_from_image(const sensor_msgs::msg::Image::SharedPtr &msg);
    GstCaps *gst_caps_new_from_compressed_image(const sensor_msgs::msg::CompressedImage::SharedPtr &msg);
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr subscription_compressed_;
};

static void media_configure(GstRTSPMediaFactory *factory, GstRTSPMedia *media, Image2rtsp *node);
static void *mainloop(void *arg);
static gboolean session_cleanup(Image2rtsp *node, rclcpp::Logger logger, gboolean ignored);

#endif // IMAGE2RTSP_IMAGE2RTSP_HPP
