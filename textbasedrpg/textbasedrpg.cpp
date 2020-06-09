// textbasedrpg.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>

class Item {
public:
    std::string name;
    int health, attack, defence;
    Item(std::string n, int hp, int atk, int def)
    {
        name = n;
        health = hp;
        attack = atk;
        defence = def;
    }



};
class GameCharacter {
public:
    std::string ename;
    int curr_health, max_health, attack, defence;
    GameCharacter()
    {
        std::string n = "", int h = 5, int a = 0, int d = 0;
    }
    GameCharacter(std::string n, int hp, int atk, int def) {
        ename = n;
        max_health = hp;
        curr_health = max_health;
        attack = atk;
        defence = def;
    }


    int takeDamage(int amount)
    {
        int damage = amount - defence;
        if (damage < 0)
        {
            damage = 0;
        }
        curr_health -= damage;
        return damage;
    }
    bool isDead()
    {
       /* if (curr_health == 0)
        {
            return true;
        }
        else
            return false;*/
        return curr_health <= 0;
    }

};
class Player: public GameCharacter {
public:
    std::vector<Item> inventory;
    Room *curr_room;
    Room *prev_room;
    Player()
    {
        std::string n = "", int h = 5, int a = 0, int d = 0;
    }
    Player(std::string n, int hp, int atk, int def) :GameCharacter(n,hp,atk,def)
    {
        Item dagger = Item("Dagger",0,5,0);
        addItem(dagger);

    }
    void addItem(Item i)
    {
        inventory.push_back(i);
        increaseStats(i.health, i.attack, i.defence);
    }
    void increaseStats(int h, int a, int d) {
        curr_health += h;
        max_health += h;
        attack += a;
        defence += d;
  
    }
    void lootRoom(Room* room) {
        std::vector<Item> items = room->items;
        room->getLoot();
        for (int i = 0; i < items.size(); i++)
        {
            addItem(items[i]);
        }

    }
    void changeRoom(Room* t)
    {
        prev_room = curr_room;
        curr_room = t;
    }
};
class Room {
public:
    int room_number;
    bool isExit;
    std::vector<Item> items;
    std::vector<GameCharacter> enemies;
    Room(int rn = 0, bool exit = false, std::vector<Item> i = std::vector<Item>(), std::vector<GameCharacter> gc = std::vector<GameCharacter>());
    Room(int p, bool e, std::vector<Item> I, std::vector<GameCharacter> g)
    {
        room_number = p;
        isExit = e;
        items = I;
        enemies = g;
    }

    void getLoot()
    {
        items.clear();
    }
    void clearEnemies()
    {
        enemies.clear();
    }



};
class Dungeon {
public: 
    Player player;
    std::vector<Room> room_array;
    Dungeon(Player p)
    {
        player = p;
    }
    int runDungeon()
    {
        std::cout << "Welcome to the Dungeon" << std::endl;
        player.curr_room = &room_array[0];
        player.prev_room = &room_array[0];
        
        while (true)
        {

        }
    }
    void enterRoom(Room* r)
    {
        if (r->enemies.size() != 0)
        {
            handleCombat(r);
        }
        else if (r->items.size() != 0)
        {
            handleLoot(r);
        }
        else
        {

        }
    }
    void emptyRoom(Room* er);
    void lootRoom(Room* lr);
    void enemyRoom(Room* fr);
    void handleLoot(Room* hl);
    void handleCombat(Room* hc);
    void moveRoom(Room* mr);
    void printAction(int, std::string[]);
    int endGame();













};
int main()
{
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
