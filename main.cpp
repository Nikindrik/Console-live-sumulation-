#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

class Animal {
public:
    int x, y, age;
    char col;

    Animal(int x, int y, int age, char col) : x(x), y(y), age(age), col(col) {}

    virtual void move() = 0;
    virtual void eat(vector<Animal*>& animals) = 0;
    virtual void ageOneYear() = 0;

    virtual ~Animal() {}
};

class Herbivore : public Animal {
public:
    Herbivore(int x, int y) : Animal(x, y, 0, 'H') {}

    void move() override {
        x += rand() % 3 - 1;  // Random перемещение [-1, 1]
        y += rand() % 3 - 1;
    }

    void eat(vector<Animal*>& animals) override {
        // Herbivores не едят животных
    }

    void ageOneYear() override {
        age++;
    }
};

class Carnivore : public Animal {
public:
    Carnivore(int x, int y) : Animal(x, y, 0, 'C') {}

    void move() override {
        x += rand() % 3 - 1;  // Random перемещение [-1, 1]
        y += rand() % 3 - 1;
    }

    void eat(vector<Animal*>& animals) override {
        for (auto& animal : animals) {
            if (animal->col == 'H' && abs(x - animal->x) <= 1 && abs(y - animal->y) <= 1) {
                // Carnivore кущаем herbivore
                delete animal;
                animal = nullptr;
                break;  // Кушаем только одного животного за ход
            }
        }
        animals.erase(remove(animals.begin(), animals.end(), nullptr), animals.end());
    }

    void ageOneYear() override {
        age++;
    }
};

class Environment {
public:
    vector<Animal*> animals;
    int grassCount;
    double grassRegrowthRate;
    double predationProbability;

    Environment(int grassCount, double grassRegrowthRate, double predationProbability)
        : grassCount(grassCount), grassRegrowthRate(grassRegrowthRate), predationProbability(predationProbability) {
        srand(time(nullptr));
    }

    void initializeHerbivores(int count) {
        for (int i = 0; i < count; ++i) {
            animals.push_back(new Herbivore(rand() % 100, rand() % 50));
        }
    }

    void initializeCarnivores(int count) {
        for (int i = 0; i < count; ++i) {
            animals.push_back(new Carnivore(rand() % 100, rand() % 50));
        }
    }

    void simulateOneYear() {
        for (auto& animal : animals) {
            animal->move();
            animal->ageOneYear();
            animal->eat(animals);
        }

        // Трава растет
        grassCount = static_cast<int>(grassCount * (1.0 + grassRegrowthRate));

        // Случайные события хищничества
        for (auto& animal : animals) {
            if (animal->col == 'H' && (rand() % 100) < (predationProbability * 100)) {
                delete animal;
                animal = nullptr;
            }
        }
        animals.erase(remove(animals.begin(), animals.end(), nullptr), animals.end());
    }

    ~Environment() {
        for (auto& animal : animals) {
            delete animal;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Rus");
    Environment environment(100, 0.1, 0.05);
    environment.initializeHerbivores(50);
    environment.initializeCarnivores(20);

    int n;
    cout << "Сколько лет будет симуляция:" << endl;
    cin >> n;

    for (int year = 0; year < n; ++year) {
        cout << "Год: " << year + 1 << ":\n";

        for (const auto& animal : environment.animals) {
            cout << "Животное в (" << animal->x << ", " << animal->y << ") - Возраст: " << animal->age << " - Тип: " << animal->col << "\n";
        }

        cout << "\n";

        environment.simulateOneYear();
    }

    return 0;
}
