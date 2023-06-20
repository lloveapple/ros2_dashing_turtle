#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <turtlesim/msg/pose.hpp>
#include <math.h>
// 定义一些全局变量，包括乌龟的位置、角度以及目标点的位置
double x, y, theta;
double nx, ny, ntheta;
double dx = 8, dy = 8;

// 这是接收到乌龟位置信息时的回调函数，它会更新全局变量的值
void poseGet(const turtlesim::msg::Pose::SharedPtr msg)
{
    x = msg->x;
    y = msg->y;
    theta = msg->theta;
}
// 这是接收到第二只乌龟位置信息时的回调函数，它也会更新全局变量的值

void selfPoseGet(const turtlesim::msg::Pose::SharedPtr msg)
{
    nx = msg->x;
    ny = msg->y;
    ntheta = msg->theta;
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("nodes_handle");
    // 创建订阅，接收乌龟的位置信息
    auto pose_sub = node->create_subscription<turtlesim::msg::Pose>("/turtle1/pose", rclcpp::QoS(10), poseGet);
    auto self_pose_sub = node->create_subscription<turtlesim::msg::Pose>("/turtle2/pose", rclcpp::QoS(10), selfPoseGet);
    // 创建发布器，发布乌龟的速度命令
    auto cmd_vel_pub = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", rclcpp::QoS(10));
    auto cmd_vel_pub2 = node->create_publisher<geometry_msgs::msg::Twist>("/turtle2/cmd_vel", rclcpp::QoS(10));


    rclcpp::Rate loop_rate(1000);

    double length_old = 10000.0;
    while (rclcpp::ok())
    {
        auto speed = std::make_shared<geometry_msgs::msg::Twist>();
        double length = sqrt((dx - x) * (dx - x) + (dy - y) * (dy - y));
        speed->linear.x = 0;
        speed->linear.y = 0;
        speed->linear.z = 0;
        speed->angular.x = 0;
        speed->angular.y = 0;
        speed->angular.z = 0;

        // 如果乌龟距离目标点的距离大于0.1，就让乌龟向目标点移动
        if (length > 0.1 && length_old+0.01 > length)
        {
            speed->linear.x = 0.5 * length ;
            double angle_to_target = atan2(dy - y, dx - x);
            if (angle_to_target < 0)
            {
                angle_to_target += 2 * M_PI; // convert to 0 to 2π range
            }

            double angle_diff = angle_to_target - theta;
            if (angle_diff > M_PI)
            {
                angle_diff -= 2 * M_PI; // wrap-around from positive to negative
            }
            else if (angle_diff < -M_PI)
            {
                angle_diff += 2 * M_PI; // wrap-around from negative to positive
            }
            speed->angular.z = angle_diff;
            cmd_vel_pub->publish(*speed);
        } else{
            length = 0.09;
        }
        if (length < 0.1)
        {
//            double distance_to_center = sqrt(pow(nx - dx, 2) + pow(ny - dy, 2));
            speed->linear.x = 1.0; // Fixed linear speed
            double angle_to_perpendicular = atan2(dy - ny, dx - nx) - M_PI/2; // Perpendicular to the line between turtle2 and the center
            if (angle_to_perpendicular < 0)
            {
                angle_to_perpendicular += 2 * M_PI; // convert to 0 to 2π range
            }

            double angle_diff = angle_to_perpendicular - ntheta;
            if (angle_diff > M_PI)
            {
                angle_diff -= 2 * M_PI; // wrap-around from positive to negative
            }
            else if (angle_diff < -M_PI)
            {
                angle_diff += 2 * M_PI; // wrap-around from negative to positive
            }

            // First, adjust the turtle's heading to the perpendicular direction
            if (abs(angle_diff) > 0.5) // Adjust the tolerance to fit your need
            {
                speed->angular.z =  8 * angle_diff; // Adjust angular speed based on the difference
            }
            else // If the heading is already in the perpendicular direction, start to rotate at constant angular speed
            {
                speed->angular.z = 1.0; // Set a constant angular speed
            }

            cmd_vel_pub2->publish(*speed);

        }
        length_old = length;
        rclcpp::spin_some(node);
        loop_rate.sleep();
    }

    return 0;
}