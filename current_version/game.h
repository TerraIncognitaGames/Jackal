#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#endif // GAME_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <functional>
#include <time.h>
#include <sstream>


using std::map;
using std::string;
using std::vector;


const size_t sizeOfIsland = 11; // без воды
const size_t numberOfPirates = 3;
const size_t numberOfPlayers = 4; // Don't change this one

/// If you change enum, don't forget to update functions.
/// TODO: replace: <enum_name> ----->  <enum_name>:char
/// В CodeBlocks глючат подсказки если сделать это сейчас
enum Direction {TOP, BOTTOM, RIGHT, LEFT, TOPRIGHT,
                      TOPLEFT, BOTTOMRIGHT, BOTTOMLEFT, HORSEDIR};
enum SquareType {UNEXPLORED, WATER, FIELD, JUNGLE, DESERT, BOG, MOUNTAINS,
                       TRAP, RUM, ARROW, HORSE, ICE, CROCODILE, BALOON,
                       GUN, CANNIBAL, FORTRESS, ABORIGINE, SHIP };
enum EffectOfSquare { STOP, GOON, ASK, KILL };
enum EventType { DROPGOLD, MOVE, DEATH };

/*
EffectOfSquare effectOfCellType(SquareType type) {
  switch (type){
    case SINGLEARROW:
    case ARROWS:
    case GUN:
    case HOARSE:
    case BALOON:
    case ICE:
    case CROCODILE:
      return true;
    default:
      return false;
  }
}*/


class Point {
public:
  int x, y;
  Point(int x_coord = 0, int y_coord = 0)
    : x(x_coord), y(y_coord) {}


  bool operator== (const Point &other) const {
    return ((other.x == x) && (other.y == y));
  }

  Point GetPointToThe(Direction dir) {
    switch (dir) {
      case TOP:
        return Point(x, y + 1);
      case BOTTOM:
        return Point(x, y - 1);
      case RIGHT:
        return Point(x + 1, y);
      case LEFT:
        return Point(x - 1, y);
      case TOPRIGHT:
        return Point(x + 1, y + 1);
      case TOPLEFT:
        return Point(x - 1, y + 1);
      case BOTTOMLEFT:
        return Point(x - 1, y - 1);
      case BOTTOMRIGHT:
        return Point(x + 1, y - 1);
      case HORSEDIR:
        throw std::runtime_error("HORSEDIR in GetPointToThe()");
        return Point(0, 0);
    }
  }

  Direction GetDirectionByPoint(const Point& second) {
    int dx = second.x - x;
    int dy = second.y - y;
    if (dx == -1) {
      if (dy == -1) {
        return BOTTOMLEFT;
      }
      if (dy == 0) {
        return LEFT;
      }
      if (dy == 1) {
        return TOPLEFT;
      }
    }
    if (dx == 1) {
      if (dy == -1) {
        return BOTTOMRIGHT;
      }
      if (dy == 0) {
        return RIGHT;
      }
      if (dy == 1) {
        return TOPRIGHT;
      }
    }
    if (dx == 0) {
      if (dy == -1) {
        return BOTTOM;
      }
      if (dy == 1) {
        return TOP;
      }
    }

    return HORSEDIR;
  }

  bool IsCorrectPoint(int size) {
    return (x >= 0 && x < size && y >= 0 && y < size &&
            !((x == size||x == 0) && (y == size || y == 0)));
  }

};

class Pirate {
private:
  bool gold_;
  Point coordinate_;
  size_t position_on_square_;
  bool alive_;

public:
  Pirate(Point coord)
      : gold_(0),
        coordinate_(coord),
        position_on_square_(0),
        alive_(true) {
      std::cout << "Pirate created" << std::endl;
    }

  bool gold() const {
    return gold_;
  }
  Point coordinate() const {
    return coordinate_;
  }
  size_t position_on_square() const {
    return position_on_square_;
  }
  bool alive() const {
    return alive_;
  }

