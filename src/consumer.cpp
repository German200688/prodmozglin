#include <iostream>
#include <cstdlib>
#include <librdkafka/rdkafka.h>
#include <csignal>
#include "database.h"
#include "common.cpp"
#include "log.hpp"



static volatile std::sig_atomic_t run = 1;

/**
 * @brief Signal termination of program
 */
static void stop(int sig) {
    run = 0;
}

static int kafkaProd() {

    rd_kafka_t* consumer; 
    LOG_TRACE("10");
    rd_kafka_conf_t* conf;
    LOG_TRACE("20");
    rd_kafka_resp_err_t err;
    LOG_TRACE("30");
    char errstr[512];
    LOG_TRACE("40");
    // Create client configuration
    conf = rd_kafka_conf_new();
    LOG_TRACE("50");
    // User-specific properties that you must set
    set_config(conf, "bootstrap.servers", "broker:9092");
    // Fixed properties
    set_config(conf, "group.id", "prodmozg1");
    LOG_TRACE("110");
    set_config(conf, "auto.offset.reset", "earliest");
    set_config(conf, "client.id", "ea"); // параметр пока не нужен. Используется для идентификации сервера группы консьюмеров
    LOG_TRACE("120");


    // Create the Consumer instance.
    consumer = rd_kafka_new(RD_KAFKA_CONSUMER, conf, errstr, sizeof(errstr));

    if (!consumer) {
        LOG_TRACE("1200");
        LOG_FATAL("Failed to create new consumer: errstr{}", errstr);
        return 1;
    }
    rd_kafka_poll_set_consumer(consumer); 
    LOG_TRACE("130");

 //   conf = NULL;
    LOG_TRACE("140");

    // Convert the list of topics to a format suitable for librdkafka. 
    const char* topic = "prodmozgm"; // ТОПИК!!!!!
    LOG_TRACE("150");
    rd_kafka_topic_partition_list_t* subscription = rd_kafka_topic_partition_list_new(1); 
    LOG_TRACE("160");
    rd_kafka_topic_partition_list_add(subscription, topic, RD_KAFKA_PARTITION_UA);
    LOG_TRACE("170");
    // Subscribe to the list of topics.
    err = rd_kafka_subscribe(consumer, subscription); 
    LOG_TRACE("180");
    if (err) {
        rd_kafka_topic_partition_list_destroy(subscription);
        rd_kafka_destroy(consumer);
        LOG_FATAL("Failed to subscribe to subscription->cnt{} topics: rd_kafka_err2str(err){}", subscription->cnt, rd_kafka_err2str(err));
        LOG_TRACE("1600");
        return 1;
    }

    rd_kafka_topic_partition_list_destroy(subscription);

    LOG_TRACE("190");
    // Install a signal handler for clean shutdown.
    signal(SIGINT, stop);
    LOG_TRACE("200");
    // Start polling for messages.
    while (run) {
        rd_kafka_message_t* consumer_message; 

        LOG_TRACE("210");
        consumer_message = rd_kafka_consumer_poll(consumer, 3000);
        if (!consumer_message) {
//            LOG_TRACE("230 consumer_message{}", consumer_message); // не поддерживает логгер тип данных. Только через std::cout
            LOG_INFO("Waiting...");
            rd_kafka_commit(consumer, NULL, 0);
            continue;
        }

        if (consumer_message->err) {
            if (consumer_message->err == RD_KAFKA_RESP_ERR__PARTITION_EOF) {
                /* We can ignore this error - it just means we've read
                 * everything and are waiting for more data.
                 */
                LOG_TRACE("1700");
            }
            else {
                LOG_FATAL("Consumer error: rd_kafka_message_errstr(consumer_message){}", rd_kafka_message_errstr(consumer_message));
                return 1;
            }
        }
        else {
            LOG_TRACE("Consumed event from topic rd_kafka_topic_name(consumer_message->rkt){}: key = (int)consumer_message->key_len{} (char*)consumer_message->key{} value = (char*)consumer_message->payload{}",
                rd_kafka_topic_name(consumer_message->rkt), (int)consumer_message->key_len, (char*)consumer_message->key, (char*)consumer_message->payload);

            std::string ddc = (char*)consumer_message->payload;
            Database dbc;


            std::string bbc = """select id from prodmozgm where name = '";
            
            std::string ccc = "';""";

            std::string aaac = bbc + ddc + ccc;

            LOG_TRACE("aaac{}", aaac);
            dbc.ExecuteSQL(aaac);
            LOG_INFO("sent to db");
            

        }

        // Free the message when we're done.
        rd_kafka_message_destroy(consumer_message);
    }

    // Close the consumer: commit final offsets and leave the group.
    LOG_INFO("Closing consumer");
    rd_kafka_consumer_close(consumer);

    // Destroy the consumer.
    rd_kafka_destroy(consumer);

    return 0;
}
