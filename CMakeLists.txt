cmake_minimum_required(VERSION 3.15)
project(prodmozg2)



find_package(spdlog REQUIRED)
find_package(Poco REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(RdKafka REQUIRED)


add_executable(${PROJECT_NAME} src/main.cpp src/log.hpp src/database.cpp src/database.h src/server.cpp src/server.h src/consumer.cpp src/common.cpp src/producer.cpp "src/consumerd.cpp" "src/commonD.cpp")
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} Poco::Poco)
target_link_libraries(${PROJECT_NAME} PostgreSQL::PostgreSQL)
target_link_libraries(${PROJECT_NAME} RdKafka::rdkafka++)
