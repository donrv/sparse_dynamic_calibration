FROM ros:melodic

RUN apt-get update && apt-get install -y --no-install-recommends && apt-get install -y --no-install-recommends git wget nano libopencv-dev build-essential ros-melodic-cv-bridge ros-melodic-pcl-ros ros-melodic-rviz ros-melodic-tf-conversions ros-melodic-libg2o \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /root
RUN git clone https://github.com/AprilRobotics/apriltags.git
RUN cd apriltags && make -j4 install

RUN mkdir -p /root/catkin_ws/src
WORKDIR /root/catkin_ws/src
RUN /bin/bash -c '. /opt/ros/melodic/setup.bash; catkin_init_workspace'
COPY . /root/catkin_ws/src/sparse_dynamic_calibration/

WORKDIR /root/catkin_ws
RUN /bin/bash -c '. /opt/ros/melodic/setup.bash; catkin_make'
RUN sed -i "6i source \"/root/catkin_ws/devel/setup.bash\"" /ros_entrypoint.sh

WORKDIR /

ENTRYPOINT ["/ros_entrypoint.sh"]
CMD ["bash"]
