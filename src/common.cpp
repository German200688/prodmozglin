//#include <glib.h>
#include <librdkafka/rdkafka.h>
#include <iostream>
#include <cstdlib>
#include "log.hpp"

/* Wrapper to set config values and error out if needed.
 */
static void set_config(rd_kafka_conf_t* conf, char* key, char* value) {
    LOG_TRACE("55541");
    char errstr[512];
    LOG_TRACE("55542");
    rd_kafka_conf_res_t res;
    LOG_TRACE("55543");
    res = rd_kafka_conf_set(conf, key, value, errstr, sizeof(errstr));
    LOG_TRACE("55544");
    if (res != RD_KAFKA_CONF_OK) {
        LOG_FATAL("Unable to set config: errstr{}", errstr);
        exit(1);
    }
}