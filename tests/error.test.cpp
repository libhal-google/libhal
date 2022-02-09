#include <boost/ut.hpp>
#include <cstdio>
#include <libembeddedhal/gpio/input_pin.hpp>

namespace embed {

void print_trace(error::stacktrace const* p_stacktrace)
{
  if constexpr (config::get_stacktrace_on_error) {
    if (p_stacktrace) {
      printf("Error Trace: \n\n");
      for (const auto& trace : p_stacktrace->get()) {
        printf("  ^ %s\n", trace.function);
        if constexpr (config::get_source_position_on_error) {
          printf("    %s:%d\n\n", trace.file, trace.line);
        }
      }
      puts("");
    }
  } else {
    puts("Tracing disabled!");
  }
}

struct dummy_error
{};

class input_pin_impl : public input_pin
{
private:
  [[nodiscard]] boost::leaf::result<void> errorable_function()
  {
    auto on_error = embed::error::setup();
    return boost::leaf::new_error(dummy_error{});
  }
  [[nodiscard]] boost::leaf::result<void> driver_configure(
    const settings&) noexcept override
  {
    auto on_error = embed::error::setup();
    return errorable_function();
  }
  [[nodiscard]] boost::leaf::result<bool> driver_level() noexcept override
  {
    return false;
  }
};

boost::ut::suite driver_test = []() {
  using namespace boost::ut;

  input_pin_impl impl;

  boost::leaf::try_handle_all(
    [&impl]() -> boost::leaf::result<void> {
      auto on_error = embed::error::setup();
      EMBED_CHECK(impl.configure({}));
      return {};
    },
    [](dummy_error const&,
       embed::error::stacktrace const* trace,
       boost::leaf::e_source_location const* location) {
      puts("Error: dummy_error{} occurred! Logging this to stdout!");
      print_trace(trace);
      if (location) {
        printf(
          "Error Source Location: %s:%d\n", location->file, location->line);
      }
    },
    []() { puts("Unknown error!?"); });
};
}  // namespace embed
