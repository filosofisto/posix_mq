#include <iostream>
#include <cstdlib>
#include <mqueue.h>
#include <errno.h>
#include <cstring>

#ifdef sun
#include <fcntl.h>
#endif

using namespace std;

int main(int argc, char **argv) {
    cout << "Message Posix Receive Sample" << endl;

    mqd_t queue;
    struct mq_attr attr;
    ssize_t bytes;
    char *buffer;

    queue = mq_open("/mq_test", O_RDONLY, S_IRUSR|S_IWUSR);
    if (queue == static_cast<mqd_t>(-1)) {
        cerr << "Error on open queue mq_test: "
             << strerror(errno)
             << endl;
        return EXIT_FAILURE;
    }

    if (mq_getattr(queue, &attr) == -1) {
        cerr << "Error on get attributes on mq_test: "
             << strerror(errno)
             << endl;
        return EXIT_FAILURE;
    }

    cout << "Message opened success" << endl;
    cout << "Attributes collected" << endl;
    cout << "\tMaximum number of messages on queue: " << attr.mq_maxmsg << endl;
    cout << "\tMaximum message size: " << attr.mq_msgsize << endl;
    cout << "\tActual number of messages on queue: " << attr.mq_curmsgs << endl;

    buffer = (char *) malloc(attr.mq_msgsize);
    if (buffer == NULL) {
        cerr << "Memory exausted: "
             << strerror(errno)
             << endl;
        return EXIT_FAILURE;
    }

    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 2;

    bytes = mq_timedreceive(queue, buffer, attr.mq_msgsize, NULL, &timeout);

    if (bytes == -1) {
        if (errno == ETIMEDOUT)
            cout << "Timeout de leitura ocorreu" << endl;
        else {
            cerr << "Error on receive data to queue: "
                 << strerror(errno)
                 << endl;
            return EXIT_FAILURE;
        }
    }

    if (errno != ETIMEDOUT)
        cout << "Message readed in queue:"
             << buffer
             << endl;

    if (mq_close(queue) == -1) {
        cerr << "Error on close queue: "
             << strerror(errno)
             << endl;
        return EXIT_FAILURE;
    }

    cout << "Message queue closed success" << endl;

    return EXIT_SUCCESS;
}