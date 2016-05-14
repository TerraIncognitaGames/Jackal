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
                      TOPLEFT, BOTTOMRIGHT, BOTTOMLEFT, NONE, HORSEDIR};
enum SquareType {UNEXPLORED, WATER, FIELD, JUNGLE, DESERT, BOG, MOUNTAINS,
                       TRAP, RUM, ARROW, HORSE, ICE, CROCODILE, BALOON,
                       GUN, CANNIBAL, FORTRESS, ABORIGINE, SHIP };
enum EffectOfSquare { STOP, GOON, ASK, KILL };
enum EventType { DROPGOLD, MOVE, DEATH };


struct Point {
  int x, y, position;
  Point()
    : x(100), y(100), position(100){  }
  Point(int x_coord, int y_coord, int position=0)
    : x(x_coord), y(y_coord), position(position) { }

  bool operator== (const Point &other) const {
    return ((other.x == x) && (other.y == y) && other.position == position);
  }
};

Point operator+(const Point& point, int dp) {
  return Point(point.x, point.y, point.position + dp);
}

const Direction& GetOppositeDirection(const Direction& dir) {
  switch (dir) {
    case TOP:
      return BOTTOM;
    case BOTTOM:
      return TOP;
    case RIGHT:
      return LEFT;
    case LEFT:
      return RIGHT;
    case TOPRIGHT:
      return BOTTOMLEFT;
    case TOPLEFT:
      return BOTTOMRIGHT;
    case BOTTOMLEFT:
      return TOPRIGHT;
    case BOTTOMRIGHT:
      return TOPLEFT;
    default:
      return dir;
  }
}


const Point& GetPointToThe(const Point& from, Direction dir,
                           const Point& history=Point(0,0,-1)) {
  int x = from.x;
  int y = from.y;
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
    case NONE:
      return Point(x, y);
    case HORSEDIR:
      if (history.position == -1) {  // shouldn't be happening
        throw std::runtime_error("Incorrect or missing history in \
                                  game.h::GetPointToThe()");
      } else {
        int dx = x - history.x;
        int dy = y - history.y;
        return Point(x + dx, y + dy);
      }
      return Point(0, 0);
  }
}

Direction GetDirection(const Point& first, const Point& second) {
  int dx = second.x - first.x;
  int dy = second.y - first.y;
  if (dx < 0) {
    if (dy  < 0) {
      return BOTTOMLEFT;
    }
    if (dy == 0) {
      return LEFT;
    }
    if (dy > 0) {
      return TOPLEFT;
    }
  }
  if (dx > 0) {
    if (dy < 0) {
      return BOTTOMRIGHT;
    }
    if (dy == 0) {
      return RIGHT;
    }
    if (dy < 0) {
      return TOPRIGHT;
    }
  }
  if (dx == 0) {
    if (dy < 0) {
      return BOTTOM;
    }
    if (dy > 0) {
      return TOP;
    }
  }
  return NONE;
}

bool IsCorrectPoint(const Point &point, const int size) {
  return (point.x >= 0 && point.x < size && point.y >= 0 && point.y < size &&
          !((point.x == size||point.x == 0) && (point.y == size || point.y == 0)));
}

vector<Point> GetPotentialAvailablePointsFrom(const Point &point) {
  vector<Direction> directions({TOP, BOTTOM, LEFT, RIGHT, TOPRIGHT,
                                TOPLEFT, BOTTOMRIGHT, BOTTOMLEFT});
  vector<Point> potential_points(10);
  potential_points.push_back(point + 1);
  for (Direction dir : directions) {
    potential_points.push_back(GetPointToThe(point, dir));
  }
  return potential_points;
}


class Pirate {
private:
  bool gold_;
  Point coordinate_;
  bool alive_;
  int owner_id_;

public:
  Pirate(Point coord, int owner_id, bool gold=false)
      : gold_(gold),
        coordinate_(coord),
        alive_(true), owner_id_(owner_id) {
      std::cout << "Pirate created" << std::endl;
    }

  int get_owner_id() const {
    return owner_id_;
  }
  bool has_gold() const {
    return gold_;
  }
  Point coordinate() const {
    return coordinate_;
  }
  bool alive() const {
    return alive_;
  }

  void kill() {
    alive_ = false;
    gold_ = false;
    coordinate_ = Point(-1, -1, -1);
  }

