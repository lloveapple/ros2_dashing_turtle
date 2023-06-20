from launch_ros.actions import Node
from launch.actions import ExecuteProcess
from launch import LaunchDescription
from turtlesim.srv import Spawn
from rclpy.node import Node as rclNode
import rclpy

def generate_launch_description():

    return LaunchDescription([
        Node(
            package='turtlesim',
            node_executable='turtlesim_node',
            node_name='sim',
        ),


        ExecuteProcess(
            cmd=['ros2', 'service', 'call', '/spawn', 'turtlesim/srv/Spawn', '{x: 8, y: 8, theta: 0, name: "turtle2"}'],
        ),

        # ros2 run teleop_twist_keyboard teleop_twist_keyboard cmd_vel:=/turtle1/cmd_vel

        Node(
            node_namespace = 'yx',
            package='circle',
            node_executable='circle',
            node_name='circle',
        ),
    ])
