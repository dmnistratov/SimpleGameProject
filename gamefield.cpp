#include "gamefield.h"
#include "bonus.h"

GameField* GameField::instance_p = nullptr;
GameField *GameField::GetInstance()
{
     if (!instance_p)
         instance_p = new GameField();
     return instance_p;
}

GameField::GameField()
{
    m_width = 0, m_height = 0, field = nullptr;
    keyb = nullptr, tpb = nullptr, rainb = nullptr;
    file = new File("logfile.txt");
    fieldlog = new FieldLogger(file);
    *fieldlog << "field created";
}

GameField::~GameField()
{
        *fieldlog << "deconstructor";
    for (size_t i = 0; i < m_width; i++){
        for (size_t j = 0; j < m_height; j++)
            delete field[i][j];
        delete field[i];
    }
    delete field;
    delete keyb;
    delete tpb;
    delete rainb;
}

void GameField::setSize(size_t w, size_t h)
{
        *fieldlog << "new size grid";
    if (w <= 0 || h <= 0 || w  > MAP_RESOLUTION || h > MAP_RESOLUTION)
        throw std::invalid_argument("Size <= 0 or > MAP_RESOLUTION");

    m_width = w;
    m_height = h;
}

// return Cell***?
Cell*** GameField::createField()
{
        *fieldlog << "field filled with rect";
    if (m_width == 0 || m_height == 0)
        throw std::invalid_argument("field creation error, field already exists or arg == 0");

    field = new Cell**[m_height];
    for (size_t i = 0; i < m_height; i++){
        field[i] = new Cell*[m_width];
        for (size_t j = 0; j < m_width; j++){
            field[i][j] = new Cell();
            field[i][j]->setX(j);
            field[i][j]->setY(i);
            field[i][j]->setScale((MAP_RESOLUTION/m_height));
            field[i][j]->setRect((MAP_RESOLUTION/m_width)*j, (MAP_RESOLUTION/m_height)*i, (MAP_RESOLUTION/m_width), (MAP_RESOLUTION/m_height));
        }
    }
    bonuses = new BonusFactory();
    keyb = bonuses->CreateBonus(BonusType::key_b);
    tpb = bonuses->CreateBonus(BonusType::tp_b);
    rainb = bonuses->CreateBonus(BonusType::rainbow_b);
    return field;
}

void GameField::clearField()
{
    for (size_t i = 0; i < m_height; i++){
        for (size_t j = 0; j < m_width; j++){
            delete field[i][j];
        }
        delete field[i];
    }
    delete field;
}

class GameFieldIterator GameField::begin()
{
    return GameFieldIterator((*field), m_width, m_height, field);
}

class GameFieldIterator GameField::end()
{
    return GameFieldIterator(*(field+m_height-1)+m_width, m_width, m_height, field);
}

void GameField::PrintXY()
{
    GameField* grid = GameField::GetInstance();
    GameFieldIterator st_test = grid->begin();
    GameFieldIterator end = grid->end();
    for (; st_test != end; ++st_test){
        std::cout << (*st_test)->getX() << " " << (*st_test)->getY() << " " << std::endl;
    }
}

void GameField::generateMap()
{
        *fieldlog << "start of generation";
    GameField* grid = GameField::GetInstance();
    keyb = bonuses->CreateBonus(BonusType::key_b);
    bool border = false, exit_set = false, entry_set = false, key_set = false, tp_set = false, rainbow_set = false;
    int rand_entry = (rand()-m_height%3)%(m_width*m_height);
    int rand_exit = (rand()+m_height%2+5)%(m_width*m_height);
    int tms = 0;
    for (GameFieldIterator temp = grid->begin(); temp != grid->end(); ++temp, tms++)
    {
        border = ((*temp)->getX() == 0 || (*temp)->getY() == 0 || (*temp)->getX() == (int)m_width-1 || (*temp)->getY() == (int)m_height-1);
        if ((*temp)->getY() % 2 == 1 && ((*temp)->getX()-3 * rand()%m_height) % 5-(rand()%2)+1 == 0)
            (*temp)->setType(Cell::empty);
        if (border)
            (*temp)->setType(Cell::border);
        if ((rand_entry == tms && !entry_set && !border) \
                || ((*temp)->getX() == (int)m_width-4 && (*temp)->getY() == (int)m_height-2 && !entry_set))
        {
                *fieldlog << "entry set";
            entry_set = true;
            (*temp)->setType(Cell::entry);
        }
        if ((rand_exit == tms && !exit_set && (*temp)->getType() != Cell::entry && !border) \
                || ((*temp)->getX() == (int)m_width-2 && (*temp)->getY() == (int)m_height-2 && !exit_set))
        {
                *fieldlog << "exit set";
            exit_set = true;
            (*temp)->setType(Cell::exit);
        }
        if ((( (int)(rand() % ((m_height-4)*(m_width-4))) == tms && !key_set) && !border)\
                || ((*temp)->getX() == (int)m_width-4 && (*temp)->getY() == (int)m_height-4 && !key_set))
        {
                *fieldlog << "bonus key set";
            keyb->place((*temp)->getX(), (*temp)->getY());
            keyb->setScale((MAP_RESOLUTION/m_width));
            key_set = true;
        }
        if ((( (int)(rand() % ((m_height-4)*(m_width-4))) == tms && !tp_set) && !border) \
                || ((*temp)->getX() == (int)m_width-7 && (*temp)->getY() == (int)m_height-2 && !tp_set))
        {
                *fieldlog << "bonus teleport set";
            tpb->place((*temp)->getX(), (*temp)->getY());
            tpb->setScale((MAP_RESOLUTION/m_width));
            tp_set = true;
        }
        if ((( (int)(rand() % ((m_height-4)*(m_width-4))) == tms && !rainbow_set) && !border)\
                || ((*temp)->getX() == (int)m_width-3 && (*temp)->getY() == (int)m_height-2 && !rainbow_set))
        {
                *fieldlog << "bonus rainbow set";
            rainb->place((*temp)->getX(), (*temp)->getY());
            rainb->setScale((MAP_RESOLUTION/m_width));
            rainbow_set = true;
        }
    }
}