  void kill() {
    alive_ = false;
    gold_ = false;
    position_on_square_ = 0;
  }

  ~Pirate() {}

  friend class GameHolder;
};

struct Request {
  EventType type;
  size_t player_id;
  size_t pirate_num;
  Point destination;
  size_t position_on_square;
  Request(EventType type, size_t player_id, size_t pirate_num,
          Point destination, size_t position_on_square)
      : type(type),
        player_id(player_id),
        pirate_num(pirate_num),
        destination(destination),
        position_on_square(position_on_square) { }

  Request() {}
};


class SquareBase {
private:
  SquareType type_;
  bool explored_; /// чтобы сервер мог следить за циклами (?)
public :
  SquareBase()
      : type_(UNEXPLORED),
        explored_(false) { }

  SquareBase(SquareType type, bool explored)
      : type_(type),
        explored_(explored) { }

  virtual EffectOfSquare effectType(size_t player_id, const Pirate& pirate) const {
    return STOP;
  }

  SquareType type() const {
    return type_;
  }

  virtual ~SquareBase() {
    // std::cout << "SqBase destructor" << std::endl;
  }

  virtual string info() const{
    string result;
    result.push_back(char(type()));
    return result;
    // return std::to_string(type()); should be working
  }
};

typedef SquareBase SquareUnexplored;

class SquareStopBase: public SquareBase { // на таких клетках может кончатся ход
public:
  SquareStopBase(SquareType type, bool explored)
    : SquareBase(type, explored) {}

  SquareStopBase()
    : SquareBase(WATER, true) {}

  virtual EffectOfSquare effectType(size_t player_id, const Pirate& pirate) const {
    return STOP;
  }

  virtual ~SquareStopBase() { };
};

typedef SquareStopBase SquareWater;

class SquareField: public SquareStopBase { /// Это также база для клеток на которые может упасть золото.
friend class GameMap;
public:
  SquareField(SquareType type = FIELD, size_t gold = 0)
      : SquareStopBase(FIELD, newSquaresExplored),
        gold_(gold){}

  size_t gold() {
    return gold_;
  }

  bool take_gold() {
    if (gold() > 0) {
      --gold_;
      return true;
    }
    return false;
  }

  void put_gold() {
    ++gold_;
  }

  virtual ~SquareField(){}

protected:
  size_t gold_;
};
class SquareSpinningBase: public SquareField { // Вертушки
  /// Золото ложится на клетку в целом. Достается выбившему.
  /// Если бросить золото, то достается первому, кто сделает ход на эту/этой клетке.
  /// Хотим ли это улучшить?
public:
  SquareSpinningBase(SquareType type, size_t max_position)
      : SquareField(type),
        max_position_(max_position) {}

  size_t max_position() {
    return max_position_;
  }

  virtual ~SquareSpinningBase(){}
private:
  size_t max_position_;
};
class SquareJungle: public SquareSpinningBase {
public:
  SquareJungle()
      : SquareSpinningBase(JUNGLE, 1) {}

  ~SquareJungle() {};
};
class SquareDesert: public SquareSpinningBase {
public:
  SquareDesert()
      : SquareSpinningBase(DESERT, 2) {}

  ~SquareDesert() {};
};
class SquareBog: public SquareSpinningBase {
public:
  SquareBog()
      : SquareSpinningBase(BOG, 3) {}

  ~SquareBog() {};
};
class SquareMountains: public SquareSpinningBase {
public:
  SquareMountains()
      : SquareSpinningBase(MOUNTAINS, 4) {}

  ~SquareMountains() {};
};
class SquareTrap: public SquareField {
public:
  bool someone_traped; /// Поведение клетки зависит от этой переменной.
  /// Уйти с клетки можно только если она false. Когда попадаешь на клетку,
  /// если там нет человека(а врагов ты уже выбил), someone_traped меняется на true.
  SquareTrap()
      : SquareField(TRAP),
        someone_traped(false) {}

