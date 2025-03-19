cmake_minimum_required(VERSION 3.8)
project(nmp25)

enable_testing()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -lpthread)
endif()

foreach(IDX RANGE 1 3 1)
	add_executable(Question-${IDX}-out Question-${IDX}/Question-${IDX}.cc)
	target_link_libraries(Question-${IDX}-out pthread)
	add_test(question-${IDX} Question-${IDX}-out)
endforeach()

# Disable -Werror for GoogleTest if it's present
if (CMAKE_CXX_FLAGS MATCHES "-Werror")
    string(REPLACE "-Werror" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

# Question 4: Extension Task. You need to integrate Google Tests here
include(FetchContent)

# Fetch GoogleTest and GoogleMock
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.10.0  # You can specify any tag here
)

# Download and make available
FetchContent_MakeAvailable(googletest)

# Disable -Werror for GoogleTest build
set_target_properties(gtest PROPERTIES COMPILE_FLAGS "-Wno-error=maybe-uninitialized")
set_target_properties(gmock PROPERTIES COMPILE_FLAGS "-Wno-error=maybe-uninitialized")

# Question 4: Extension Task - Add GoogleTest integration
add_executable(Question-4-out Question-4/Question-4.cc)

# Link GoogleTest and GoogleMock with the test executable
target_link_libraries(Question-4-out gtest gmock gtest_main pthread)

# Add the test for Question 4
add_test(question-4 Question-4-out)