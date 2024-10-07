//#include <glib.h>
#include <librdkafka/rdkafka.h>
#include <iostream>
#include <cstdlib>
#include "log.hpp"

/* Wrapper to set config values and error out if needed.
 */

static void set_configD(rd_kafka_conf_t* confD, char* keyD, char* valueD) {
    LOG_TRACE("55541");
    char errstrD[512];
    LOG_TRACE("55542");
    rd_kafka_conf_res_t resD;
    LOG_TRACE("55543");
    resD = rd_kafka_conf_set(confD, keyD, valueD, errstrD, sizeof(errstrD));
    LOG_TRACE("55544");
    if (resD != RD_KAFKA_CONF_OK) {
        LOG_FATAL("Unable to set config: errstr{}", errstrD);
        exit(1);
    }
}