// factory method pattern
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Pizza{
public:
	virtual void prepare() {
		cout << "Preparing " << name << endl;
		cout << "Tossing dough..." << endl;
		cout << "Adding sauce..." << endl;
		cout << "Adding toppings: " << endl;

		vector<string>::iterator iter;
		for (iter = toppings.begin(); iter != toppings.end(); iter++) {
			cout << "\t" << *iter;
		}
		cout << endl;
	}

	virtual void bake() {
		cout << "Bake for 25 minutes at 350" << endl;
	}

	virtual void cut() {
		cout << "Cutting the pizza into diagonal slices" << endl;
	}

	virtual void box() {
		cout << "Place pizza in official PizzaStore box" << endl;
	}

	virtual string getName() {
		return name;
	}

protected:
	string name;
	string dough;
	string sauce;
	vector<string> toppings;
};

class PizzaStore {
public:
	virtual Pizza* orderPizza(string type) {
		Pizza* pizza = createPizza(type);
		pizza->prepare();
		pizza->bake();
		pizza->cut();
		pizza->box();
		return pizza;
	}

	virtual Pizza* createPizza(string type) = 0;
};

class NYStyleCheesePizza : public Pizza {
public:
	NYStyleCheesePizza() {
		name = "NY Style Sauce and Cheese Pizza";
		dough = "Thin Crust Dough";
		sauce = "Marinara Sauce";
		toppings.push_back("Grated Reggiano Cheese");
	}
};

class ChicagoStyleCheesePizza : public Pizza {
public:
	ChicagoStyleCheesePizza() {
		name = "Chicago Style Deep Dish Cheese Pizza";
		dough = "Extra Thick Crust Dough";
		sauce = "Plum Tomato Sauce";
		toppings.push_back("Shredded Mozzarella Cheese");
	}

	virtual void cut() {
		cout << "Cutting the pizza into square slices" << endl;
	}
};

class NYPizzaStore : public PizzaStore {
public:
	Pizza* createPizza(string item) {
		if (item.compare("cheese") == 0) {
			return new NYStyleCheesePizza();
		}
		/*else if (item.compare("veggie")) {
			return new NYStyleVeggiePizza();
		}
		else if (item.compare("clam")) {
			return new NYStyleClamPizza();
		}
		else if (item.compare("pepperoni")) {
			return new NYStylePepperoniPizza();
		}*/
		else return nullptr;
	}
};

class ChicagoPizzaStore : public PizzaStore{
public:
	Pizza* createPizza(string item) {
		if (item.compare("cheese") == 0) {
			return new ChicagoStyleCheesePizza();
		}
		/*else if (item.compare("veggie")) {
		return new NYStyleVeggiePizza();
		}
		else if (item.compare("clam")) {
		return new NYStyleClamPizza();
		}
		else if (item.compare("pepperoni")) {
		return new NYStylePepperoniPizza();
		}*/
		else return nullptr;
	}
};


class PizzaTestDrive {
public:
	static void main(string argv[]) {
		PizzaStore *nyStore = new NYPizzaStore();
		PizzaStore *chicagoStore = new ChicagoPizzaStore();

		Pizza *pizza = nyStore->orderPizza("cheese");
		cout << "Ethan ordered a " << pizza->getName() << endl;

		cout << endl;

		pizza = chicagoStore->orderPizza("cheese");
		cout << "Joel ordered a " << pizza->getName() << endl;
	}
};

int main()
{
	PizzaTestDrive::main(nullptr);
	return 0;
}