  ~SquareTrap() {}
};
class SquareRum: public SquareField {
public:
  SquareRum()
      : SquareField(RUM),
        drunk_during_turn_(0) {}

  size_t drunk_during_turn() const {
    return drunk_during_turn_;
  }

  void drink(size_t turn) {
    drunk_during_turn_ = turn + numberOfPlayers;
  }

  ~SquareRum() {}
private:
  size_t drunk_during_turn_; /// current turn + 4
};
class SquareFortress: public SquareStopBase {
 public:
  SquareFortress(SquareType type)
      : SquareStopBase(type, newSquaresExplored) { }

  SquareFortress()
      : SquareStopBase(FORTRESS, newSquaresExplored) { }

  virtual ~SquareFortress() {}
};
class SquareAborigine : public SquareFortress {
public:
  SquareAborigine()
      : SquareFortress(ABORIGINE) {}

  ~SquareAborigine() {}
};
class SquareArrow : public SquareBase {
public:
  SquareArrow(const vector<Direction>& escape_directions)
      : SquareBase(ARROW, newSquaresExplored),
        escape_directions_(escape_directions) {}

  SquareArrow() : SquareBase(ARROW, newSquaresExplored) {}

  EffectOfSquare effectType(size_t player_id, const Pirate& pirate) const {
    if (escape_directions_.size() == 1) {
      return GOON;
    }
    return ASK;
  }

  const vector<Direction>& escape_directions() const{
    return escape_directions_;
  }

  ~SquareArrow() { }

  string info() const {
    string result;
    result.push_back(char(type()));
    result.push_back(' ');
    for (auto dir:escape_directions_) {
      result.push_back(char(dir));
    }
    return result;
  }

private:
  vector<Direction> escape_directions_;
};
class SquareHorse: public SquareBase {
public:
  SquareHorse()
      : SquareBase(HORSE, newSquaresExplored) {}

  EffectOfSquare effectType(size_t player_id, const Pirate& pirate) const {
    return ASK;
  }

  ~SquareHorse() { };
};
class SquareIce: public SquareBase {
public:
  SquareIce(Point coordinate)
      : SquareBase(ICE, newSquaresExplored),
        coordinate_(coordinate),
        lastMoveDir_(HORSEDIR) {}

  EffectOfSquare effectType(size_t player_id, const Pirate& pirate) const {
    if (pirate.coordinate().GetDirectionByPoint(coordinate_) == 1) {
      return ASK; // if player is on this square, coordinate_ == pirate.coordinate.
       // pirate should remember his history. You will have to run this function before
       // pirate is here?
    }
    return GOON;
  }

  string info() const {
    string result;
    std::stringstream stream(result);
    stream << char(type()) << ' ' << coordinate_.x << ' ' << coordinate_.y;
    return result;
  }

  ~SquareIce() { };
private:
  Point coordinate_;
  Direction lastMoveDir_;
};
// class SquareGoonBase: public SquareBase {};
class Ship: public SquareStopBase {
public:
  Ship(Point coord, size_t owner_id)
      : SquareStopBase(SHIP, true),
      coordinate_(coord),
      owner_id_(owner_id) { }

  Point coordinate() const {
    return coordinate_;
  }

  EffectOfSquare effectType(size_t player_id, const Pirate& pirate) const {
    if (player_id == owner_id_)
      return STOP;
    return KILL;
  }

  string info() const{
    string result;
    std::stringstream stream(result);
    stream << char(type()) << ' ' << coordinate().x << ' ' << coordinate().y << ' ' << owner_id_;
    return result;
  }

  ~Ship() {}
private:
  Point coordinate_;
  size_t owner_id_;
};
class SquareCrocodile : public SquareBase {
public:
  SquareCrocodile() : SquareBase(CROCODILE, newSquaresExplored) { }

  EffectOfSquare effectType(size_t player_id, const Pirate& pirate) const {
    return GOON;
  }

  ~SquareCrocodile() {};

  Point prev_coord_;
};
class SquareBaloon : public SquareBase {
public:
  SquareBaloon() : SquareBase(BALOON, newSquaresExplored) { }

