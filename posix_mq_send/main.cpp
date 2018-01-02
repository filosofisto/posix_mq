#include <iostream>
#include <cstdlib>
#include <mqueue.h>
#include <errno.h>
#include <cstring>
#include <sstream>
#ifdef sun
#include <fcntl.h>
#endif

using namespace std;

int main(int argc, char **argv) {
    cout << "Message Posix Sample" << endl;

    /*if (argc < 2) {
        cerr << "Usage: " << endl
             << "\t" << argv[0] << " <data>" << endl
             << "\tWhere <data> is text for send to queue" << endl;
        return EXIT_FAILURE;
    }*/

    mqd_t queue;

    queue = mq_open("/mq_test", O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR, NULL);
    if (queue == static_cast<mqd_t>(-1)) {
        cerr << "Error on open queue mq_test: "
             << strerror(errno)
             << endl;
        return EXIT_FAILURE;
    }

    cout << "Message opened success" << endl;

    int messageId = 0;
    stringstream ss;

    while (messageId < 20) {
        ss << "Message " << messageId;

        if (mq_send(queue, ss.str().c_str(), ss.str().size(), 0) == -1) {
            cerr << "Error on send data to queue: "
                 << strerror(errno)
                 << endl;
            return EXIT_FAILURE;
        }

        ss.str(string());

        cout << "Message inserted in queue" << endl;

        messageId++;
    }

    if (mq_close(queue) == -1) {
        cerr << "Error on close queue: "
             << strerror(errno)
             << endl;
        return EXIT_FAILURE;
    }

    cout << "Message close success" << endl;

    return EXIT_SUCCESS;
}