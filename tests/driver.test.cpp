#include <boost/ut.hpp>
#include <cstdio>
#include <libembeddedhal/driver.hpp>

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

class driver_impl : public driver<>
{
public:
  boost::leaf::result<void> errorable_function()
  {
    auto on_error = embed::error::setup();
    return boost::leaf::new_error(dummy_error{});
  }

  boost::leaf::result<void> driver_initialize()
  {
    auto on_error = embed::error::setup();
    return errorable_function();
  }
};

boost::ut::suite driver_test = []() {
  using namespace boost::ut;

  driver_impl impl;

  boost::leaf::result<void> result = boost::leaf::try_handle_some(
    [&impl]() -> boost::leaf::result<void> {
      EMBED_CHECK(impl.initialize());
      return {};
    },
    [](dummy_error const&,
       embed::error::stacktrace const* trace,
       boost::leaf::e_source_location const* location) {
      puts("Error: dummy_error{} occured! Logging this to stdout!");
      print_trace(trace);
      if (location) {
        printf(
          "Error Source Location: %s:%d\n", location->file, location->line);
      }
    },
    []() { puts("Unknown error!?"); });
};
}  // namespace embed
