
#include <iostream>
#include <cstdlib>
#include <librdkafka/rdkafka.h>
#include <csignal>
#include <cstring>
#include "database.h"


#include "common.cpp"

#define ARR_SIZE(arr) ( sizeof((arr)) / sizeof((arr[0])) )



static void dr_msg_cb(rd_kafka_t* kafka_handle,
    const rd_kafka_message_t* rkmessage,
    void* opaque) {
    if (rkmessage->err) {
        std::cout << "Message delivery failed: " << rd_kafka_err2str(rkmessage->err) << std::endl;
        LOG_ERROR("Message delivery failed: rd_kafka_err2str(rkmessage->err){}", rd_kafka_err2str(rkmessage->err));
    }
}

static int director(char* prodmotor) {
    std::cout << "prodmotor = " << prodmotor << std::endl;
    LOG_TRACE("prodmotor = prodmotor{}", prodmotor);
    rd_kafka_t* producer;
    rd_kafka_conf_t* conf;
    char errstr[512];

    // Create client configuration
    conf = rd_kafka_conf_new();

    // User-specific properties that you must set
    set_config(conf, "bootstrap.servers", "broker:9092");

    // Fixed properties
    set_config(conf, "acks", "all");

    // Install a delivery-error callback.
    rd_kafka_conf_set_dr_msg_cb(conf, dr_msg_cb);

    // Create the Producer instance.
    producer = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (!producer) {
        std::cout << "Failed to create new producer: " << errstr << std::endl;
        LOG_ERROR("Failed to create new producer: errstr{}", errstr);
        return 1;
    }

    conf = NULL;

    // Produce data by selecting random values from these lists.
    int message_count = 1;

    const char* topic = "prodmozgromb";
    const char* user_ids[1] = { "commandpotom" };
    const char* products[1] = { prodmotor };
    std::cout << "prodmotor after = " << prodmotor << std::endl;
    LOG_TRACE("prodmotor after =  prodmotor{}", prodmotor);

    for (int i = 0; i < message_count; i++) {
        const char* key = user_ids[rand() % ARR_SIZE(user_ids)];
        const char* value = products[rand() % ARR_SIZE(products)];
        size_t key_len = strlen(key);
        size_t value_len = strlen(value);

        rd_kafka_resp_err_t err;

        err = rd_kafka_producev(producer,
            RD_KAFKA_V_TOPIC(topic),
            RD_KAFKA_V_MSGFLAGS(RD_KAFKA_MSG_F_COPY),
            RD_KAFKA_V_KEY((void*)key, key_len),
            RD_KAFKA_V_VALUE((void*)value, value_len),
            RD_KAFKA_V_OPAQUE(NULL),
            RD_KAFKA_V_END);

        if (err) {
            std::cout <<  "Failed to produce to topic" << topic << " : " << rd_kafka_err2str(err) << std::endl;
            LOG_ERROR("Failed to produce to topic topic{} : rd_kafka_err2str(err){}", topic, rd_kafka_err2str(err));
            return 1;
        }
        else {
            std::cout << "Produced event to topic " << topic << " : key = " << key << " value = " << value << std::endl;
            LOG_TRACE("Produced event to topic topic{} : key = key{} value = value{}", topic, key, value);
        }

        rd_kafka_poll(producer, 0);
    }

    // Block until the messages are all sent.
    std::cout << "Flushing final messages.." << std::endl;
    LOG_TRACE("Flushing final messages..");
    rd_kafka_flush(producer, 10 * 1000);

    if (rd_kafka_outq_len(producer) > 0) {
        std::cout << rd_kafka_outq_len(producer) << " message(s) were not delivered" << std::endl;
        LOG_INFO("rd_kafka_outq_len(producer){} message(s) were not delivered", rd_kafka_outq_len(producer));
    }

    std::cout << message_count << " events were produced to topic " << topic << std::endl;
    LOG_INFO("message_count{} events were produced to topic topic{}", message_count, topic);

    rd_kafka_destroy(producer);

    return 0;
}
