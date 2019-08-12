include_directories(${PROJECT_SOURCE_DIR})

add_executable(CQF-deNoise CQF-deNoise.cpp)
target_link_libraries(CQF-deNoise core-lib base-lib z bz2 boost_system boost_thread boost_program_options boost_timer boost_chrono  pthread rt)

add_executable(cqf_operations cqf_operations.cpp)
target_link_libraries(cqf_operations core-lib base-lib z bz2 boost_system boost_thread boost_program_options boost_timer boost_chrono  pthread rt)
