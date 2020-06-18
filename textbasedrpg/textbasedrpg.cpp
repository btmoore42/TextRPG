// textbasedrpg.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>

/*Contains items that can be found in the dungeon. Each item is composed of the same 4 stats and the name of the item*/
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
/*Simple base character class that can be used to create enemies in the dungeon*/
class GameCharacter {
public:
    std::string ename;
    int curr_health, max_health, attack, defence;
    GameCharacter()
    {
        std::string n = "";
        int h = 5;
        int a = 1; 
        int d = 0;
        ename = n;
        max_health = h;
        curr_health = max_health;
        attack = a;
        defence = d;
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
/*Holds all the room information for the dungeon*/
class Room {
public:
    int room_number;
    bool isExit; //if there is an exit in  a room
    std::vector<Item> items; //holds the items for a specific room
    std::vector<GameCharacter> enemies; //holds the enemy encounters for a specific room
    Room()
    {
        int rn = 0;
        bool exit = false;
        std::vector<Item> i = std::vector<Item>();
        std::vector<GameCharacter> gc = std::vector<GameCharacter>();
        room_number = rn;
        isExit = exit;
        items = i;
        enemies = gc;
    }
    Room(int p, bool e, std::vector<Item> I, std::vector<GameCharacter> g)
    {
        room_number = p;
        isExit = e;
        items = I;
        enemies = g;
    }

    void getLoot() //take the loot found in a room and clear the vector 
    {
        items.clear();
    }
    void clearEnemies() //clear the enemies after they are defeated
    {
        enemies.clear();
    }



};

/*Player inherits its core stats from the GameCharacter class*/
class Player: public GameCharacter {
public:
    std::vector<Item> inventory;
    Room *curr_room;
    Room *prev_room;
    Player() :GameCharacter()
    {
     //   GameCharacter();

    }
    Player(std::string n, int hp, int atk, int def) :GameCharacter(n,hp,atk,def)
    {
        Item dagger = Item("Dagger",0,5,0);
        addItem(dagger);

    }
    void addItem(Item i) //adds the item to the players inventory 
    {
        inventory.push_back(i);
        increaseStats(i.health, i.attack, i.defence); //increase the players stats based on the stats of the item
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
    void changeRoom(Room* t) //move the players position through the dungeon
    {   //the print statements were for debugging purposes 
        //std::cout << "previous room is " << prev_room << std::endl;
        prev_room = curr_room;
        //std::cout << "previous room is " << prev_room << std::endl;
        //std::cout << "current room is " << curr_room << std::endl;
        
        curr_room = t;
       // std::cout << "current room is " << curr_room << std::endl;
    }
};
/*Contains all the logic for the Dungeon and how the player moves and interacts throughout it*/
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
        player.curr_room = &room_array[0]; //set the player to the first room
        player.prev_room = &room_array[0]; //Previous room is the same because it is the first room
        
        while (true) //the main game loop 
        {
            //std::cout << "current room is " << player.curr_room << std::endl;
            enterRoom(player.curr_room); //handles events on room entry
            if (player.isDead())
            {
                std::cout << "Game Over! Try again? " << std::endl;

                return endGame();
            }
            else
            {
                if (player.curr_room->isExit)
                {
                    if (player.curr_room->enemies.size() == 0)
                    {
                        std::cout << "You cleared the dungeon! Would you like to play again?" << std::endl;
                        return endGame();
                    }
                }
            }
           moveRoom(player.curr_room);
          // std::cout << "current room is " << player.curr_room << std::endl;
        }
    }
    void enterRoom(Room* r)
    {
        if (r->enemies.size() != 0) //if there is an enemy call the function to handle it
        {
          enemyRoom(r);
        }
        else if (r->items.size() != 0) //if there is an item call the function to handle it
        {
            lootRoom(r);
        }
        else
        {
            emptyRoom(r); //otherwise display movement options
        }
    }
    void emptyRoom(Room* er)
    {
        std::cout << "Entered an empty room.";
        std::string actions[] = { "1. Move to another room" };
        while (true)
        {
            printAction(1, actions);
            std::string input;
            std::cin >> input;
            if (input == "1")
            {
                return;
            }
            else
            {
                std::cout << "Invalid choice" << std::endl;
            }
        }

    }
    void lootRoom(Room* lr)
    {
        std::cout << "Entering the room, you see a chest in the corner." << std::endl;
        std::string actions[] = { "1. Loot the chest","2. Exit the room" };
        while (true)
        {
            printAction(2, actions);
            std::string input;
            std::cin >> input;
            if (input == "1")
            {
                handleLoot(lr);
                return;
            }
            else if (input == "2")
            {
                moveRoom(lr);
                return;
            }
            else
            {
                std::cout << "Invalid choice" << std::endl;
            }
        }

    }
    /*if there is an enemy in a room let user choose to fight or retreat*/
    void enemyRoom(Room *fr)
    {
        GameCharacter enemy = fr->enemies.front();
        std::cout << "A(n) " <<  enemy.ename     << " has appeared!" << std::endl;
       
        int size = fr->enemies.size();
        std::string actions[] = { "1. Fight " + enemy.ename,"2. Retreat" };
        while (true)
        {
            printAction(2, actions);
            std::string input;
            std::cin >> input;
            if (input == "1")
            {
                handleCombat(&enemy);
                return;
            }
            else if (input == "2")
            {
                player.changeRoom(player.prev_room);
                enterRoom(player.curr_room);
                return;
            }
            else
            {
                std::cout << "Invalid choice" << std::endl;
            }
        }
     
    }
    void handleLoot(Room* hl)
    {
        Item item = hl->items.front();
        int size = hl->items.size();
        player.lootRoom(hl);
        hl->getLoot();
        for (int i = 0; i < size; i++) //for loop based on the amount of items in the item vector for a room
        {
            std::cout << "You found a " << item.name << " in the chest" << std::endl;
            std::cout << "You equip the items, your stats have changed." << std::endl;
            std::cout << "Your health is now " << player.max_health << " and your attack is now " << player.attack << " and defence is " << player.defence << "." << std::endl;
         }
    }
    /*Handles combat encounters, causes player to deal and take damage*/
    void handleCombat(GameCharacter *enemy)
    {
        std::string actions[] = { "1. Attack","2. Retreat" };
        int damage;
        while (true)
        {
            printAction(2, actions);
            std::string input;
            std::cin >> input;
            if (input == "1")
            {
                damage = enemy->takeDamage(player.attack);
                std::cout << "You did " << damage << " damage to the " << enemy->ename << "." << std::endl;
                
            }
            else if (input == "2")
            {
                player.changeRoom(player.prev_room);
                enterRoom(player.curr_room);
                return;
            }
            else
            {
                std::cout << "Invalid choice" << std::endl;
            }
            if (enemy->isDead() == true)
            {
                std::cout << "You have defeated " << enemy->ename << "!" << std::endl;
                player.increaseStats(10, 5, 5);
                player.curr_room->clearEnemies();
                return;
            }
            if(enemy->isDead()==false)
            {
                damage = player.takeDamage(enemy->attack);
                player.curr_health -= damage;
                std::cout << "The " << enemy->ename << " attacked you for " << damage << " damage!" << std::endl;
            }
            if (player.isDead() == true)
            {
                std::cout << "You have been defeated." << std::endl;
                return;
            }

           


        }
    }
    /*Moves the player through the rooms, values are hardcoded for a 4 room dungeon */
    void moveRoom(Room* mr)
    {
        while (true)
        {
            if (mr->room_number == 0)
            {
                std::string action[] = { "1. Move Right","2. Move Down" };
                printAction(2, action);
                std::string input;
                std::cin >> input;
                if (input == "1")
                {
                    player.changeRoom(&room_array[1]);
                   // std::cout << "Moving to room 1 from 0" << std::endl;
                    return;

                }
                else if (input == "2")
                {
                   // std::cout << "Moving to room 2 from 0" << std::endl;
                    player.changeRoom(&room_array[2]);
                    return;
                }
            }
            else if (mr->room_number == 1)
            {
                std::string action[] = { "1. Move left" };
                printAction(1, action);
                std::string input;
                std::cin >> input;
                if (input == "1")
                {
                   // std::cout << "Moving to room 0 from 1" << std::endl;
                    player.changeRoom(&room_array[0]);
                    return;
                }
                
            }
            else if (mr->room_number == 2)
            {
                std::string action[] = { "1. Move Up", "2. Move Right" };
                printAction(2, action);
                std::string input;
                std::cin >> input;
                if (input == "1")
                {
                   // std::cout << "Moving to room 0 from 2" << std::endl;
                    player.changeRoom(&room_array[0]);
                    return;
                }
                else if (input == "2")
                {
                   // std::cout << "Moving to room 3 from 2" << std::endl;
                    player.changeRoom(&room_array[3]);
                    return;
                }
            }
            else if (mr->room_number == 3)
            {
                
                std::string action[] = { "1. Move left" };
                printAction(1, action);
                std::string input;
                std::cin >> input;
                if (input == "1")
                {
                    //std::cout << "Moving to room 2 from 3" << std::endl;
                    player.changeRoom(&room_array[2]);
                    return;
                }
                
            }
        }
    }
    /* Function to print the option strings for actions in the dungeon*/
    void printAction(int num_actions, std::string actions[])
    {
        std::cout << "Choose an action: " << std::endl;
        for (int i = 0; i < num_actions; i++)
        {
            std::cout << actions[i] << std::endl;
        }
    }
    /*When the dungeon is cleared or the player dies allow them to play again*/
    int endGame()
    {
        std::string action[] = {"1. Yes ", "2. No"};
        while (true)
        {
            printAction(2, action);
            std::string in;
            std::cin >> in;
            if (in == "1")
            {
                return 1;
            }
            else if (in == "2")
            {
                return 0;
            }
            else
            {
                std::cout << "Invalid choice." << std::endl;
            }
        }
    }













};
int main()
{
    std::vector<Item> no_item_vector = std::vector<Item>();
    std::vector<Item> item_vector = std::vector<Item>();
    std::vector<GameCharacter> no_enemies_vector = std::vector<GameCharacter>();
    std::vector<GameCharacter>  first_enemy_vector = std::vector<GameCharacter>();
    std::vector<GameCharacter> second_enemy_vector = std::vector<GameCharacter>();
    Player Player_Character = Player("Josh", 50, 5, 5);
    GameCharacter Ogre =  GameCharacter("Ogre", 25, 10, 7);
    GameCharacter spider = GameCharacter("Spider", 12, 14, 2);
    Item short_sword = Item("Shortsword", 0, 3, 1);
    item_vector.push_back(short_sword);
    Room first_room = Room(0, false,no_item_vector,no_enemies_vector);
    
    Room second_room = Room(1, false, item_vector, no_enemies_vector);
  
    first_enemy_vector.push_back(Ogre);
    Room third_room = Room(2, false, no_item_vector, first_enemy_vector);
    second_enemy_vector.push_back(spider);
    Room fourth_room = Room(3, true, no_item_vector, second_enemy_vector);

    Dungeon dungeon = Dungeon(Player_Character);
    dungeon.room_array.push_back(first_room);
    dungeon.room_array[0] = first_room;
    dungeon.room_array.push_back(second_room);
    dungeon.room_array[1] = second_room;
    dungeon.room_array.push_back(third_room);
    dungeon.room_array[2] = third_room;
    dungeon.room_array.push_back(fourth_room);
    dungeon.room_array[3] = fourth_room;
    while (true)
    {   
        int result = dungeon.runDungeon();
        if(result==0)
        {
            break;
        }
    }
   
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
