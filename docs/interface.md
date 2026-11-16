# OnRobot Capability Interfaces

RG2 and RG6 share the generic gripper launch, command, and state interfaces.
Their providers differ only in the model argument and aperture range. Endpoints
are model-neutral so a user-started Gazebo simulation can replace the physical
gripper without changing plans.

| Capability | Endpoint | Models |
| --- | --- | --- |
| RG launch | `/gripper_command` (`control_msgs/action/GripperCommand`) | RG2, RG6 |
| RG command | `/gripper_command` (`control_msgs/action/GripperCommand`) | RG2, RG6 |
| RG simulation state | `/gripper_joint_states` (`sensor_msgs/msg/JointState`) | RG2, RG6 |
| VG command | `/gripper_command` (`onrobot_msgs/action/VGCommand`) | VG10, VGC10 |

Run the matching `onrobot_control` launch capability to start control software
for hardware. It does not start Gazebo. For simulation, start Gazebo separately
with the desired model and use the same command and state endpoints; do not also
start the hardware control launch provider. Only one gripper may claim the
model-neutral endpoints in a ROS graph at a time.

RG command positions are aperture in meters: RG2 supports up to 0.11 m and RG6
up to 0.16 m. Gazebo does not model force, so use `max_effort=0`. The state
provider reads the simulated aperture stream; hardware state is not advertised
as `JointState` by the current driver.

VG commands use modes `0=release`, `1=grip`, and `2=idle`, with vacuum targets
from 0 to 80 percent. VGC10 simulation requires cup contact with a supported
world object before accepting a grip goal. The current hardware VG action server
is an offline-only stub and rejects goals until a verified execution backend is
available; VG10 command execution is therefore not hardware-validated.