#include <libhal/units.hpp>
#include <boost/ut.hpp>

namespace hal {

void g_force_test() {
    using namespace hal::literals;
    using namespace boost::ut;
    "g_force_type UDL test"_test = []() {
        // Set Up
        g_force test_g_force_value = 2.0_g;
        float control_value = 2.0;
        // Verify
        expect(test_g_force_value == control_value);
    };

    "g_force_type calculation test"_test = []() {
        // Setup
        float earth_accelereation_rate = 9.81; // m/s^2
        g_force earth_g_force = 1.0_g;
        
        // Test Multiplication and Division
       // g_force 
        g_force g_force_quotient = earth_accelereation_rate/9.81f;
        g_force g_force_product = 1.5_g * 2;

        // Attempting to sum g_force together
        g_force g_force_sum = 1.0_g + 1.5_g;
        g_force g_force_difference = 1.0_g - 1.5_g;

        // Verify
        expect(g_force_quotient == earth_g_force);
        expect(g_force_product == 3.0_g);
        expect(g_force_sum == 2.5_g);
        expect(g_force_difference == -0.5_g);
    };

    "g_force_type boundry test"_test = []() {
        // Set up
        g_force g_force_max = std::numeric_limits<g_force>::max();
        g_force g_force_min = std::numeric_limits<g_force>::min();

        // Control values
        float float_max = std::numeric_limits<float>::max();
        float float_min = std::numeric_limits<float>::min();

        // Verify
        expect(g_force_max == float_max);
        expect(g_force_min == float_min);
    };
}


} // namespace hal