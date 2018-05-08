#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <Windows.h>

using namespace std;

class Singleton {
private:
	static Singleton*	uniqueInstance;
	static int	count;

	int id;

	Singleton(int id) {
		this->id = id;
	}
public:

	static Singleton& getInstance() {
		if (uniqueInstance == nullptr) {
			
			std::mutex mtx_lock;
			std::lock_guard<std::mutex> guard(mtx_lock);
			
			if (uniqueInstance == nullptr) {
				uniqueInstance = new Singleton(count);
				count++;
			}
		}

		return *uniqueInstance;
	}

	int getId() {
		return this->id;
	}
	static int getCount() {
		return count;
	}
};

Singleton* Singleton::uniqueInstance = nullptr;
int Singleton::count = 0;


class SingletonTest {
public:
	static void main(string argv[]) {
		
		std::mutex mtx_lock;

		std::thread t1, t2;
		t1 = std::thread([&]()
		{
			for (int i = 0; i < 10; ++i)
			{
				std::lock_guard<std::mutex> guard(mtx_lock);
				std::cout << "t1 : count=" << Singleton::getCount() << ", id=" << Singleton::getInstance().getId() << endl;
			}
		});

		t2 = std::thread([&]()
		{
			for (int i = 0; i < 10; ++i)
			{
				std::lock_guard<std::mutex> guard(mtx_lock);
				std::cout << "t2 : count=" << Singleton::getCount() << ", id=" << Singleton::getInstance().getId() << endl;
			}
		});

		t1.join();
		t2.join();
	}
};

int main()
{
	SingletonTest::main(nullptr);
	return 0;
}
