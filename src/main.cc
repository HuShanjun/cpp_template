#include <iostream>
#include <sys/poll.h>
#include <thread>
#include "threadpool/ThreadPool.h"
using namespace std;

int main()
{
    try
    {
        cout<< "this is test" << endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}