  ~Pirate() {}

  friend class GameHolder;
};

struct Request {
  EventType type;
  size_t player_id;
  size_t pirate_num;
  Point destination;
  Request(EventType type, size_t player_id, size_t pirate_num,
          Point destination)
      : type(type),
        player_id(player_id),
        pirate_num(pirate_num),
        destination(destination)
         { }
  Request()
    : type(MOVE),
      player_id(100),
      pirate_num(100),
      destination(0, 0, 0)
      {}

  ~Request() {}
};


class SquareBase {
private:
  SquareType type_;
  bool explored_;
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
        gold_(gold) {}

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
    return GOON;
  }

  ~SquareHorse() { };
};
class SquareIce: public SquareBase {
public:
  SquareIce(Point coordinate)
      : SquareBase(ICE, newSquaresExplored),
        coordinate_(coordinate),
        lastMoveDir_(HORSEDIR) {}

  EffectOfSquare effectType(/*int player_id, const Pirate& pirate*/) const {
    // if (GetDirection(pirate.coordinate(), coordinate_) == 1) {
    //   return ASK;
    // }
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
    if (pirate.get_owner_id() == owner_id_)
      return STOP;
    return KILL;
  }

  size_t owner_id() {
    return owner_id_;
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
  SquareCrocodile() : SquareBase(CROCODILE, newSquaresExplored), prev_coord_(0, 0, 0) { }

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
        }  // WTF?????
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
         string square_info)
      : Request(type, player_id, pirate_num, destination),
        square_info(square_info) { }

  Event(Request req, string square_info)
      : Request(req),
        square_info(square_info) { }
};

struct Player {
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
        pirates.push_back(Pirate(ship_coord, id));
      }
      std::cout << "Player constructor" << std::endl;
    }

  virtual ~Player() { }
};


class GameMap: public vector<vector<SquareBase*>> {
 public:
  GameMap(size_t size = (sizeOfIsland + 2))
      : vector<vector<SquareBase*>>(0) {
    Initialize(size);
    std::cout << "Map constructor" << std::endl;
  }

  void Initialize(size_t size); /// Эта функция реализуется по разному для сервера и клиента

  /* vector<SquareBase*> operator[](size_t i) {
    return vector<vector<SquareBase*>>::operator[](i);
  } */

  SquareBase* operator[](const Point &point) {
    return vector<vector<SquareBase*>>::operator[](point.x)[point.y];
  }

