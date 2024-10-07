#include "server.h"
#include "log.hpp"
#include <cstdlib>
#include <iostream>
#include <spdlog/spdlog.h>
#include "consumer.cpp"
#include "consumerd.cpp"
#include <thread>


int main(int argc, char* argv[]) {

spdlog::set_level(spdlog::level::info);


    LOG_FATAL("Welcome to RobotF!");
    LOG_TRACE("Welcome to RobotT!");
    LOG_DEBUG("Welcome to RobotD!");
    LOG_INFO("Welcome to RobotI!");
    const int countdb_mas = 5;

/* Для отладки надо
    int MotorInst[countdb_mas] = {
       { 32 },
       { 1129 },
       { 4197 },
        {125},
        {152}
    };
    */
       LOG_INFO("Welcome to code!");

  TodoServerApp app;

    LOG_INFO("Welcome to kafka!");

   std::thread pkD(kafkaProdD);
   std::thread pk(kafkaProd);
        LOG_INFO("Welcome to http!");
        return app.run(argc, argv);
	pk.join();
return 0;
        
}

