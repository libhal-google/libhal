#include <libhal/streams.hpp>

#include <boost/ut.hpp>

namespace hal::stream {
namespace {

struct example_stream
{
  work_state state()
  {
    return m_state;
  }

  friend std::span<const hal::byte> operator|(
    const std::span<const hal::byte>& p_input_data,
    [[maybe_unused]] example_stream& p_self)
  {
    return p_input_data;
  }

  work_state m_state;
};
}  // namespace

boost::ut::suite stream_terminated_test = []() {
  using namespace boost::ut;
  using namespace std::literals;

  static_assert(byte_stream<example_stream>);
  static_assert(byte_stream<parse<size_t>>);
  static_assert(byte_stream<find>);
  static_assert(byte_stream<fill_upto>);
  static_assert(byte_stream<skip>);

  "hal::terminate(byte_stream) -> true with finished state"_test = []() {
    // Setup
    example_stream stream;
    stream.m_state = work_state::finished;

    // Exercise
    bool result = terminated(stream);

    // Verify
    expect(result) << "Should return true as the work_state is 'finished'!";
  };

  "hal::terminate(byte_stream) -> true with failed state"_test = []() {
    // Setup
    example_stream stream;
    stream.m_state = work_state::failed;

    // Exercise
    bool result = terminated(stream);

    // Verify
    expect(result) << "Should return true as the work_state is 'failed'!";
  };

  "hal::terminate(byte_stream) -> false with in_progress state"_test = []() {
    // Setup
    example_stream stream;
    stream.m_state = work_state::in_progress;

    // Exercise
    bool result = terminated(stream);

    // Verify
    expect(!result)
      << "Should return false as the work_state is 'in_progress'!";
  };
};

// =============================================================================
//
//                              |  parse<T> Stream  |
//
// =============================================================================
boost::ut::suite parse_stream_test = []() {
  using namespace boost::ut;
  using namespace std::literals;

  "[parse<std::uint32_t>] normal usage"_test = []() {
    // Setup
    std::string_view digits_in_between = "abcd1234x---";
    auto digits_span = hal::as_bytes(digits_in_between);
    parse<std::uint32_t> parse_int;

    // Exercise
    auto remaining = digits_span | parse_int;

    expect(that % work_state::finished == parse_int.state());
    expect(that % 1234 == parse_int.value());
    expect(that % digits_span.subspan(digits_in_between.find("x")).size() ==
           remaining.size());
    expect(that % digits_span.subspan(digits_in_between.find("x")).data() ==
           remaining.data());
  };

  "[parse<std::uint64_t>] normal usage"_test = []() {
    // Setup
    std::string_view digits_in_between = "abcd12356789101234x---";
    auto digits_span = hal::as_bytes(digits_in_between);
    parse<std::uint64_t> parse_int;

    // Exercise
    auto remaining = digits_span | parse_int;

    expect(that % work_state::finished == parse_int.state());
    expect(that % 12'356'789'101'234ULL == parse_int.value());
    expect(that % digits_span.subspan(digits_in_between.find("x")).size() ==
           remaining.size());
    expect(that % digits_span.subspan(digits_in_between.find("x")).data() ==
           remaining.data());
  };

  "[parse<std::uint32_t>] empty span"_test = []() {
    // Setup
    parse<std::uint32_t> parse_int;

    // Exercise
    auto remaining = std::span<const hal::byte>() | parse_int;

    expect(that % work_state::in_progress == parse_int.state());
    expect(that % 0 == parse_int.value());
    expect(that % 0 == remaining.size());
    expect(that % nullptr == remaining.data());
  };

  "[parse<std::uint32_t>] no digits"_test = []() {
    // Setup
    std::string_view digits_in_between = "abcd?efghx-[--]/";
    auto digits_span = hal::as_bytes(digits_in_between);
    parse<std::uint32_t> parse_int;

    // Exercise
    auto remaining = digits_span | parse_int;

    expect(that % work_state::in_progress == parse_int.state());
    expect(that % 0 == parse_int.value());
    expect(that % 0 == remaining.size());
    expect(that % &(*digits_span.end()) == remaining.data());
  };

  "[parse<std::uint32_t>] two blocks"_test = []() {
    // Setup
    std::array<std::string_view, 2> halves = { "abc12", "98ce" };
    auto span0 = hal::as_bytes(halves[0]);
    auto span1 = hal::as_bytes(halves[1]);

    parse<std::uint32_t> parse_int;

    auto remaining0 = span0 | parse_int;
    auto remaining1 = span1 | parse_int;

    expect(that % work_state::finished == parse_int.state());
    expect(that % 1298 == parse_int.value());
    expect(that % 0 == remaining0.size());
    expect(that % &(*span0.end()) == remaining0.data());
    expect(that % span1.subspan(halves[1].find("c")).size() ==
           remaining1.size());
    expect(that % span1.subspan(halves[1].find("c")).data() ==
           remaining1.data());
  };

  "[parse<std::uint32_t>] three blocks"_test = []() {
    // Setup
    std::array<std::string_view, 3> halves = { "abc1", "23", "45ce" };
    auto span0 = hal::as_bytes(halves[0]);
    auto span1 = hal::as_bytes(halves[1]);
    auto span2 = hal::as_bytes(halves[2]);

    parse<std::uint32_t> parse_int;

    auto remaining0 = span0 | parse_int;
    auto remaining1 = span1 | parse_int;
    auto remaining2 = span2 | parse_int;

    expect(that % work_state::finished == parse_int.state());
    expect(that % 12345 == parse_int.value());
    expect(that % 0 == remaining0.size());
    expect(that % &(*span0.end()) == remaining0.data());
    expect(that % 0 == remaining1.size());
    expect(that % &(*span1.end()) == remaining1.data());
    expect(that % span2.subspan(halves[2].find("c")).size() ==
           remaining2.size());
    expect(that % span2.subspan(halves[2].find("c")).data() ==
           remaining2.data());
  };

  "[parse<std::uint32_t>] 2 blocks, 1 empty"_test = []() {
    // Setup
    std::array<std::string_view, 3> halves = { "abc12", "", "45ce" };
    auto span0 = hal::as_bytes(halves[0]);
    auto span1 = hal::as_bytes(halves[1]);
    auto span2 = hal::as_bytes(halves[2]);

    parse<std::uint32_t> parse_int;

    auto remaining0 = span0 | parse_int;
    auto remaining1 = span1 | parse_int;
    auto remaining2 = span2 | parse_int;

    expect(that % work_state::finished == parse_int.state());
    expect(that % 1245 == parse_int.value());
    expect(that % 0 == remaining0.size());
    expect(that % &(*span0.end()) == remaining0.data());
    expect(that % 0 == remaining1.size());
    expect(that % &(*span1.end()) == remaining1.data());
    expect(that % span2.subspan(halves[2].find("c")).size() ==
           remaining2.size());
    expect(that % span2.subspan(halves[2].find("c")).data() ==
           remaining2.data());
  };

  "[parse<std::uint32_t>] chain of three"_test = []() {
    // Setup
    std::string_view three_numbers = "a123b456c789d";
    auto span = hal::as_bytes(three_numbers);

    parse<std::uint32_t> parse_int0;
    parse<std::uint32_t> parse_int1;
    parse<std::uint32_t> parse_int2;

    // Exercise
    auto remaining = span | parse_int0 | parse_int1 | parse_int2;

    expect(that % work_state::finished == parse_int0.state());
    expect(that % work_state::finished == parse_int1.state());
    expect(that % work_state::finished == parse_int2.state());
    expect(that % 123 == parse_int0.value());
    expect(that % 456 == parse_int1.value());
    expect(that % 789 == parse_int2.value());
    expect(that % 1 == remaining.size());
    // 'd' is left in the end
    expect(that % &span.back() == remaining.data());
  };
};

// =============================================================================
//
//                               |  Find Stream  |
//
// =============================================================================
boost::ut::suite find_stream_test = []() {
  // Setup
  using namespace boost::ut;
  using namespace std::literals;

  "[find] normal usage"_test = []() {
    // Setup
    std::string_view str = "Content-Length: 1023\r\n";
    auto span = hal::as_bytes(str);
    find finder(hal::as_bytes(": "sv));

    // Exercise
    auto remaining = span | finder;

    expect(that % work_state::finished == finder.state());
    expect(that % span.subspan(str.find(": ") + 1).size() == remaining.size());
    expect(that % span.subspan(str.find(": ") + 1).data() == remaining.data());
  };

  "[find] empty span"_test = []() {
    // Setup
    find finder(hal::as_bytes(": "sv));

    // Exercise
    auto remaining = std::span<hal::byte>() | finder;

    expect(that % work_state::in_progress == finder.state());
    expect(that % 0 == remaining.size());
    expect(that % nullptr == remaining.data());
  };

  "[find] nothing"_test = []() {
    // Setup
    std::string_view digits_in_between = "abcd??efghx-[--]/";
    auto digits_span = hal::as_bytes(digits_in_between);

    find finder(hal::as_bytes("????"sv));

    // Exercise
    auto remaining = digits_span | finder;

    expect(that % 0 == remaining.size());
    expect(that % &(*digits_span.end()) == remaining.data());
  };

  "[find] two blocks"_test = []() {
    // Setup
    std::array<std::string_view, 2> halves = { "abc12", "98ce" };
    auto span0 = hal::as_bytes(halves[0]);
    auto span1 = hal::as_bytes(halves[1]);

    find finder(hal::as_bytes("1298"sv));

    auto remaining0 = span0 | finder;
    auto remaining1 = span1 | finder;

    expect(that % 0 == remaining0.size());
    expect(that % &(*span0.end()) == remaining0.data());
    expect(that % span1.subspan(halves[1].find("8")).size() ==
           remaining1.size());
    expect(that % span1.subspan(halves[1].find("8")).data() ==
           remaining1.data());
  };

  "[find] three blocks"_test = []() {
    // Setup
    std::array<std::string_view, 3> halves = { "abc1", "23", "45ce" };
    auto span0 = hal::as_bytes(halves[0]);
    auto span1 = hal::as_bytes(halves[1]);
    auto span2 = hal::as_bytes(halves[2]);

    find finder(hal::as_bytes("345"sv));

    auto remaining0 = span0 | finder;
    auto remaining1 = span1 | finder;
    auto remaining2 = span2 | finder;

    expect(that % 0 == remaining0.size());
    expect(that % &(*span0.end()) == remaining0.data());
    expect(that % 0 == remaining1.size());
    expect(that % &(*span1.end()) == remaining1.data());
    expect(that % span2.subspan(halves[2].find("5")).size() ==
           remaining2.size());
    expect(that % span2.subspan(halves[2].find("5")).data() ==
           remaining2.data());
  };

  "[find] chain of three"_test = []() {
    // Setup
    std::string_view three_numbers = "----a---b---c";
    auto span = hal::as_bytes(three_numbers);

    find finder0(hal::as_bytes("a"sv));
    find finder1(hal::as_bytes("b"sv));
    find finder2(hal::as_bytes("c"sv));

    // Exercise
    auto remaining = span | finder0 | finder1 | finder2;

    expect(that % 1 == remaining.size());
    // 'd' is left in the end
    expect(that % &span.back() == remaining.data());
  };
};

// =============================================================================
//
//                             |  fill_upto Stream  |
//
// =============================================================================
boost::ut::suite fill_upto_stream_test = []() {
  // Setup
  using namespace boost::ut;
  using namespace std::literals;

  "[fill_upto] normal usage"_test = []() {
    // Setup
    std::string_view str = "some data#_$more data";
    auto span = hal::as_bytes(str);
    std::string_view expected_str = "some data#_$";
    auto expected = hal::as_bytes(expected_str);

    std::array<hal::byte, 128> buffer;
    auto target_string = "#_$"sv;
    auto target = hal::as_bytes(target_string);
    fill_upto filler(target, buffer);

    // Exercise
    auto remaining = span | filler;

    // Verify
    expect(that % work_state::finished == filler.state());
    expect(
      that %
        span.subspan(str.find(target_string) + target_string.size()).size() ==
      remaining.size());
    expect(
      that %
        span.subspan(str.find(target_string) + target_string.size()).data() ==
      remaining.data());
    expect(that %
             span.subspan(0, str.find(target_string) + target_string.size())
               .size() ==
           filler.span().size());
    expect(std::equal(expected.begin(),
                      expected.end(),
                      buffer.begin(),
                      buffer.begin() + expected.size()));
  };

  "[fill_upto] two blocks usage"_test = []() {
    // Setup
    std::array<std::string_view, 2> str = { "some data#"sv, "_$more data"sv };
    std::string_view expected_str = "some data#_$";
    auto expected = hal::as_bytes(expected_str);
    std::array span{ hal::as_bytes(str[0]), hal::as_bytes(str[1]) };

    std::array<hal::byte, 128> buffer;
    fill_upto filler(hal::as_bytes("#_$"sv), buffer);

    // Exercise
    auto remaining0 = span[0] | filler;
    auto remaining1 = span[1] | filler;

    // Verify
    expect(that % work_state::finished == filler.state());
    expect(that % 0 == remaining0.size());
    expect(that % &(*span[0].end()) == remaining0.data());
    expect(that % (span[1].size() - 2) == remaining1.size());
    expect(that % (span[1].data() + 2) == remaining1.data());
    expect(std::equal(expected.begin(),
                      expected.end(),
                      buffer.begin(),
                      buffer.begin() + expected.size()));
  };
};

// =============================================================================
//
//                               |  Multi Stream Test  |
//
// =============================================================================
boost::ut::suite multi_stream_test = []() {
  using namespace boost::ut;
  using namespace std::literals;

  "[✨ multi ✨] http request"_test = []() {
    std::string_view str = "HTTP/1.1 200 OK\r\n"
                           "Content-Encoding: gzip\r\n"
                           "Accept-Ranges: bytes\r\n"
                           "Age: 501138\r\n"
                           "Cache-Control: max-age=604800\r\n"
                           "Content-Type: text/html; charset=UTF-8\r\n"
                           "Date: Thu, 17 Nov 2022 06:19:47 GMT\r\n"
                           "Etag: \" 3147526947 \"\r\n"
                           "Expires: Thu, 24 Nov 2022 06:19:47 GMT\r\n"
                           "Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT\r\n"
                           "Server: ECS (sab/56CE)\r\n"
                           "Vary: Accept-Encoding\r\n"
                           "X-Cache: HIT\r\n"
                           "Content-Length: 47\r\n"
                           "\r\n"
                           "<html><body><h1>Hello, World</h1></body></html>";

    auto input_data = hal::as_bytes(str);
    std::array<hal::byte, 1024> response_buffer;
    response_buffer.fill('.');

    find find_content_length(hal::as_bytes("Content-Length: "sv));
    parse<std::uint32_t> parse_body_length;
    fill_upto find_end_of_header(hal::as_bytes("\r\n\r\n"sv), response_buffer);

    [[maybe_unused]] auto start_of_body =
      input_data | find_content_length | parse_body_length;

    [[maybe_unused]] auto remaining = input_data | find_end_of_header;

    if (terminated(find_end_of_header.state())) {
      fill fill_buffer(find_end_of_header.unfilled(),
                       parse_body_length.value());
      remaining = remaining | fill_buffer;
    }

    /*
    printf("[Content-Length]: %u, %d\n",
           static_cast<int>(parse_body_length.value()),
           static_cast<int>(response_buffer.size() - remaining.size()));

    printf("response_buffer:\r\n%.*s\n",
           static_cast<int>(response_buffer.size() - remaining.size()),
           response_buffer.data());
    printf("remaining = %.*s\n",
           static_cast<int>(remaining.size()),
           remaining.data());
    */
  };
};

boost::ut::suite stream_try_until_test = []() {
  using namespace boost::ut;
  using namespace std::literals;

  "[try_until] normal usage"_test = []() {
    // Setup
    std::string_view digits_in_between = "abcd1234x---";
    parse<std::uint32_t> parse_int;
    auto getter = [&digits_in_between]() -> std::span<const hal::byte> {
      return std::span<const hal::byte>(
        reinterpret_cast<const hal::byte*>(digits_in_between.data()),
        digits_in_between.size());
    };

    constexpr int timeout_call_limit = 10;
    int counts = 0;
    auto timeout_function = [&counts]() mutable -> status {
      counts++;
      if (counts >= timeout_call_limit) {
        return hal::new_error(std::errc::timed_out);
      }
      return {};
    };

    // Exercise
    auto result = try_until(parse_int, getter, timeout_function).value();

    // Verify
    expect(that % work_state::finished == result);
  };

  "[try_until] complete before timeout"_test = []() {
    // Setup
    std::string_view digits_in_between = "abcd1234x---";
    parse<std::uint32_t> parse_int;
    auto getter = [&digits_in_between]() -> std::span<const hal::byte> {
      return std::span<const hal::byte>(
        reinterpret_cast<const hal::byte*>(digits_in_between.data()),
        digits_in_between.size());
    };

    auto timeout_function = []() mutable -> status { return {}; };

    // Exercise
    auto result = try_until(parse_int, getter, timeout_function).value();

    // Verify
    expect(that % work_state::finished == result);
  };

  "[try_until] stream in multiple parts"_test = []() {
    // Setup
    std::string_view stream_part1 = "abcd1";
    std::string_view stream_part2 = "123";
    std::string_view stream_part3 = "4x---";
    int counts = 0;
    parse<std::uint32_t> parse_int;
    auto getter = [&counts,
                   stream_part1,
                   stream_part2,
                   stream_part3]() -> std::span<const hal::byte> {
      if (counts == 0) {
        return std::span<const hal::byte>(
          reinterpret_cast<const hal::byte*>(stream_part1.data()),
          stream_part1.size());
      }
      if (counts == 1) {
        return std::span<const hal::byte>(
          reinterpret_cast<const hal::byte*>(stream_part2.data()),
          stream_part2.size());
      }
      return std::span<const hal::byte>(
        reinterpret_cast<const hal::byte*>(stream_part3.data()),
        stream_part3.size());
    };

    constexpr int timeout_call_limit = 10;
    auto timeout_function = [&counts]() mutable -> status {
      counts++;
      if (counts >= timeout_call_limit) {
        return hal::new_error(std::errc::timed_out);
      }
      return {};
    };

    // Exercise
    auto result = try_until(parse_int, getter, timeout_function).value();

    // Verify
    expect(that % work_state::finished == result);
    expect(that % 2 == counts);
  };

  "[try_until] timed out"_test = []() {
    // Setup
    parse<std::uint32_t> parse_int;
    auto getter = []() -> std::span<const hal::byte> {
      return std::span<const hal::byte>();
    };

    constexpr int timeout_call_limit = 2;
    int counts = 0;
    auto timeout_function = [&counts]() mutable -> status {
      counts++;
      if (counts >= timeout_call_limit) {
        return hal::new_error(std::errc::timed_out);
      }
      return {};
    };

    // Exercise
    auto result = try_until(parse_int, getter, timeout_function);

    // Verify
    expect(!bool{ result });
    expect(that % 2 == counts);
  };

  "[try_until] timeout in multiple parts"_test = []() {
    // Setup
    std::string_view stream_part1 = "abcd";
    std::string_view stream_part2 = "1234";
    std::string_view stream_part3 = "x---";
    int counts = 0;
    parse<std::uint32_t> parse_int;
    auto getter = [&counts,
                   stream_part1,
                   stream_part2,
                   stream_part3]() -> std::span<const hal::byte> {
      if (counts == 0) {
        return std::span<const hal::byte>(
          reinterpret_cast<const hal::byte*>(stream_part1.data()),
          stream_part1.size());
      }
      if (counts == 1) {
        return std::span<const hal::byte>(
          reinterpret_cast<const hal::byte*>(stream_part2.data()),
          stream_part2.size());
      }
      return std::span<const hal::byte>(
        reinterpret_cast<const hal::byte*>(stream_part3.data()),
        stream_part3.size());
    };
    constexpr int timeout_call_limit = 1;
    auto timeout_function = [&counts]() mutable -> status {
      counts++;
      if (counts >= timeout_call_limit) {
        return hal::new_error(std::errc::timed_out);
      }
      return {};
    };

    // Exercise
    auto result = try_until(parse_int, getter, timeout_function);

    // Verify
    expect(!bool{ result });
    expect(that % 1 == counts);
  };
};
}  // namespace hal::stream
