#include "error.hpp"

#include <tuple>
#include <vector>

namespace embed {
/**
 * @brief Helper utility for making mocks for class functions that return
 * boost::leaf::result<void>.
 *
 * This class stores records of a functions call history in order to be
 * recovered later for inspection in tests and simulations.
 *
 * See pwm_mock.hpp and tests/pwm_mock.test.cpp as an example of how this is
 * done in practice.
 *
 * @tparam args_t - the arguments of the class function
 */
template<typename... args_t>
class spy_handler
{
public:
  /**
   * @brief Set the record function to return an error after a specified number
   * of recordings.
   *
   * @param p_call_count_before_trigger - how many calls before an error is
   * returned.
   */
  void trigger_error_on_call(int p_call_count_before_trigger)
  {
    if (p_call_count_before_trigger < 0) {
      throw std::range_error("trigger_error_on_call() must be 0 or above");
    }
    m_error_trigger = p_call_count_before_trigger;
  }

  /**
   * @brief Record the arguments of a function being spied on.
   *
   * @param p_args - arguments to record
   * @return boost::leaf::result<void> - returns success or an error if the
   * error trigger has been reached.
   */
  [[nodiscard]] boost::leaf::result<void> record(args_t... p_args)
  {
    m_call_history.push_back(std::make_tuple(p_args...));

    if (m_error_trigger > 1) {
      m_error_trigger--;
    } else if (m_error_trigger == 1) {
      m_error_trigger--;
      return boost::leaf::new_error();
    }

    return {};
  }

  /**
   * @brief Return the call history of the save function
   *
   * @return const auto& - reference to the call history vector
   */
  const auto& call_history() const noexcept { return m_call_history; }

  /**
   * @brief Reset call recordings and turns off error trigger
   *
   */
  void reset() noexcept
  {
    m_call_history.clear();
    m_error_trigger = 0;
  }

private:
  std::vector<std::tuple<args_t...>> m_call_history{};
  int m_error_trigger = 0;
};
}  // namespace embed
