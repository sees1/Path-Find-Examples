#include <rclcpp/rclcpp.hpp>

#include "lifecycle_msgs/msg/transition.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"

#include "nav2_costmap_2d/costmap_2d_ros.hpp"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"

#include <string>

class PathManager : public rclcpp_lifecycle::LifecycleNode
{
private:
  using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

public:
  explicit PathManager(const std::string& name = "path_manager")
  : rclcpp_lifecycle::LifecycleNode(name)
  { }

  CallbackReturn on_configure(const rclcpp_lifecycle::State & state)
  {
    algorithm_ = declare_parameter("Algorithm", "A*");
    road_mode_ = declare_parameter("RoadMode", false);
    costmap_mode_ = declare_parameter("CostmapMode", true);

    tf_buffer_ = std::make_shared<tf2_ros::Buffer>(get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

    costmap_ = std::make_shared<nav2_costmap_2d::Costmap2DROS>("path_costmap", tf_buffer_);

    costmap_->configure();

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State & state)
  {
    costmap_->activate();

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State & state)
  {
    costmap_->deactivate();

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State & state)
  {
    costmap_->cleanup();

    return CallbackReturn::SUCCESS;
  }


private: // ros params
  std::string algorithm_;
  bool road_mode_;
  bool costmap_mode_;

private:
  std::shared_ptr<nav2_costmap_2d::Costmap2DROS> costmap_;
  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
};