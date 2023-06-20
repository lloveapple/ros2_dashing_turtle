#include <rclcpp/rclcpp.hpp> // 这个头文件包含了ROS2的基本功能
#include <geometry_msgs/msg/twist.hpp> // 这个头文件包含了用于发布速度指令的消息类型
#include <turtlesim/msg/pose.hpp> // 这个头文件包含了乌龟的位姿消息类型
#include <math.h> // 这个头文件包含了数学函数和常数

// 定义全局变量来存储乌龟的位置和方向
double x, y, theta;
double nx, ny, ntheta;


// 这个函数用于更新第一个乌龟的位置和方向
void poseGet(const turtlesim::msg::Pose::SharedPtr msg)
{
    x = msg->x;
    y = msg->y;
    theta = msg->theta;
}

// 这个函数用于更新第二个乌龟的位置和方向
void selfPoseGet(const turtlesim::msg::Pose::SharedPtr msg)
{
    nx = msg->x;
    ny = msg->y;
    ntheta = msg->theta;
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv); // 初始化ROS2
    auto node = rclcpp::Node::make_shared("nodes_handle"); // 创建一个节点

    // 创建订阅，当收到位姿信息时，就会更新乌龟的位置和方向
    auto pose_sub = node->create_subscription<turtlesim::msg::Pose>("/turtle1/pose", rclcpp::QoS(10), poseGet);
    auto self_pose_sub = node->create_subscription<turtlesim::msg::Pose>("/turtle2/pose", rclcpp::QoS(10), selfPoseGet);
    // 创建一个发布器，用于发布速度指令
    auto cmd_vel_pub = node->create_publisher<geometry_msgs::msg::Twist>("/turtle2/cmd_vel", rclcpp::QoS(10));

    // 设置循环的频率，这里是1000赫兹
    rclcpp::Rate loop_rate(1000);

    while (rclcpp::ok())
    {
        auto speed = std::make_shared<geometry_msgs::msg::Twist>(); // 创建一个新的速度指令
        double length = sqrt((nx - x) * (nx - x) + (ny - y) * (ny - y));// 计算两个乌龟之间的距离
        // 速度指令的初始值都是0
        speed->linear.x = 0;
        speed->linear.y = 0;
        speed->linear.z = 0;
        speed->angular.x = 0;
        speed->angular.y = 0;
        speed->angular.z = 0;
//        if (length < 0.1)
//        {
//            speed->angular.z = theta-ntheta; // msg is the Pose of turtle2
//            cmd_vel_pub->publish(*speed);
//        }
//        if (length > 0.1)
//        {
//            speed->linear.x = (x - nx) / length * 0.3;
//            speed->linear.y = (y - ny) / length * 0.3;
//        }

        if (length > 0.1) // 如果距离大于0.1，乌龟就开始移动
        {
            // 设置线速度，使乌龟向第一个乌龟移动
            speed->linear.x = 0.5 * length ;
            // 计算乌龟应该面向的方向
            double angle_to_target = atan2(y - ny, x - nx);
            if (angle_to_target < 0)
            {
                angle_to_target += 2 * M_PI; //  // 把角度限制在0到2π之间
            }
            // 计算乌龟当前的方向和应该面向的方向之间的差距
            double angle_diff = angle_to_target - ntheta;
            if (angle_diff > M_PI)
            {
                angle_diff -= 2 * M_PI; // 如果差距超过π，就让它减去2π，使差距保持在-π到π之间
            }
            else if (angle_diff < -M_PI)
            {
                angle_diff += 2 * M_PI; // 如果差距小于-π，就让它加上2π，使差距保持在-π到π之间
            }
            // 根据角度差距设置角速度
            speed->angular.z =  4 * angle_diff;
        }
        // 发布速度指令
        cmd_vel_pub->publish(*speed);
        // 处理一些ROS2的事件，如接收到的消息和服务请求
        rclcpp::spin_some(node);
        // 等待一段时间，使得循环频率达到设定的值
        loop_rate.sleep();
    }

    return 0;
}