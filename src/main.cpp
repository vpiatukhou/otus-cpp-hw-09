#include "async.h"

int main(int argc, char* argv[]) {
    auto h1 = async::connect(3);

    async::receive(h1, "1-1", 3);
    async::receive(h1, "1-2", 3);
    async::receive(h1, "1-3", 3); //the end of the block: 1-1, 1-2, 1-3

    auto h2 = async::connect(2);

    async::receive(h1, "1-4", 3);
    async::receive(h1, "1-5", 3);

    async::receive(h2, "2-1", 3);
    async::receive(h2, "2-2", 3); //the end of the block: 2-1, 2-2

    auto h3 = async::connect(4);
    async::receive(h1, "1-6", 3); //the end of the block: 1-4, 1-5, 1-6

    async::receive(h3, "3-1", 3);
    async::receive(h3, "3-2", 3);

    async::receive(h1, "1-7", 3);
    async::receive(h1, "1-8", 3); //the end of the block: 1-7, 1-8

    async::disconnect(h1);
    async::disconnect(h2);

    async::receive(h3, "3-3", 3);
    async::receive(h3, "3-4", 3); //the end of the block: 3-1, 3-2, 3-3, 3-4
    async::receive(h3, "3-5", 3); //the end of the block: 3-5

    async::disconnect(h3);

    return 0;
}
