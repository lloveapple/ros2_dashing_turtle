#include <rclcpp/rclcpp.hpp> // 提供了ROS2的基础功能
#include <tf2/LinearMath/Quaternion.h> // 提供了对四元数的操作
#include <tf2_ros/transform_listener.h> // 提供了tf2的转换监听功能
#include <geometry_msgs/msg/twist.hpp> // 提供了Twist消息类型
#include <turtlesim/srv/spawn.hpp> // 提供了Spawn服务类型
#include <tf2_ros/buffer.h> // 提供了tf2的缓冲区
#include <tf2_geometry_msgs/tf2_geometry_msgs.h> // 提供了tf2和geometry_msgs之间的转换
#include <cmath> // 提供了数学函数

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv); // 初始化ROS2
    auto node = rclcpp::Node::make_shared("my_tf_listener"); // 创建一个节点

    // 创建一个Spawn服务的客户端
    rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr client =
            node->create_client<turtlesim::srv::Spawn>("spawn");
    // 创建一个Spawn服务的请求
    auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
    request->name = "turtle2"; // 设置要生成的乌龟的名字
    request->x = 5.0; // 设置乌龟的初始x坐标
    request->y = 5.0; // 设置乌龟的初始y坐标
    request->theta = 0.0; // 设置乌龟的初始方向

    // 等待Spawn服务可用
    while (!client->wait_for_service(std::chrono::seconds(1)))
    {
        if (!rclcpp::ok()) // 如果ROS2不正常
        {
            RCLCPP_ERROR(node->get_logger(), "Interrupted while waiting for the service. Exiting."); // 打印错误信息
            return 0; // 并结束程序
        }
        RCLCPP_INFO(node->get_logger(), "service not available, waiting again..."); // 打印提示信息
    }

    auto result = client->async_send_request(request); // 发送请求

    // 创建一个发布器，发布Twist消息
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher =
            node->create_publisher<geometry_msgs::msg::Twist>("turtle2/cmd_vel", 10);

    tf2_ros::Buffer tfBuffer(node->get_clock()); // 创建一个tf2的缓冲区
    tf2_ros::TransformListener tfListener(tfBuffer); // 创建一个tf2的转换监听器

    rclcpp::Rate rate(10.0); // 设置循环的频率
    while (rclcpp::ok()) // 在ROS2正常的情况下
    {
        geometry_msgs::msg::TransformStamped transformStamped; // 创建一个TransformStamped消息
        try
        {
            // 获取转换
            transformStamped = tfBuffer.lookupTransform("turtle2", "turtle1", tf2::TimePointZero);
        }
        catch (tf2::TransformException &ex) // 如果获取失败
        {
            RCLCPP_ERROR(node->get_logger(), "Transform error: %s", ex.what()); // 打印错误信息
            rate.sleep(); // 等待一段时间
            continue; // 继续下一次循环
        }

        // 创建一个Twist消息
        geometry_msgs::msg::Twist vel_msg;
        // 根据转换计算角速度和线速度
        vel_msg.angular.z = 4.0 * atan2(transformStamped.transform.translation.y, transformStamped.transform.translation.x);
        vel_msg.linear.x = 0.5 * sqrt(pow(transformStamped.transform.translation.x, 2) +
                                      pow(transformStamped.transform.translation.y, 2));
        publisher->publish(vel_msg); // 发布消息

        rate.sleep(); // 等待一段时间
    }
    return 0;
}