void GameField::createMap(std::string mapstr)
{
    std::stringstream ss;
    ss << mapstr;
    int val;
    GameField* grid = GameField::GetInstance();
    for (GameFieldIterator temp = grid->begin(); temp != grid->end(); ++temp)
    {
        ss >> val;
        if (val < -2 || val > 2)
            throw std::invalid_argument("map currupted");
        (*temp)->setType(val);
    }
}

void GameField::loadKey(std::string keystr)
{
    std::stringstream ss;
    ss << keystr;
    int x, y;
    int val;
    ss >> x;
    if (x > (int)m_width || x < 0)
        throw std::invalid_argument("X error");
    ss >> y;
    if (y > (int)m_height || y < 0)
        throw std::invalid_argument("Y error");
    keyb->place(x, y);
    ss >> val;
    keyb->setScale(val);
    ss >> val;
    keyb->setStat(val);
}

void GameField::loadTp(std::string tpstr)
{
    std::stringstream ss;
    ss << tpstr;
    int x, y;
    int val;
    ss >> x;
    if (x > (int)m_width || x < 0)
        throw std::invalid_argument("X error");
    ss >> y;
    if (y > (int)m_height || y < 0)
        throw std::invalid_argument("Y error");
    tpb->place(x, y);
    ss >> val;
    tpb->setScale(val);
}

void GameField::loadRainb(std::string rainbstr)
{
    std::stringstream ss;
    ss << rainbstr;
    int x, y;
    int val;
    ss >> x;
    if (x > (int)m_width || x < 0)
        throw std::invalid_argument("X error");
    ss >> y;
    if (y > (int)m_height || y < 0)
        throw std::invalid_argument("Y error");
    rainb->place(x, y);
    ss >> val;
    rainb->setScale(val);
}


bool GameField::isCollide(int x, int y)
{
    if (x > (int)m_width || x < 0)
        throw std::invalid_argument("X error");
    if (y > (int)m_height || y < 0)
        throw std::invalid_argument("Y error");
    if (field[y][x]->getType() == Cell::empty || field[y][x]->getType() == Cell::border)
        return true;
    return false;
}

bool GameField::isExit(int x, int y)
{
    if (x > (int)m_width || x < 0)
        throw std::invalid_argument("X error");
    if (y > (int)m_height || y < 0)
        throw std::invalid_argument("Y error");
    if (field[y][x]->getType() == Cell::exit)
        return true;
    return false;
}

IBonus* GameField::pickup(int x, int y)
{
    if (x > (int)m_width || x < 0)
        throw std::invalid_argument("X error");
    if (y > (int)m_height || y < 0)
        throw std::invalid_argument("Y error");
    if (keyb->getX() == x && keyb->getY() == y)
    {
        return keyb;
    }
    else if (rainb->getX() == x && rainb->getY() == y)
        return rainb;
    else if (tpb->getX() == x && tpb->getY() == y)
        return tpb;
    return nullptr;
}

std::string GameField::saveMap()
{
    std::string mapstring;
    GameField* grid = GameField::GetInstance();
    for (GameFieldIterator temp = grid->begin(); temp != grid->end(); ++temp)
    {
        mapstring.append(std::to_string((*temp)->getType()));
        mapstring.append(" ");
    }
    mapstring.append("\n");
    mapstring.append(keyb->saveBonus());
    mapstring.append(tpb->saveBonus());
    mapstring.append(rainb->saveBonus());
    return mapstring;
}
