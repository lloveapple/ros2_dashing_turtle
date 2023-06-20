#include <rclcpp/rclcpp.hpp> // 提供了ROS2的基础功能
#include <tf2/LinearMath/Quaternion.h> // 提供了对四元数的操作
#include <tf2_ros/transform_broadcaster.h> // 提供了tf2的转换广播功能
#include <geometry_msgs/msg/transform_stamped.hpp> // 提供了TransformStamped消息类型
#include <turtlesim/msg/pose.hpp> // 提供了Pose消息类型

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv); // 初始化ROS2
    std::string turtle_name; // 定义一个字符串来保存乌龟的名字

    auto node = rclcpp::Node::make_shared("my_tf_broadcaster"); // 创建一个节点
    tf2_ros::TransformBroadcaster br(node); // 创建一个tf2的转换广播器

    // 从参数中获取乌龟的名字
    node->declare_parameter("turtle_name"); // 声明一个参数
    if (!node->get_parameter("turtle_name", turtle_name)) { // 获取参数值
        RCLCPP_ERROR(node->get_logger(), "need turtle name as parameter"); // 如果获取失败，打印错误信息
        return -1; // 并结束程序
    }

    // 定义一个回调函数来处理Pose消息
    auto poseCallback = [&](const turtlesim::msg::Pose::SharedPtr msg) {
        // 创建一个TransformStamped消息
        geometry_msgs::msg::TransformStamped transform_stamped;
        transform_stamped.header.stamp = rclcpp::Clock().now(); // 设置时间戳
        transform_stamped.header.frame_id = "world"; // 设置参考坐标系
        transform_stamped.child_frame_id = turtle_name; // 设置目标坐标系
        transform_stamped.transform.translation.x = msg->x; // 设置x方向的平移
        transform_stamped.transform.translation.y = msg->y; // 设置y方向的平移
        transform_stamped.transform.translation.z = 0.0; // 设置z方向的平移

        tf2::Quaternion q; // 创建一个四元数
        q.setRPY(0, 0, msg->theta); // 根据roll、pitch、yaw角度设置四元数的值
        // 设置旋转
        transform_stamped.transform.rotation.x = q.x();
        transform_stamped.transform.rotation.y = q.y();
        transform_stamped.transform.rotation.z = q.z();
        transform_stamped.transform.rotation.w = q.w();

        br.sendTransform(transform_stamped); // 发送转换
    };

    // 创建一个订阅器，订阅Pose消息
    auto sub = node->create_subscription<turtlesim::msg::Pose>(turtle_name + "/pose", 10, poseCallback);

    rclcpp::spin(node); // 处理ROS2的事件
    rclcpp::shutdown(); // 关闭ROS2
    return 0;
}
