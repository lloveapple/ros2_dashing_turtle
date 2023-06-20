# Docs

You can view our documentation to install environment and understand the code in our upload files. [click here](https://github.com/lloveapple/ros2_dashing_turtle/blob/main/%E6%96%87%E6%A1%A3.md)

# Run

### For ntf :

In one terminal

```shell
cd src/ntf/
source /opt/ros/dashing/setup.sh
colcon build
source install/setup.sh 
ros2 launch ntf ntf.py
```

In another terminal, open a package that controls the turtle. You may have to install this package yourself.

```shell
ros2 run teleop_twist_keyboard teleop_twist_keyboard cmd_vel:=/turtle1/cmd_vel
```

![image-20230620164628107](Readme.assets/image-20230620164628107.png)

## For tf :

In one terminal

```shell
cd src/tf/
source /opt/ros/dashing/setup.sh 
colcon build
source install/setup.sh 
ros2 launch tf tf.py
```

In another terminal, open a package that controls the turtle. You may have to install this package yourself.

```shell
ros2 run teleop_twist_keyboard teleop_twist_keyboard cmd_vel:=/turtle1/cmd_vel
```

![image-20230620164907206](Readme.assets/image-20230620164907206.png)

### For circle :

In  terminal

```shell
cd src/circle/
source /opt/ros/dashing/setup.sh 
colcon build
source install/setup.sh 
ros2 launch circle circle.py
```

![image-20230620165148437](Readme.assets/image-20230620165148437.png)
