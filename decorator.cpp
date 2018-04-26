#include <iostream>
#include <string>

using namespace std;

class Beverage {
public:
	virtual string getDescription() {
		return description;
	}
	virtual double cost() = 0;
protected:
	string description = "no title";
};

class CondimentDecorator : public Beverage {
public:
	virtual string getDescription() = 0;
};

class Espresso : public Beverage {
public:
	Espresso() {
		description = "Espresso";
	}
	virtual double cost() {
		return 1.99;
	}
};

class HouseBlend : public Beverage {
public:
	HouseBlend() {
		description = "HouseBlend";
	}
	virtual double cost() {
		return .89;
	}
};

class DarkRoast : public Beverage {
public:
	DarkRoast() {
		description = "DarkRoast";
	}
	virtual double cost() {
		return 1.39;
	}
};


class Mocha : public CondimentDecorator {
public:
	Mocha(Beverage* beverage) {
		this->beverage = beverage;
	}

	virtual ~Mocha() {
		delete beverage;
	}

	virtual string getDescription() {
		return beverage->getDescription() + ", Mocha";
	}
	virtual double cost() {
		return beverage->cost() + .20;
	}
private:
	Beverage * beverage;
};

class Whip : public CondimentDecorator {
public:
	Whip(Beverage* beverage) {
		this->beverage = beverage;
	}

	virtual ~Whip() {
		delete beverage;
	}

	virtual string getDescription() {
		return beverage->getDescription() + ", Whip";
	}
	virtual double cost() {
		return beverage->cost() + .30;
	}
private:
	Beverage * beverage;
};

class StarbuzzCoffee {
public:
	static void main(string args[]) {
		Beverage* beverage1 = new Espresso();
		cout << beverage1->getDescription() << " $" << beverage1->cost() << endl;

		Beverage* beverage2 = new DarkRoast();
		beverage2 = new Mocha(beverage2);
		beverage2 = new Mocha(beverage2);
		beverage2 = new Whip(beverage2);
		cout << beverage2->getDescription() << " $" << beverage2->cost() << endl;

		Beverage* beverage3 = new HouseBlend();
		beverage3 = new Mocha(beverage3);
		beverage3 = new Whip(beverage3);
		cout << beverage3->getDescription() << " $" << beverage3->cost() << endl;

		delete beverage1;
		delete beverage2;
		delete beverage3;
	}
};

int main()
{
	StarbuzzCoffee::main(nullptr);
	return 0;
}