  SquareBase* operator[](const Point &point) const {
    return vector<vector<SquareBase*>>::operator[](point.x)[point.y];
  }

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
  SquareType get_square_type(const Point& pt) const {
    return map_[pt]->type();
  }
  GameHolder(vector<Player*>& players, size_t size = sizeOfIsland + 2)
      : map_(size), turn_(0), players_(players) {
    std::cout << "Game constructor" << std::endl;
  }
  bool IsPossibleRequest(Request& req) const {
    const Pirate pirate = players_[req.player_id]->pirates[req.pirate_num];
    switch (req.type) {
      case MOVE:
        return PirateCanGoTo(pirate, req.destination);
      default:
        return true;
    }
  }
  bool Accept(Request& req) {
    if (!IsPossibleRequest(req)) {
      return false;
    }
    /// Do something
    return true;
    // здесь прописываются все изменения на карте: с пиратом и с клеткой под ним
    // на которого указывает событие (перемещение или бросок монетки или смерть)
    // но не вся цепочка событий
    // в клиенте происходит просто accept всех событий отправленных сервером
  }
protected:
  vector<Request> Resurrect(Player* player, Point coord) const {
    vector<Request> res(0);
    for (size_t i=0; i<numberOfPirates; ++i) {
      if (!player->pirates[i].alive_) {
        res.push_back(Request(MOVE, player->id, i, coord));
        return res;
      }
    }
    return res;
  }
 public: /// TEMP
  std::vector<Player*> players_;
 protected:
  int map_size() const {
    return map_.size();
  }
  SquareType square_type(Point p) const {
    return map_[p]->type();
  }
  string square_info(Point p) const {
    return map_[p]->info();
  }
  vector<Pirate*> GetPiratesAtSquare(Point coor) const {
    vector<Pirate*> result;
    for (Player* player: players_) {
      for (Pirate pirate: player->pirates) {
        if (pirate.coordinate().x == coor.x &&
            pirate.coordinate().y == coor.y)  {
          result.push_back(&pirate);
        }
      }
    }
    return result;
  }
//  Pirate* GetPirateAtPoint(const Point& coor) {
//    for (Player* player: players_) {
//      for (Pirate pirate: player->pirates) {
//        if (pirate.coordinate() == coor)  {
//          return &pirate;
//        }
//      }
//    }
//    return nullptr;
//  }
  bool PirateCanGoTo(const Pirate& pirate, const Point &to) const { /// REDO!!!! Read rules and redo! and test!
    const Point from = pirate.coordinate();
    SquareBase* from_square = map_[from];
    SquareBase* to_square = map_[to];
    switch (square_type(from)) {
      case WATER:
        return (square_type(to) == WATER || square_type(to) == SHIP);
      case TRAP:
        return false; /// REDO
      default:
        if (square_type(to) == WATER) { /// REDO case of arrow
          return false;
        }
    }
    switch (square_type(from)) {
      case JUNGLE:
      case BOG:
      case DESERT:
      case MOUNTAINS:
        if (!(from.position ==
             dynamic_cast<SquareSpinningBase*>(from_square)->max_position())) {
          return (!(pirate.has_gold() && (GetPiratesAtSquare(from + 1)).size()) &&
              to == from + 1); /// REDO frendly pirate! REDO надо сделать общую проверку наличия пирата в точке назначения
        }
      default:
          /* here we already know, that pirate goes to another square
          // and also we know, that from_ and to_ squares both are not water_squares
          // and from_square is not a trap_square
          // let's switch to_square, since we already have no interest in from_square
          // interesting cases are:
          //   SHIP (check, it's friendly)
          //   CROCODILE (return false)
          //   FORTRESS and ABORIGINE (check that it's empty)
          //   default: check that either pirate hasn't coin or to_square is empty */
          int distance_to_square = pow(from.x - to.x, 2) + pow(from.y - to.y, 2);
          if (distance_to_square * (distance_to_square - 3) >= 0) {
            return false;  // square is out of sight
          }
          switch(square_type(to)) {
            case SHIP:
              return (dynamic_cast<Ship*>(to_square)->owner_id() ==
                      pirate.get_owner_id());
            case CROCODILE:
              return false;
            case FORTRESS:
            case ABORIGINE:
              return !bool(GetPiratesAtSquare(to).size());
            default:
                return !(pirate.has_gold() && bool(GetPiratesAtSquare(to).size()));
          }
    }
  }
  vector<Point> GetAvailablePoints(const Pirate& pirate) const {
    const Point from = pirate.coordinate();
    const SquareBase* square = map_[from];
    vector<Point> available_points;
    vector<Point> potential_points = GetPotentialAvailablePointsFrom(from);
    for (const Point& potential_point : potential_points) {
      if (PirateCanGoTo(pirate, potential_point)) {
        available_points.push_back(potential_point);
      }
    }
    return available_points;
  }
  Request GenerateRequest(size_t player_id, size_t pirate_num) {
    // Будет определять, что происходит с пиратом когда он оказался на клетке,
    // которая не дает выбора
  }
  vector<Request> Attack(const Player* player, Point coor) const {
    std::cout << player->login << " attacks " << coor.x <<", "<< coor.y
              << ", " << coor.position << std::endl;
    vector<Request> result(0);
    for (Player* other_player: players_) {
      if (player->id == other_player->id) {
      } else {
        for (size_t i=0; i<numberOfPirates; ++i) {
          if (player->pirates[i].coordinate() == coor) {
            if (get_square_type(coor) == WATER || get_square_type(coor) == SHIP) {
              result.push_back(Request(EventType::DEATH, other_player->id, i, coor));
            } else {
              result.push_back(Request(EventType::MOVE, other_player->id, i, other_player->ship->coordinate()));
            }
          }
        }
      }
    }
    return result;
  }
public:
  ~GameHolder() {
    for (Player* player:players_) {
      delete(player);
    }
    std::cout << "Game holder destructor" << std::endl;
  }

protected:
  GameMap map_;  // field_[0][0] is a Left Bottom corner.
  size_t turn_;
};

