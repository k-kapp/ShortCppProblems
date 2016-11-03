#include <iostream>
#include <vector>
using namespace std;

class Spell { 
    private:
        string scrollName;
    public:
        Spell(): scrollName("") { }
        Spell(string name): scrollName(name) { }
        virtual ~Spell() { }
        string revealScrollName() {
            return scrollName;
        }
};

class Fireball : public Spell { 
    private: int power;
    public:
        Fireball(int power): power(power) { }
        void revealFirepower(){
            cout << "Fireball: " << power << endl;
        }
};

class Frostbite : public Spell {
    private: int power;
    public:
        Frostbite(int power): power(power) { }
        void revealFrostpower(){
            cout << "Frostbite: " << power << endl;
        }
};

class Thunderstorm : public Spell { 
    private: int power;
    public:
        Thunderstorm(int power): power(power) { }
        void revealThunderpower(){
            cout << "Thunderstorm: " << power << endl;
        }
};

class Waterbolt : public Spell { 
    private: int power;
    public:
        Waterbolt(int power): power(power) { }
        void revealWaterpower(){
            cout << "Waterbolt: " << power << endl;
        }
};

class SpellJournal {
    public:
        static string journal;
        static string read() {
            return journal;
        }
}; 
string SpellJournal::journal = "";

void counterspell(Spell *spell) {
 /* Enter your code here */
    Fireball * fire_ptr;
    Frostbite * frost_ptr;
    Thunderstorm * thunder_ptr;
    Waterbolt * water_ptr;
    if (fire_ptr = dynamic_cast<Fireball *>(spell))
    {
        fire_ptr->revealFirepower();
    }
    else if (frost_ptr = dynamic_cast<Frostbite *>(spell))
    {
        frost_ptr->revealFrostpower();
    }
    else if (thunder_ptr = dynamic_cast<Thunderstorm *>(spell))
    {
        thunder_ptr->revealThunderpower();
    }
    else if (water_ptr = dynamic_cast<Waterbolt *>(spell))
    {
        water_ptr->revealWaterpower();
    }
    else
    {
        string scroll_name = spell->revealScrollName();
        vector<int> * prev_seqs = new vector<int>(SpellJournal::journal.size(), 0);
        vector<int> * curr_seqs = new vector<int>(SpellJournal::journal.size(), 0);
        for (auto chr_iter = scroll_name.begin(); chr_iter != scroll_name.end(); advance(chr_iter, 1))
        {
            for (int i = 0; i < curr_seqs->size(); i++)
            {
                if (i > 0)
                {
                        curr_seqs->at(i) = prev_seqs->at(i) > curr_seqs->at(i - 1) ? prev_seqs->at(i) : curr_seqs->at(i - 1);
                    if (*chr_iter == SpellJournal::journal.at(i))
                    {
                        curr_seqs->at(i) = prev_seqs->at(i - 1) + 1;
                    }
                }
                else
                {
                    curr_seqs->at(i) = prev_seqs->at(i);
                    if (*chr_iter == SpellJournal::journal.at(i))
                    {
                        curr_seqs->at(i) = 1;
                    }
                }
            }
            auto temp = prev_seqs;
            prev_seqs = curr_seqs;
            curr_seqs = temp;
        }
        cout << prev_seqs->back() << endl;
        
        delete prev_seqs;
        delete curr_seqs;
    }
}

class Wizard {
    public:
        Spell *cast() {
            Spell *spell;
            string s; cin >> s;
            int power; cin >> power;
            if(s == "fire") {
                spell = new Fireball(power);
            }
            else if(s == "frost") {
                spell = new Frostbite(power);
            }
            else if(s == "water") {
                spell = new Waterbolt(power);
            }
            else if(s == "thunder") {
                spell = new Thunderstorm(power);
            } 
            else {
                spell = new Spell(s);
                cin >> SpellJournal::journal;
            }
            return spell;
        }
};

int main() {
    int T;
    cin >> T;
    Wizard Arawn;
    while(T--) {
        Spell *spell = Arawn.cast();
        counterspell(spell);
    }
    return 0;
}