  EffectOfSquare effectType(size_t player_id, const Pirate& pirate) const {
    return GOON;
  }
  ~SquareBaloon() {}
};
class SquareGun : public SquareBase {
public:
  SquareGun() : SquareBase(GUN, newSquaresExplored) { }
  EffectOfSquare effectType(size_t player_id, const Pirate& pirate) const {
    return GOON;
  }
  ~SquareGun() {}
};
class SquareCanibal : public SquareBase {
public:
  SquareCanibal() : SquareBase(CANNIBAL, newSquaresExplored) { }
  EffectOfSquare effectType(size_t player_id, const Pirate& pirate) const {
    return KILL;
  }
  ~SquareCanibal() {}
};


class FactoryForSquares {
 public:
  static FactoryForSquares& Instance() {
    static FactoryForSquares factory;
    return factory;
  }

  SquareBase* createSquare(SquareType stype, string params="") {
    switch (stype) {
      case UNEXPLORED:
        return new SquareBase();
      case WATER:
        return new SquareWater();
      case FIELD:
        return new SquareField();
      case JUNGLE:
        return new SquareJungle();
      case DESERT:
        return new SquareDesert();
      case BOG:
        return new SquareBog();
      case MOUNTAINS:
        return new SquareMountains();
      case TRAP:
        return new SquareTrap();
      case RUM:
        return new SquareRum();
      case HORSE:
        return new SquareHorse();
      case ICE: {
        vector<size_t> vec=stringToVector(params);
        if (vec.size() != 2) {
          throw std::runtime_error("Wrong parameters number for ice: " + params + ", 2 required.");
        }
        return new SquareIce(Point(vec[0], vec[1]));
      }
      case CROCODILE:
        return new SquareCrocodile;
      case ARROW: {
        vector<Direction> vec;
        for (auto it:params) {
          vec.push_back(static_cast<Direction>(it));
        }
        return new SquareArrow(vec);
      }
      case BALOON:
        return new SquareBaloon();
      case GUN:
        return new SquareGun();
      case CANNIBAL:
        return new SquareCanibal();
      case FORTRESS:
        return new SquareFortress();
      case ABORIGINE:
        return new SquareAborigine();
      case SHIP: {
        vector<size_t> vec = stringToVector(params);
        if (vec.size() != 3) {
          throw std::runtime_error("Wrong parameters number for Ship: " + params + ", 3 required.");
        }
        return new Ship(Point(vec[0], vec[1]), vec[2]);
      }
    }
    return new SquareBase;
  }

  SquareBase* createSquare(string info){
    // std::stringstream stream(info);
    SquareType type = static_cast<SquareType>(info[0]); // Поскольку SquareType --- char.
    info.erase(0, 2);
    return createSquare(type, info);
  }

private:
  static vector<size_t> stringToVector(string info) {
    info.push_back(' '); // otherwise empty string can be processed in a strange way
    std::stringstream stream(info);
    vector<size_t> return_vector;
    size_t tmp;
    stream >> tmp;
    while (!stream.eof()) {
      return_vector.push_back(tmp);
      stream >> tmp;
    }
    return return_vector;
  }

  FactoryForSquares() {}
  FactoryForSquares(const FactoryForSquares &);
  FactoryForSquares &operator = (const FactoryForSquares &);
  ~FactoryForSquares() {}
};



class Event: public Request {
public:
  string square_info;

  Event(EventType type, size_t player_id, size_t pirate_num, Point destination,
        size_t position_on_square, string square_info)
      : Request(type, player_id, pirate_num, destination, position_on_square),
        square_info(square_info) { }

  Event(Request req, string square_info)
      : Request(req),
        square_info(square_info) { }
};

class Player {
public:
  size_t id;
  string login;
  std::vector<Pirate> pirates;
  Ship* ship;

