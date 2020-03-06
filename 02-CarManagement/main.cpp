#include <fstream>
#include <iostream>
#include <sstream>

class Rec {
public:
  std::string type;
  short year;
  short month;
  short date;
  std::string place;
  std::string result;
  Rec *next;

  Rec(std::string line, Rec *next) : next(next) {
    std::istringstream lineStream(line);

    std::getline(lineStream, type, ',');
    std::getline(lineStream, dateS, ',');
    std::getline(lineStream, place, ',');
    std::getline(lineStream, result, ',');
    // 0123456789
    // 2019-12-04
    year = std::stoi(dateS.substr(0, 4));
    month = std::stoi(dateS.substr(5, 2));
    date = std::stoi(dateS.substr(8, 2));
  }
  void print() {
    std::cout << "    " << type << ' ' << place << ' ' << result << std::endl;
  }
  std::string toString() {
    return "REC:" + type + ',' + dateS + ',' + place + ',' + result + '\n';
  }

private:
  std::string dateS;
};

class RecList {
public:
  Rec *head;
  void add(std::string line) { head = new Rec(line, head); }
  void print() {
    Rec *r = head;
    while (r) {
      r->print();
      r = r->next;
    }
  }
  std::string toString() {
    std::string s;
    Rec *r = head;
    while (r) {
      s += r->toString();
      r = r->next;
    }
    return s;
  }
};

class Car {
public:
  std::string id;
  std::string type;
  std::string buyDate;
  std::string price;
  std::string driver;
  Car *next;
  RecList *recs;

  Car(std::string line, Car *next) : next(next) {
    recs = new RecList();
    std::istringstream lineStream(line);
    std::getline(lineStream, id, ',');
    std::getline(lineStream, type, ',');
    std::getline(lineStream, buyDate, ',');
    std::getline(lineStream, price, ',');
    std::getline(lineStream, driver, ',');
  };
  void print() {
    std::cout << id << ' ' << type << ' ' << buyDate << ' ' << price << ' '
              << driver << std::endl;
    recs->print();
    std::cout << "----------------------------------------------\n";
  }
  std::string toString() {
    std::string s;
    s += "CAR:" + id + ',' + type + ',' + buyDate + ',' + price + ',' + driver +
         '\n';
    s += recs->toString();
    return s;
  }
};

class CarList {
public:
  Car *head;

  CarList() : head(nullptr) {}
  void print() {
    Car *t = head;
    while (t) {
      t->print();
      t = t->next;
    }
  }
  void print(std::string id) {
    Car *t = head;
    while (t) {
      if (t->id == id)
        t->print();
      t = t->next;
    }
  }
  void add(std::string line) { head = new Car(line, head); }
  std::string toString() {
    Car *t = head;
    std::string s;
    while (t) {
      s += t->toString();
      t = t->next;
    }
    return s;
  }
};

bool save(CarList *carList) {
  std::cout << carList->toString();
  return true;
}

int main(int argc, char const *argv[]) {
  std::ifstream csvDb;
  std::string line;

  csvDb.open("/home/ac/DSA/02-CarManagement/carInfo.txt");

  CarList *carList = new CarList();
  while ((std::getline(csvDb, line))) {
    if (line.substr(0, 4) == "CAR:")
      carList->add(line.substr(4));
    else
      carList->head->recs->add(line.substr(4));
  }

  bool quit = false;

  while (!quit) {
    std::cout << "> ";
    std::getline(std::cin, line);
    if (line == "list")
      carList->print();
    else if (line == "quit")
      quit = save(carList);
    else if (line.rfind("addcar ", 0) == 0)
      carList->add(line.substr(7));
    else if (line.rfind("show ", 0) == 0)
      carList->print(line.substr(5));
  }
  return 0;
}
