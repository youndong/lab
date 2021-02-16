#include <iostream>
#include <vector>

using namespace std;

class WeatherData;
class DisplayElement;

/* interface */
class Observer {
public:
	virtual void update(float temp, float humidity, float pressure) = 0;
};

/* interface */
class Subject {
public:
	virtual void registerObserver(Observer *o) = 0;
	virtual void removeObserver(Observer *o) = 0;
	virtual void notifyObservers() = 0;
};

/* interface */
class DisplayElement {
public:
	virtual void display() = 0;
};

class CurrentConditionsDisplay : public Observer, public DisplayElement {
public:
	CurrentConditionsDisplay(Subject *weatherData) {
		this->weatherData = weatherData;
		weatherData->registerObserver(this);
	}

	~CurrentConditionsDisplay() {
		weatherData->removeObserver(this);
	}

	void update(float temperature, float humidity, float pressure) {
		this->temperature = temperature;
		this->humidity = humidity;
		display();
	}

	void display() {
		cout << "Current conditions: " << temperature << "F degrees and " << humidity << "% humidity" << endl;
	}
private:
	float temperature;
	float humidity;
	Subject	*weatherData;
};

class StatisticsDisplay : public Observer, public DisplayElement {
public:
	StatisticsDisplay(Subject *weatherData) {
		this->weatherData = weatherData;
		weatherData->registerObserver(this);
	}

	~StatisticsDisplay() {
		weatherData->removeObserver(this);
	}
	void update(float temperature, float humidity, float pressure) {
		this->temperature = temperature;
		this->humidity = humidity;
		display();
	}

	void display() {
		cout << "Statistics: " << temperature << "F degrees and " << humidity << "% humidity" << endl;
	}
private:
	float temperature;
	float humidity;
	Subject	*weatherData;
};

class ForecastDisplay : public Observer, public DisplayElement {
public:
	ForecastDisplay(Subject *weatherData) {
		this->weatherData = weatherData;
		weatherData->registerObserver(this);
	}
	~ForecastDisplay() {
		weatherData->removeObserver(this);
	}
	void update(float temperature, float humidity, float pressure) {
		this->temperature = temperature;
		this->humidity = humidity;
		display();
	}

	void display() {
		cout << "Forecast: " << temperature << "F degrees and " << humidity << "% humidity" << endl;
	}
private:
	float temperature;
	float humidity;
	Subject	*weatherData;
};

class WeatherData : public Subject {
public:
	void setMeasurements(float temperature, float humidity, float pressure) {
		this->temperature = temperature;
		this->humidity = humidity;
		this->pressure = pressure;

		measurementsChanged();
	}
	void measurementsChanged() {
		notifyObservers();
	}
	virtual void registerObserver(Observer *o) {
		this->observers.push_back(o);
	}
	virtual void removeObserver(Observer *o) {
		vector<Observer*>::iterator iter;
		for (iter = this->observers.begin(); iter != this->observers.end(); ) {
			if (o == *iter) {
				iter = observers.erase(iter);
			}
			else {
				//++iter;
				iter++;
			}
		}
	}
	virtual void notifyObservers() {
		vector<Observer*>::iterator iter;
		for (iter = this->observers.begin(); iter != this->observers.end(); iter++) {
			auto observer = *iter;
			observer->update(temperature, humidity, pressure);
		}
	}


private:
	vector<Observer*>	observers;
	float			temperature;
	float			humidity;
	float			pressure;
};

class WeatherStation {
public:
	static void main(string args[]) {
		WeatherData* weatherData = new WeatherData();

		CurrentConditionsDisplay	*currentDisplay = new CurrentConditionsDisplay(weatherData);
		StatisticsDisplay		*statisticsDisplay = new StatisticsDisplay(weatherData);
		ForecastDisplay			*forecastDisplay = new ForecastDisplay(weatherData);

		weatherData->setMeasurements(80, 65, 30.4f);
		weatherData->setMeasurements(82, 70, 29.2f);
		weatherData->setMeasurements(78, 90, 29.2f);

		delete currentDisplay;
		delete forecastDisplay;

		weatherData->setMeasurements(81, 75, 30.1f);
		weatherData->setMeasurements(83, 65, 29.8f);

		delete statisticsDisplay;
	}
};

int main()
{
	WeatherStation::main(nullptr);
	return 0;
}
