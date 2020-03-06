#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

void help() {
  std::cout << "车辆管理帮助\n"
            << "================================================\n"
            << "数据文件：当前目录下 `carInfo.txt`\n"
            << "list:               列出所有车辆及其违章记录\n"
            << "quit:               保存并退出\n"
            << "add car <id>,<type>,<YYYY-MM-DD>,<price>,<driver>\n"
            << "                    添加车辆\n"
            << "add rec <car id>,<rec id>,<YYYY-MM-DD>,<place>,<result>\n"
            << "                    添加违章记录\n"
            << "del car <id>:       车辆报废\n"
            << "show car <id>:      车辆信息及违章记录\n"
            << "show place <place>: 按照违章地点列出违章记录\n"
            << "show type <type>:   按照违章类型列出违章记录\n";
}

class Rec {
public:
  std::string id;
  std::string type;
  short year;
  short month;
  short date;
  std::string place;
  std::string result;
  Rec *next;

  Rec(std::string line, Rec *next) : next(next) {
    std::istringstream lineStream(line);

    std::getline(lineStream, id, ',');
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
    std::cout << "    " << id << ' ' << type << ' ' << place << ' ' << result
              << std::endl;
  }
  std::string toString() {
    return "REC:" + id + ',' + type + ',' + dateS + ',' + place + ',' + result +
           '\n';
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
    short count = 0;
    while (r) {
      count++;
      r->print();
      r = r->next;
    }
    std::cout << "共 " << count << " 条违章记录\n";
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
  void print(std::string id) { getCarById(id)->print(); }
  std::string toString() {
    Car *t = head;
    std::string s;
    while (t) {
      s += t->toString();
      t = t->next;
    }
    return s;
  }
  Car *getCarById(std::string id) {
    Car *car = head;
    while (car) {
      if (car->id == id)
        return car;
      car = car->next;
    }
    throw std::out_of_range("Car not found");
  }
  void add(std::string line) { head = new Car(line, head); }
  void del(std::string id) {
    Car *prev = nullptr;
    Car *car = head;

    while (car) {
      if (car->id == id) {
        if (prev != nullptr)
          prev->next = car->next;
        else
          head = car->next;
        std::cout << "Deleted:\n";
        car->print();
        return;
      }
      prev = car;
      car = car->next;
    }
    std::cout << "Not Found";
  }
  void showRecBy(std::string key, std::string value) {
    Car *c = head;
    short count = 0;
    while (c) {
      Rec *r = c->recs->head;
      while (r) {
        bool equal = false;
        if (key == "place")
          equal = r->place == value;
        else if (key == "type")
          equal = r->type == value;
        if (equal) {
          std::cout << c->id << "号车：";
          r->print();
          count++;
        }
        r = r->next;
      }
      c = c->next;
    }
    std::cout << "共 " << count << " 条违章记录\n";
  }
  void addRec(std::string line) {
    std::istringstream lineStream(line);
    std::string carId;

    std::getline(lineStream, carId, ',');
    std::getline(lineStream, line);
    getCarById(carId)->recs->add(line);
  }
};

bool save(CarList *carList) {
  std::ofstream f;
  f.open("carInfo.txt");
  if (!f.is_open()) {
    std::cout << "Write Failure";
    return false;
  }
  f << carList->toString();
  return true;
}

int main(int argc, char const *argv[]) {
  std::ifstream db;
  std::string line;

  db.open("carInfo.txt");
  if (!db.is_open()) {
    help();
    return -1;
  }

  CarList *carList = new CarList();
  while ((std::getline(db, line))) {
    if (line.substr(0, 4) == "CAR:")
      carList->add(line.substr(4));
    else
      carList->head->recs->add(line.substr(4));
  }

  bool quit = false;

  while (!quit) {
    std::cout << "> ";
    std::getline(std::cin, line);
    try {
      if (line == "list")
        carList->print();
      else if (line == "quit")
        quit = save(carList);
      else if (line == "help")
        help();
      else if (line.rfind("add car ", 0) == 0)
        carList->add(line.substr(8));
      else if (line.rfind("add rec ", 0) == 0)
        carList->addRec(line.substr(8));
      else if (line.rfind("del car ", 0) == 0)
        carList->del(line.substr(8));
      else if (line.rfind("show car ", 0) == 0)
        carList->print(line.substr(9));
      else if (line.rfind("show place ", 0) == 0)
        carList->showRecBy("place", line.substr(11));
      else if (line.rfind("show type ", 0) == 0)
        carList->showRecBy("type", line.substr(10));
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
    }
  }
  return 0;
}