  Player(size_t id, string login, Point ship_coord)
      : id(id),
        login(login),
        pirates(),
        ship(new Ship(ship_coord, id)) {
      for (size_t i=0; i < numberOfPirates; ++i){
        pirates.push_back(Pirate(ship_coord));
      }
      std::cout << "Player constructor" << std::endl;
    }

  virtual ~Player() { }
};


class GameMap: public vector<vector<SquareBase*> > {
public:
  GameMap(size_t size = (sizeOfIsland + 2))
      : vector<vector<SquareBase*> >(0)
    {
      init(size);
      std::cout << "Map constructor" << std::endl;
    }

    void init(size_t size); /// Эта функция реализуется по разному для сервера и клиента

    ~GameMap() {
      for (auto i:*this) {
        for (SquareBase* j: i){
          delete j;
        }
      }
    }

};


class GameHolder {
public:
  GameHolder(vector<Player*>& players, size_t size = sizeOfIsland + 2)
      : map_(size),
      turn_(0),
        players_(players) {
        std::cout << "Game constructor" << std::endl;
      }

  SquareType get_square_type(Point p) const {
    return map_[p.x][p.y]->type();
  }

  string get_square_info(Point p) const {
    return map_[p.x][p.y]->info();
  }

  vector<Pirate*> getPiratesAtPoint(Point coor) const {
    vector<Pirate*> result;
    for (Player* player: players_) {
      for (Pirate pirate: player->pirates) {
        if (pirate.coordinate() == coor) {
          result.push_back(&pirate);
        }
      }
    }
    return result;
  }

  bool possible_req(Request& req) const {
    return true;
  }
  bool accept(Request& req) {
    if (!possible_req(req)) {
      return false;
    }
    /// Do something
    return true;
    // здесь прописываются все изменения на карте: с пиратом и с клеткой под ним
    // на которого указывает событие (перемещение или бросок монетки или смерть)
    // но не вся цепочка событий
    // в клиенте происходит просто accept всех событий отправленных сервером
  }

  Request generate_request(size_t player_id, size_t pirate_num) {
    // Будет определять, что происходит с пиратом когда он оказался на клетке,
    // которая не дает выбора
  }

  /*SquareBase* operator[](const Player &p) {
    return map_[p.coord.x][p.coord.y];
  }
  void Bash(Point coord, Direction direction) {
    int size = map_.size();
    (*this)[coord][direction] = false;
    Point adjacent_point = GetPointToThe(coord, direction);
    if (IsCorrectPoint(adjacent_point, map_.size())) {
      (*this)[coord][GetOppositeDirection(direction)] = false;
    }
  }*/
  int size() const {  // name should be lowercase.
    return map_.size();
  }

  vector<Request> attack(const Player* player, Point coor, size_t position) const {
    std::cout << player->login << " attacks " << coor.x <<", "<< coor.y << ", " << position << std::endl;
    vector<Request> result(0);
    for (Player* other_player: players_) {
      if (player->id == other_player->id) {
      } else {
        for (size_t i=0; i<numberOfPirates; ++i) {
          if (player->pirates[i].coordinate() == coor && player->pirates[i].position_on_square() == position) {
            if (get_square_type(coor) == WATER || get_square_type(coor) == SHIP) {
              result.push_back(Request(EventType::DEATH, other_player->id, i, coor, 0));
            } else {
              result.push_back(Request(EventType::MOVE, other_player->id, i, other_player->ship->coordinate(), 0));
            }
          }
        }
      }
    }
    return result;
  }

  bool resurrect(Player* player, Point coord, Request &request) {
    for (size_t i=0; i<numberOfPirates; ++i) {
      if (!player->pirates[i].alive_) {
        request = Request(MOVE, player->id, i, coord, 0);
        return true;
      }
    }
    return false;
  }

  ~GameHolder() {
    for (Player* player:players_) {
      delete(player);
    }
    std::cout << "Game holder destructor" << std::endl;
  }

protected:
  GameMap map_;  // field_[0][0] is a Left Bottom corner.
  size_t turn_;
public:
  std::vector<Player*> players_;
};

