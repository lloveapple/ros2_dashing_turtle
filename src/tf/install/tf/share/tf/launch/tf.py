from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='turtlesim',
            node_executable='turtlesim_node',
            node_name='sim',
        ),
        # ExecuteProcess(
        #     cmd=['ros2', 'service', 'call', '/spawn', 'turtlesim/srv/Spawn', '{x: 5, y: 5, theta: 0, name: "turtle2"}'],
        # ),
        # ros2 run teleop_twist_keyboard teleop_twist_keyboard cmd_vel:=/turtle1/cmd_vel

        # Node(
        #     package='turtlesim',
        #     node_executable='turtle_teleop_key',
        #     node_name='teleop',
        #     output='screen'
        # ),
        Node(
            package='tf',
            node_executable='tf_bro',
            node_name='turtle1_tf_broadcaster',
            parameters=[{'turtle_name': '/turtle1'}],
        ),
        Node(
            package='tf',
            node_executable='tf_bro',
            node_name='turtle2_tf_broadcaster',
            parameters=[{'turtle_name': '/turtle2'}],
        ),
        Node(
            package='tf',
            node_executable='tf_lis',
            node_name='listener',
        ),
    ])
