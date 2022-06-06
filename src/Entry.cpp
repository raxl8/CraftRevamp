#include "pch.h"

#include <uvw.hpp>

static void thread_cb(std::shared_ptr<void> arg) {
    printf("Hello World!\n");
}


int main() {
    auto loop = uvw::Loop::getDefault();
    auto thread = loop->resource<uvw::Thread>(thread_cb);
    thread->run();
    thread->join();

    return 0;
}
