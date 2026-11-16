#pragma once

#include <any>
#include <cstdint>
#include <string>

#include <onrobot_msgs/action/vg_command.hpp>

#include <capabilities2_events/event_parameters.hpp>
#include <capabilities2_runner/action_runner.hpp>

namespace onrobot_capabilities
{
class VGCommandRunner final : public capabilities2_runner::ActionRunner<onrobot_msgs::action::VGCommand>
{
public:
  void start(rclcpp::Node::SharedPtr node, const capabilities2_runner::runner_opts& run_config,
             const std::string& bond_id) override
  {
    init_action(node, run_config, "/gripper_command", "onrobot_msgs::action::VGCommand");
    emit_started(bond_id, "", param_on_started());
  }

protected:
  onrobot_msgs::action::VGCommand::Goal generate_goal(capabilities2_events::EventParameters& parameters) override
  {
    const int mode_a = std::any_cast<int>(parameters.get_value("mode_a", 1));
    const int mode_b = std::any_cast<int>(parameters.get_value("mode_b", 2));
    const int vacuum_a = std::any_cast<int>(parameters.get_value("target_vacuum_a_percent", 60));
    const int vacuum_b = std::any_cast<int>(parameters.get_value("target_vacuum_b_percent", 0));
    if (mode_a < 0 || mode_a > 2 || mode_b < 0 || mode_b > 2 ||
        vacuum_a < 0 || vacuum_a > 80 || vacuum_b < 0 || vacuum_b > 80)
    {
      throw capabilities2_runner::runner_exception("VG modes must be 0..2 and vacuum targets must be 0..80");
    }

    onrobot_msgs::action::VGCommand::Goal goal;
    goal.mode_a = static_cast<std::uint8_t>(mode_a);
    goal.mode_b = static_cast<std::uint8_t>(mode_b);
    goal.target_vacuum_a_percent = static_cast<std::uint8_t>(vacuum_a);
    goal.target_vacuum_b_percent = static_cast<std::uint8_t>(vacuum_b);
    return goal;
  }

  std::string generate_feedback(const onrobot_msgs::action::VGCommand::Feedback::ConstSharedPtr feedback) override
  {
    return "measured vacuum A=" + std::to_string(feedback->measured_vacuum_a_milli_relative) +
           ", B=" + std::to_string(feedback->measured_vacuum_b_milli_relative);
  }

  capabilities2_events::EventParameters param_on_success() override
  {
    return result_parameters();
  }

  capabilities2_events::EventParameters param_on_failure() override
  {
    return result_parameters();
  }

private:
  capabilities2_events::EventParameters result_parameters() const
  {
    capabilities2_events::EventParameters parameters;
    const int vacuum_a = result_ ? static_cast<int>(result_->measured_vacuum_a_milli_relative) : 0;
    const int vacuum_b = result_ ? static_cast<int>(result_->measured_vacuum_b_milli_relative) : 0;
    parameters.set_value("measured_vacuum_a_milli_relative", vacuum_a, capabilities2_events::OptionType::INT);
    parameters.set_value("measured_vacuum_b_milli_relative", vacuum_b, capabilities2_events::OptionType::INT);
    return parameters;
  }
};
}  // namespace onrobot_capabilities