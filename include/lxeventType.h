#ifndef __LX_EVENT_H__
#define __LX_EVENT_H__

typedef int (*event_reply)(int sockfd, unsigned int eventid, const char * data);
typedef int (*event_release)(int sockfd, void * data);

typedef enum event_type_e {
    EVENT_TYPE_READ,
    EVENT_TYPE_WRITE,
    EVENT_TYPE_ACCEPTS,
    EVENT_TYPE_WAIT_REPLY
}event_type_e;

typedef struct lxevent_env {

    int sockfd;
    int eventid;

    event_type_e type;

    event_reply replyCb;
    event_release releaseCb;

}lxevent_env;

typedef struct lxevent_baseenv lxevent_baseenv;

typedef struct lxeventMultiplexer {



}lxeventMultiplexer;

struct lxevent_baseenv {
    lxevent_env * head;

    int sockfd;
    unsigned int port;

    struct lxeventMultiplexer * multiplexer;
    char meth[32];

};

#endif