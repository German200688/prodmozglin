#include <iostream>
#include <cstdlib>
#include <librdkafka/rdkafka.h>
#include <csignal>
#include "database.h"
#include "commonD.cpp"
#include "log.hpp"



static volatile std::sig_atomic_t run4 = 1;

/**
 * @brief Signal termination of program
 */
static void stopD(int sig1) {
    run4 = 0;
}

static int kafkaProdD() {

    rd_kafka_t* consumerD;
    LOG_TRACE("10");
 
    rd_kafka_conf_t* confD;
    LOG_TRACE("20");
    rd_kafka_resp_err_t errD;
    LOG_TRACE("30");
    char errstrD[512];
    LOG_TRACE("40");
    // Create client configuration
    confD = rd_kafka_conf_new();
    LOG_TRACE("50");
    // User-specific properties that you must set
    set_configD(confD, "bootstrap.servers", "broker:9092");
    // Fixed properties
    set_configD(confD, "group.id", "prodmozg1");
    LOG_TRACE("110");
    set_configD(confD, "auto.offset.reset", "earliest");
    set_configD(confD, "client.id", "ea"); // ïàðàìåòð ïîêà íå íóæåí. Èñïîëüçóåòñÿ äëÿ èäåíòèôèêàöèè ñåðâåðà ãðóïïû êîíñüþìåðîâ
    LOG_TRACE("120");


    // Create the Consumer instance.
    consumerD = rd_kafka_new(RD_KAFKA_CONSUMER, confD, errstrD, sizeof(errstrD));

    if (!consumerD) {
        LOG_TRACE("1200");
        LOG_FATAL("Failed to create new consumer: errstr{}", errstrD);
        return 1;
    }
    rd_kafka_poll_set_consumer(consumerD);
    LOG_TRACE("130D");

    //   conf = NULL;
    LOG_TRACE("140D");

    // Convert the list of topics to a format suitable for librdkafka. 
    const char* topicD = "prodmozgromb2"; // ÒÎÏÈÊ!!!!!
    LOG_TRACE("150D");
    rd_kafka_topic_partition_list_t* subscription = rd_kafka_topic_partition_list_new(1);
    LOG_TRACE("160D");
    rd_kafka_topic_partition_list_add(subscription, topicD, RD_KAFKA_PARTITION_UA);
    LOG_TRACE("170D");
    // Subscribe to the list of topics.
    errD = rd_kafka_subscribe(consumerD, subscription);
    LOG_TRACE("180D");
    if (errD) {
        rd_kafka_topic_partition_list_destroy(subscription);
        rd_kafka_destroy(consumerD);
      //  LOG_FATAL("Failed to subscribe to subscription->cnt{} topics: rd_kafka_err2str(err){}", subscriptionD->cnt, rd_kafka_err2str(err));
        LOG_TRACE("1600");
        return 1;
    }

    rd_kafka_topic_partition_list_destroy(subscription);

    LOG_TRACE("190D");
    // Install a signal handler for clean shutdown.
    signal(SIGINT, stopD);
    LOG_TRACE("200D");
    // Start polling for messages.
    while (run4) {
        rd_kafka_message_t* consumer_messageD;

        LOG_TRACE("210");
        consumer_messageD = rd_kafka_consumer_poll(consumerD, 3000);
        if (!consumer_messageD) {
            //            LOG_TRACE("230 consumer_message{}", consumer_message); // íå ïîääåðæèâàåò ëîããåð òèï äàííûõ. Òîëüêî ÷åðåç std::cout
            LOG_INFO("WaitingDDDDDD...");
            rd_kafka_commit(consumerD, NULL, 0);
            continue;
        }

        if (consumer_messageD->err) {
            if (consumer_messageD->err == RD_KAFKA_RESP_ERR__PARTITION_EOF) {
                /* We can ignore this error - it just means we've read
                 * everything and are waiting for more data.
                 */
                LOG_TRACE("1700");
            }
            else {
                LOG_FATAL("Consumer error: rd_kafka_message_errstr(consumer_message){}", rd_kafka_message_errstr(consumer_messageD));
                return 1;
            }
        }
        else {
            LOG_TRACE("Consumed event from topic rd_kafka_topic_name(consumer_message->rkt){}: key = (int)consumer_message->key_len{} (char*)consumer_message->key{} value = (char*)consumer_message->payload{}",
                rd_kafka_topic_name(consumer_messageD->rkt), (int)consumer_messageD->key_len, (char*)consumer_messageD->key, (char*)consumer_messageD->payload);

            std::string ddcD = (char*)consumer_messageD->payload;
            Database dbcD;


            std::string bbcD = """select id from prodmozgd where name = '";

            std::string cccD = "';""";

            std::string aaacD = bbcD + ddcD + cccD;
std::cout << "Селект " << aaacD << std::endl;
            LOG_TRACE("aaacD{}", aaacD);
            dbcD.ExecuteSQLD(aaacD);
            LOG_INFO("sent to db");


        }

        // Free the message when we're done.
        rd_kafka_message_destroy(consumer_messageD);
    }

    // Close the consumer: commit final offsets and leave the group.
    LOG_INFO("Closing consumer");
    rd_kafka_consumer_close(consumerD);

    // Destroy the consumer.
    rd_kafka_destroy(consumerD);

    return 0;
}
