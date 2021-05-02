#include <iostream>
#include <future>
#include <thread>

using namespace std;
using namespace this_thread;
using namespace chrono;

void TEST_01() {
    cout << "##" << __FUNCTION__ << endl;

    promise<int> prom;
    auto future = prom.get_future();

    auto t = std::thread([](std::promise<int>&& prom)
        {

            prom.set_value(10);
        }, std::move(prom));

    cout << future.get() << endl;

    t.join();
}

void TEST_02() {
    cout << "##" << __FUNCTION__ << endl;

    promise<int> prom1;
    auto future1 = prom1.get_future();
    promise<int> prom2;
    auto future2 = prom2.get_future();

    auto t = std::thread([](std::promise<int>&& prom1, std::promise<int>&& prom2)
        {
            prom1.set_value(10);
            sleep_for(seconds(2));
            prom2.set_value(20);
            // sleep_for(seconds(2));
        }, std::move(prom1), std::move(prom2));

    cout << future1.get() << endl;
    cout << future2.get() << endl;

    t.join();
}

void TEST_03() {
    cout << "##" << __FUNCTION__ << endl;

    promise<int> prom;
    auto future = prom.get_future();
    int a = 10;

    auto t = std::thread([](std::promise<int>&& prom, int value)
        {
            value = 100;
            prom.set_value(value);
        }, std::move(prom), a);

    cout << future.get() << endl;
    cout << "a = " << a << endl;

    t.join();
}


void TEST_04() {
    cout << "##" << __FUNCTION__ << endl;

    promise<int> prom;
    auto future = prom.get_future();
    int a = 10;

    auto t = std::thread([](std::promise<int>&& prom, int* value)
        {
            *value = 20;
            prom.set_value(*value);
        }, std::move(prom), &a);

    cout << future.get() << endl;
    cout << "a = " << a << endl;

    t.join();
}

void TEST_05() {
    cout << "##" << __FUNCTION__ << endl;

    promise<int> prom;
    auto future = prom.get_future();
    int a = 10;

    /*
    auto t = std::thread([a](std::promise<int>&& prom) // a
        {
            prom.set_value(a);
            a = 20; // error: assignment of read-only variable ¡®a¡¯
        }, std::move(prom));
    */

    auto t = std::thread([&a](std::promise<int>&& prom) // &a
        {
            prom.set_value(a);
            a = 100;
        }, std::move(prom));

    cout << future.get() << endl;
    cout << "a = " << a << endl;

    t.join();
}

void TEST_10() {
    cout << "##" << __FUNCTION__ << endl;

    promise<int> prom1;
    auto future1 = prom1.get_future();
    promise<int> prom2;
    auto future2 = prom2.get_future();
    int a = 10;
    int b = 20;

    auto t = std::thread([&a, &b](std::promise<int>&& prom1, std::promise<int>&& prom2)
        {
            prom1.set_value(a);
            sleep_for(seconds(2));
            a = 100;
            prom2.set_value(b);
            sleep_for(seconds(2));

        }, std::move(prom1), std::move(prom2));

    cout << future1.get() << endl;

    while (true) {
        future_status status = future2.wait_for(chrono::seconds(1));

        if (status == future_status::timeout) {
            cerr << ">";
        }
        else if (status == future_status::ready) {
            cerr << endl;
            break;
        }
    }

    cout << future2.get() << endl;
    cout << a << endl;

    t.join();

}

int main()
{
    //TEST_01();
    //TEST_02();
    TEST_03();
    TEST_04();
    TEST_05();
    //TEST_10();
    return 0;
}

