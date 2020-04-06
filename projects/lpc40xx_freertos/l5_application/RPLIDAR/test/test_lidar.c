// #include "unity.h"

// #include <string.h>

// // Include the Mocks
// // - This will not pull the REAL source code of these modules (such as board_io.c)
// // - This will auto-generate "Mock" versions based on the header file
// #include "Mockcan_led.h"

// // Include the source we wish to test
// #include "lidar.c"

// void setUp(void) {}

// void tearDown(void) {}

// void test__direct_response_check(void) {
//   char bytes[] = {0xa5, 0x5a, 0x05, 0x00, 0x00, 0x40, 0x81};

//   for (int i = 0; i < 6; i++) {
//     TEST_ASSERT_FALSE(direct_response_check(bytes[i]));
//   }
//   TEST_ASSERT_TRUE(direct_response_check(bytes[6]));
// }
