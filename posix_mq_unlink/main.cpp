#include <iostream>
#include <cstring>
#include <cstdlib>
#include <mqueue.h>

using namespace std;

void usage(char **argv);

int main(int argc, char **argv) {
    cout << "Posix Message unlink" << endl;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        usage(argv);
        return EXIT_FAILURE;
    }

    if (mq_unlink(argv[1]) == -1) {
        cerr << "Error to unlink message " << argv[1] << endl;
        return EXIT_FAILURE;
    }

    cout << "unlinked of " << argv[1] << " successful" << endl;
    return EXIT_SUCCESS;
}

void usage(char **argv)
{
    cout << "Usage: " << argv[0] << " <msgname>" << endl;
}