# CMake generated Testfile for 
# Source directory: /home/kirato/OS_F1/OS2/libs/cJSON
# Build directory: /home/kirato/OS_F1/OS2/cmake-build-debug/libs/cJSON
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(cJSON_test "/home/kirato/OS_F1/OS2/cmake-build-debug/libs/cJSON/cJSON_test")
set_tests_properties(cJSON_test PROPERTIES  _BACKTRACE_TRIPLES "/home/kirato/OS_F1/OS2/libs/cJSON/CMakeLists.txt;252;add_test;/home/kirato/OS_F1/OS2/libs/cJSON/CMakeLists.txt;0;")
subdirs("tests")
subdirs("fuzzing")
