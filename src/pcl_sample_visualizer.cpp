#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/point_cloud.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl_conversions/pcl_conversions.h>

class cloudHandler
{
public:
    cloudHandler()
    : viewer("Cloud Viewer")
    {  
       output_sub = nh.subscribe("/pcl_reader_output", 1, &cloudHandler::input_cloud, this);
      downsampled_sub = nh.subscribe("/statistical_outlier_removal_cloud", 1, &cloudHandler::filtered_cloud, this);

        viewer_timer = nh.createTimer(ros::Duration(0.1), &cloudHandler::timerCB, this);

        viewer.createViewPort(0.0, 0.0, 0.5, 1.0, output_view);
        viewer.setBackgroundColor(0, 0, 0, output_view);

        viewer.createViewPort(0.5, 0.0, 1.0, 1.0, downsampled_view);
        viewer.setBackgroundColor(0, 0, 0, downsampled_view);



        viewer.addCoordinateSystem(1.0);
        viewer.initCameraParameters();
    }

    void input_cloud(const sensor_msgs::PointCloud2ConstPtr& input)
    {
        pcl::PointCloud<pcl::PointXYZ> cloud;
        pcl::fromROSMsg(*input, cloud);

        viewer.removeAllPointClouds(output_view);
        viewer.addPointCloud<pcl::PointXYZ>(cloud.makeShared(), "input", output_view);
    }


    void filtered_cloud(const sensor_msgs::PointCloud2ConstPtr& input)
    {
        pcl::PointCloud<pcl::PointXYZ> cloud;
        pcl::fromROSMsg(*input, cloud);

        viewer.removeAllPointClouds(downsampled_view);
        viewer.addPointCloud<pcl::PointXYZ>(cloud.makeShared(), "filtered", downsampled_view);
    }

    void timerCB(const ros::TimerEvent&)
    {
        viewer.spinOnce();

        if (viewer.wasStopped())
        {
            ros::shutdown();
        }
    }

protected:
    ros::NodeHandle nh;
    ros::Subscriber output_sub, downsampled_sub;
    pcl::visualization::PCLVisualizer viewer;
    int output_view, downsampled_view;
    ros::Timer viewer_timer;
};

main (int argc, char **argv)
{
    ros::init (argc, argv, "pcl_visualize2");

    cloudHandler handler;

    ros::spin();

    return 0;
}