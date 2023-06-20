from launch_ros.actions import Node
from launch.actions import ExecuteProcess
from launch import LaunchDescription
from turtlesim.srv import Spawn
from rclpy.node import Node as rclNode
import rclpy
class SpawnTurtleClientNode(rclNode):
    def __init__(self, x=0.0, y=0.0, theta=0.0, name="turtle1"):
        super().__init__('spawn_turtle_client')
        self.client = self.create_client(Spawn, 'spawn')

    def call_service(self, x, y, theta, name):
        request = Spawn.Request()
        request.x = x
        request.y = y
        request.theta = theta
        request.name = name

        while not self.client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('service not available, waiting again...')

        self.future = self.client.call_async(request)


def generate_launch_description():
    # rclpy.init()
    # spawn_turtle_client_node1 = SpawnTurtleClientNode()
    # spawn_turtle_client_node2 = SpawnTurtleClientNode(x = 20.0, y= 20.0, name="turtle2")

    return LaunchDescription([
        Node(
            package='turtlesim',
            node_executable='turtlesim_node',
            node_name='sim',
        ),
        # ExecuteProcess(
        #     cmd=['ros2', 'service', 'call', '/spawn', 'turtlesim/srv/Spawn', '{x: 0, y: 0, theta: 0, name: "turtle1"}'],
        # ),

        ExecuteProcess(
            cmd=['ros2', 'service', 'call', '/spawn', 'turtlesim/srv/Spawn', '{x: 5, y: 5, theta: 0, name: "turtle2"}'],
        ),
        # ros2 run teleop_twist_keyboard teleop_twist_keyboard cmd_vel:=/turtle1/cmd_vel
        # Node(
        #     package='teleop_twist_keyboard',
        #     node_executable='teleop_twist_keyboard',
        #     node_name='teleop',
        #     remappings=[
        #         ('cmd_vel', '/turtle1/cmd_vel'),
        #     ],
        #     output='screen',
        # ),

        Node(
            node_namespace = 'yx',
            package='ntf',
            node_executable='ntf',
            node_name='ntf',
        ),
    ])
