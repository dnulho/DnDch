#include <iostream>
#include <string>
#include <iomanip>
#include <array>
#include <algorithm>
#include <functional>
using namespace std;

///////////////////////////////
//       Declarations        //
///////////////////////////////
void banner (string name, string type = "\\", int size = 40);
class character{
public:
    character();                    // Constructor
    bool operator== (character);    // use when comparing init of objects
    bool operator< (character);
    bool operator> (character);
    string getname ();              // return name of object
    int getinit ();                 // return init of object
    void set_init (int a);          // set init of object to 'a'
    void heal_full();               // max out hp and 0 nl dam
    void heal(int amt);             // modify hp by given amount
    void heal_nl (int amt);         // heal nl dam by given amount
    void dam_nl (int amt);          // give nl dam by given amount, checks for disabled or unconscious state.
    int getHP();                    // return current hp
    int getnlDam();                 // return nl dam
    void SetnMob (character* nmob); // Sets next mob
    character* GetnMob ();          // Gives the address of next mob.
    int getCoords();                // Gives xy coords

private:
    string name;            // name of character
    int mHP;                // max hit points
    int HP;                 // current hit points
    int nlDam;              // amount of non lethal damage object currently has
    int init;               // initiative of character
    int x_cor, y_cor;       // x and y co-ordinates on map
    character* nextMob;     // points to next mob to iterate to
};
class encounter{
public:
    encounter (character*, int);        // Constructor
    void listFull ();
private:
    int ENEMIES;                        // number of enemies in this encounter
    character* enemy[];                  // array of enemy mobs
    character* holding[];               // array of pointers to all mobs currently holding turn
    character* player;                  // pointer to array of player mobs
    character* firstMob;                // pointer to highest init mob
    void setInits (character ary[], int SIZE);
    character* mergeSortLL (character p[], int pSIZE, character e[], int eSIZE);
    character* sortIndArray (character ary[]);
};
void rebuildCharArray (character arry[], int size){
    delete[] arry;
    arry = new character[size];
}
///////////////////////////////
//        Main Function      //
///////////////////////////////
int main(){
    int input = 1;
    string select;
    int PLAYERS;
    banner ("COMBAT HELPER", "=");
    cout << "Welcome to D&D Combat Helper.\n"
         << "Lets begin by adding all players.\n"
         << "How many players are there?";
    cin  >> PLAYERS;
    character* player = new character[PLAYERS];


//////////////////////////////////
//          MAIN MENU           //
//////////////////////////////////

while (input != 0)
{
    banner ("MAIN MENU");

    cout << "\nPlease select from the following options:\n"
    << "1. (Re)build all players\n"
    << "2. Start a new encounter\n"
    << "3. Reset all player's health\n"
    << "0. Quit\n";
    cin >> input;
    switch (input)
  {
    case 1: //New players
    {
    break;
    };
    case 2: //new fight
    {
        encounter crnt_enco (player, PLAYERS);
        break;
    };
    case 3: //Heal players
    {
        cout << "All players restored to full health";
        break;
    };
    case 0: // quit
    {
        cout << "Thank you for using D&D Combat Helper.\nHave a nice day.";
        break;
    };
    default: // incorrect input
    {
        cout << "Invalid selection, please try again.";
        break;
    };
  }; // Switch end
}; //While end

}
///////////////////////////////
// Misc Function Definitions //
///////////////////////////////
void banner (string name, string type, int size){
    cout << endl;
    for (int i=0; i < size; i++)    { cout << type; };

    cout << "\n" << type << type
    << setw (((size-4)/2) + ((name.size())/2) /*- (name.size()%2)*/ ) << name
    << setw (((size-4)/2) - ((name.size())/2) + (name.size()%2))
    << type << type << "\n";

    for (int i=0; i < size; i++)    { cout << type; };
    cout << endl;
};
///////////////////////////////
//      character class      //
///////////////////////////////
character::character() {
    string select;
    cout << "\nWhat is new character's name?";
    cin >> name;
    cout << "\nWhat is their max HP?";
    cin >> mHP;
    cout << "Are they currently injured? (y/n)";
    cin >> select;
    if (select == "y"){
        cout << "What is their current HP?";
        cin >> HP;
        cout << "How much non-lethal damage do they currently have?";
        cin >> nlDam;
        }
    else{
        HP = mHP;
        nlDam = 0;
    };
    init = 0;
    nextMob = NULL;
};
bool character::operator== (character b) {return init == b.init;}
bool character::operator> (character b) {return init > b.init;}
bool character::operator< (character b) {return init < b.init;}
string character::getname () { return name; }
int character::getinit () { return init; }
void character::set_init (int a) { init = a; }
void character::heal_full() {
    HP = mHP;
    nlDam = 0;
};
void character::heal(int amt) { HP += amt; }
void character::heal_nl (int amt) {
    if (nlDam < 0)
    {
        nlDam -= (amt - nlDam);
        cout << name << "now has "<< nlDam << " non lethal damage" << endl;
    }
    else if (nlDam >= 0)
        { cout << name << " has no non lethal damage. No action required" << endl;};

}
void character::dam_nl (int amt) {
     nlDam += amt;
     if ( nlDam == HP )
     {
         cout << name << " is now Staggered, and can only take one standard action.";
     }
     else if (nlDam > HP)
     {
         cout << name << " is now unconscious.";
     }
}
int character::getHP() { return HP; }
int character::getnlDam() { return nlDam; }
void character::SetnMob(character* nmob) { nextMob = nmob;}

///////////////////////////////
//      encounter class      //
///////////////////////////////
encounter::encounter (character players_in[], int PLAYERS ){
    banner ("NEW ENCOUNTER", "*");
    player = players_in;
    cout<< "How many enemies?";
    cin >> ENEMIES;
    enemy = new character[ENEMIES];
    cout<< "Roll for initiative and input for each person or enemy:"
        << "\n(Don't worry about ties, they will be resolved shortly)";
    setInits(player, PLAYERS);       // get players init
    setInits(enemy, ENEMIES);        // get enemies init
    firstMob = mergeSortLL (player, enemy);
}
void encounter::setInits (character ary[], int SIZE) {
    for ( int i = 0; i < SIZE ; i++) {
        int in;
        cout << "What is " << ary[i].getname() << "'s initiative?: ";
        cin >> in;
        ary[i].set_init(in);
    }
}
character* encounter::mergeSortLL (character p[], int pSIZE, character e[], int eSIZE) {
    character* hS = NULL;
    int pU = 0;
    int eU = 0;
    while (pU < pSIZE && eU < eSIZE) {
        if (p[pU].getinit() > e[eU].getinit()) {
            if (p[pU].getinit() > hS.getinit()) {
                hS.GetnMob(p[pU]);
                hS = p[pU];
                pU++;
                continue;
            }
            else if (p[pU].getinit() < hS.getinit()) {

            }
        }
    }
}
character* encounter::sortIndArray (character ary[]) {
    character* high = ary[0];
    for (int i = 0, j = 0; i < ary; i++, j = 0) {
        while ( ary[i].getinit() < ary[j].getinit()) {
            j++;
        };

    }
}
/*void encounter::listFull () {
    banner ("All Characters", ":");
    for (unsigned int i = 0; i < allpeople.size (); i++) {
        cout<< allpeople[i].getname () << " - HP: " << allpeople[i].getHP()
            << " - Initiative: " << allpeople[i].getinit ();
    }

}*/

