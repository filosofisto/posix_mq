#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <mqueue.h>
#include <errno.h>
#include <cstring>
#include <sstream>
#ifdef sun
#include <fcntl.h>
#endif

using namespace std;

void usage(char **argv);
int execute(char *msgname, int msgsize, int maxmsg, int interval);

int main(int argc, char **argv) {
    cout << "Posix Message Generator" << endl;

    char msgname[32];
    int opt, msgsize, maxmsg, interval;

    // Parameters
    while ((opt = getopt(argc, argv, "n:s:m:i:")) && opt != -1) {
        switch (opt) {
            case 'n':
                strncpy(msgname, optarg, strlen(optarg));
                break;
            case 's':
                msgsize = atoi(optarg);
                break;
            case 'm':
                maxmsg = atoi(optarg);
                break;
            case 'i':
                interval = atoi(optarg);
                break;
            default:
                break;
        }
    }

    if (optind > argc) {
        usage(argv);
        return EXIT_FAILURE;
    }

    int status = execute(msgname, msgsize, maxmsg, interval);
}

void usage(char **argv)
{
    cout << "Uso: " << argv[0] << " -n msgname -s msgsize -m maxmsg -i interval (seconds)" << endl;
}

int execute(char *msgname, int msgsize, int maxmsg, int interval)
{
    mqd_t queue;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_msgsize = msgsize;
    attr.mq_maxmsg = maxmsg;
    int flags = O_CREAT|O_WRONLY;
    int perms = S_IRUSR|S_IWUSR;

    queue = mq_open(msgname, flags, perms, NULL);

#ifdef sun
    if (queue == (mqd_t) -1) {
#endif
#ifndef sun
    if (queue == static_cast<mqd_t>(-1)) {
#endif
        cerr << "Error on open queue " << msgname << ": "
             << strerror(errno)
             << endl;
        return EXIT_FAILURE;
    }

    cout << "Message opened success" << endl;

    stringstream message;
    int size;

    for (int i = 0; i < maxmsg; i++) {
        message << "Message [" << i+1 << "]";

        size = message.str().size();

        if (mq_send(queue, message.str().c_str(), size, 0) == -1) {
            cerr << "Error on send data to queue: "
                 << strerror(errno)
                 << endl;
            return EXIT_FAILURE;
        }

        cout << message.str() << " inserted successful" << endl;

        message.str(string());

        sleep(interval);
    }

    if (mq_close(queue) == -1) {
        cerr << "Error on close queue: "
             << strerror(errno)
             << endl;
        return EXIT_FAILURE;
    }

    cout << "Message close success" << endl;
}