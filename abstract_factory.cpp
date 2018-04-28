#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Dough {
public:
	Dough() {
		cout << "+ Dough" << endl;
	}
	virtual ~Dough() {}
};

class ThinCrustDough : public Dough {
public:
	ThinCrustDough() {
		cout << "+ ThinCrustDough" << endl;
	}
	virtual ~ThinCrustDough() {}
};

class Sauce {
public:
	Sauce() {
		cout << "+ Sauce" << endl;
	}
	virtual ~Sauce() {}
};

class MarinaraSauce : public Sauce {
public:
	MarinaraSauce() {
		cout << "+ MarinaraSauce" << endl;
	}
	virtual ~MarinaraSauce() {}
};
class Veggies {
};
class Garlic : public Veggies {
public:
	Garlic() {
		cout << "+ Garlic" << endl;
	}
	virtual ~Garlic() {}
};
class Onion : public Veggies {
public:
	Onion() {
		cout << "+ Onion" << endl;
	}
	virtual ~Onion() {}
};
class Mushroom : public Veggies {
public:
	Mushroom() {
		cout << "+ Mushroom" << endl;
	}
	virtual ~Mushroom() {}
};
class RedPepper : public Veggies {
public:
	RedPepper() {
		cout << "+ RedPepper" << endl;
	}
	virtual ~RedPepper() {}
};

class Cheese {
public:
	Cheese() {
		cout << "+ Cheese" << endl;
	}
	virtual ~Cheese() {}
};

class ReggianoCheese : public Cheese {
public:
	ReggianoCheese() {
		cout << "+ ReggianoCheese" << endl;
	}
	virtual ~ReggianoCheese() {}
};

class Pepperoni {
public:
	Pepperoni() {
		cout << "+ Pepperoni" << endl;
	}
	virtual ~Pepperoni() {}
};

class SlicedPepperoni : public Pepperoni {
public:
	SlicedPepperoni() {
		cout << "+ SlicedPepperoni" << endl;
	}
	virtual ~SlicedPepperoni() {}
};
class Clams {
public:
	Clams() {
		cout << "+ Clams" << endl;
	}
	virtual ~Clams() {}
};
class FreshClams : public Clams {
public:
	FreshClams() {
		cout << "+ FreshClams" << endl;
	}
	virtual ~FreshClams() {}
};

class Pizza {
public:
	virtual void prepare() = 0;

	virtual void bake() {
		cout << "Bake for 25 minutes at 350" << endl;
	}

	virtual void cut() {
		cout << "Cutting the pizza into diagonal slices" << endl;
	}

	virtual void box() {
		cout << "Place pizza in official PizzaStore box" << endl;
	}

	virtual void setName(string name) {
		this->name = name;
	}
	virtual string getName() {
		return name;
	}

	virtual string toString() {
		cout << name << endl;
		return name;
	}

protected:
	string name;
	Dough* dough;
	Sauce* sauce;
	vector<Veggies*> veggies;
	Cheese* cheese;
	Pepperoni* pepperoni;
	Clams* clam;
};


class PizzaIngredientFactory {
public:
	virtual Dough* createDough() = 0;
	virtual Sauce* createSauce() = 0;
	virtual Cheese *createCheese() = 0;
	virtual vector<Veggies*> createVeggies() = 0;
	virtual Pepperoni* createPepperoni() = 0;
	virtual Clams* createClam() = 0;

};

class NYPizzaIngredientFactory : public PizzaIngredientFactory {
public:
	virtual Dough* createDough() {
		return new ThinCrustDough();
	}

	virtual Sauce* createSauce() {
		return new MarinaraSauce();
	}

	virtual Cheese* createCheese() {
		return new ReggianoCheese();
	}

	virtual vector<Veggies*> createVeggies() {
		vector<Veggies*> veggiese = { new Garlic(), new Onion(), new Mushroom{}, new RedPepper() };
		return veggiese;
	}

	virtual Pepperoni* createPepperoni() {
		return new SlicedPepperoni();
	}

	virtual Clams* createClam() {
		return new FreshClams();
	}
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

class CheesePizza : public Pizza {
public:
	CheesePizza(PizzaIngredientFactory* ingredientFactory) {
		this->ingredientFactory = ingredientFactory;
	}

	void prepare() {
		cout << "Preparing " << name << endl;
		dough = ingredientFactory->createDough();
		sauce = ingredientFactory->createSauce();
		cheese = ingredientFactory->createCheese();
	}

private:
	PizzaIngredientFactory * ingredientFactory;
};

class ClamPizza : public Pizza {
public:
	ClamPizza(PizzaIngredientFactory* ingredientFactory) {
		this->ingredientFactory = ingredientFactory;
	}

	void prepare() {
		cout << "Preparing " << name << endl;
		dough = ingredientFactory->createDough();
		sauce = ingredientFactory->createSauce();
		cheese = ingredientFactory->createCheese();
		clam = ingredientFactory->createClam();
	}

private:
	PizzaIngredientFactory * ingredientFactory;
};

class NYPizzaStore : public PizzaStore {
public:
	Pizza * createPizza(string item) {
		Pizza *pizza = nullptr;
		NYPizzaIngredientFactory *ingredientFactory = new NYPizzaIngredientFactory();

		if (item.compare("cheese") == 0) {
			pizza = new CheesePizza(ingredientFactory);
			pizza->setName("New York Style Cheese Pizza");
		}
		
		return pizza;
	}
};

class PizzaTestDrive {
public:
	static void main(string argv[]) {
		PizzaStore *nyStore = new NYPizzaStore();

		Pizza *pizza = nyStore->orderPizza("cheese");
		cout << "Ethan ordered a " << pizza->getName() << endl;
	}
};

int main()
{
	PizzaTestDrive::main(nullptr);
	return 0;
}
