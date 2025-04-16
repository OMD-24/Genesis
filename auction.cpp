#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <random>
#include <ctime>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;


// PlayerImageWindow: Shows player image using SFML in a separate thread

class PlayerImageWindow {
public:
    PlayerImageWindow() : window(nullptr), shouldClose(false) {
        // Start the render thread
        renderThread = std::thread(&PlayerImageWindow::renderLoop, this);
        // Let the thread start up
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    ~PlayerImageWindow() {
        // Signal to close the window and join the thread
        shouldClose = true;
        if(window) {
            window->close();
        }
        if(renderThread.joinable())
            renderThread.join();
    }

    // Loads the image for the given player name and updates the texture.
    void showPlayerImage(const string &playerName) {
        // Build file path by replacing spaces with underscores
        string formattedName = playerName;
        replace(formattedName.begin(), formattedName.end(), ' ', '_');
        string imagePath = "images/" + formattedName + ".jpg"; // e.g., images/Virat_Kohli.jpg

        // Lock the mutex while updating texture and sprite
        std::lock_guard<std::mutex> lock(mtx);

        if (!texture.loadFromFile(imagePath)) {
            // If image could not be loaded, display a default "not found" message.
            cout << "[Image Display] Image not found for " << playerName << "\n";
            // Create a red placeholder image
            sf::Image img;
            img.create(300, 350, sf::Color::Red);
            texture.loadFromImage(img);
        } 
        sprite.setTexture(texture);
        // Scale the sprite to approximately 300x350 if needed.
        float scaleX = 300.f / texture.getSize().x;
        float scaleY = 350.f / texture.getSize().y;
        sprite.setScale(scaleX, scaleY);
    }

private:
    sf::RenderWindow *window;
    sf::Texture texture;
    sf::Sprite sprite;
    std::thread renderThread;
    std::mutex mtx;
    bool shouldClose;

    // This function runs in a separate thread and handles the SFML event loop
    void renderLoop() {
        window = new sf::RenderWindow(sf::VideoMode(350, 400), "Player Image");
        window->setFramerateLimit(30);
        while (window->isOpen() && !shouldClose) {
            sf::Event event;
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window->close();
            }
            window->clear(sf::Color::Black);
            // Lock to safely read the current sprite
            {
                std::lock_guard<std::mutex> lock(mtx);
                window->draw(sprite);
            }
            window->display();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        delete window;
        window = nullptr;
    }
};

// ******************************************************************
// Player Class
// ******************************************************************
class Player {
public:
    string name;
    int basePrice;
    int age;
    string role;
    int matches;
    int runs;
    double strikeRate;
    int wickets;
    double bowlingEconomy;
    int catches;
    double points;

    Player(string name, int basePrice, int age, string role, int matches, int runs,
           double strikeRate, int wickets, double bowlingEconomy, int catches, double points)
           : name(name), basePrice(basePrice), age(age), role(role), matches(matches),
             runs(runs), strikeRate(strikeRate), wickets(wickets),
             bowlingEconomy(bowlingEconomy), catches(catches), points(points) {}

    // Print the player's stats
    string toString() const {
        stringstream ss;
        ss << "Name: " << name
           << ", Base Price: " << basePrice
           << ", Age: " << age
           << ", Role: " << role
           << ", Matches: " << matches
           << ", Runs: " << runs
           << ", Strike Rate: " << strikeRate
           << ", Wickets: " << wickets
           << ", Bowling Economy: " << bowlingEconomy
           << ", Catches: " << catches;
        return ss.str();
    }
};


// Players Class

class Players {
public:
    vector<Player> playerList;

    Players() {
        // Adding players with stats
        playerList.push_back(Player("Virat Kohli", 200, 36, "Batsman", 258, 8252, 132.5, 4, 8.8, 215, 9.9));
        playerList.push_back(Player("MS Dhoni", 180, 43, "Wicket-Keeper", 271, 5373, 137.99, 0, 0.0, 315, 9.8));
        playerList.push_back(Player("Rohit Sharma", 180, 37, "Batsman", 262, 6684, 131.19, 15, 8.02, 197, 9.8));
        playerList.push_back(Player("Jasprit Bumrah", 150, 31, "Bowler", 135, 68, 86.08, 166, 7.33, 80, 9.6));
        playerList.push_back(Player("KL Rahul", 140, 32, "Batsman/Wicket-Keeper", 136, 4883, 135.61, 0, 0.0, 90, 9.0));
        playerList.push_back(Player("Suryakumar Yadav", 150, 34, "Batsman", 156, 3833, 115.58, 1, 0.0, 50, 9.2));
        playerList.push_back(Player("Hardik Pandya", 150, 31, "All-Rounder", 142, 2608, 145.0, 74, 9.0, 45, 9.0));
        playerList.push_back(Player("Shubman Gill", 100, 25, "Batsman", 109, 3424, 128.4, 0, 0.0, 25, 8.5));
        playerList.push_back(Player("Rishabh Pant", 110, 27, "Wicket-Keeper", 118, 3387, 145.0, 2, 0.0, 30, 8.8));
        playerList.push_back(Player("Bhuvneshwar Kumar", 160, 35, "Bowler", 181, 300, 120.0, 187, 6.5, 40, 8.7));
        playerList.push_back(Player("Ravindra Jadeja", 190, 36, "All-Rounder", 247, 5002, 130.5, 164, 6.7, 55, 9.3));
        playerList.push_back(Player("Yuzvendra Chahal", 130, 34, "Bowler", 165, 200, 100.0, 207, 7.5, 30, 8.5));
        playerList.push_back(Player("Kuldeep Yadav", 110, 30, "Bowler", 89, 150, 110.0, 97, 7.2, 25, 8.2));
        playerList.push_back(Player("Sanju Samson", 130, 30, "Wicket-Keeper", 120, 4617, 135.5, 1, 0.0, 35, 8.6));
        playerList.push_back(Player("Abhishek Sharma", 60, 24, "All-Rounder", 69, 1568, 160.0, 20, 7.1, 20, 8));
        playerList.push_back(Player("Axar Patel", 100, 31, "All-Rounder", 155, 1720, 125.0, 80, 6.4, 28, 8.4));
        playerList.push_back(Player("Shreyas Iyer", 120, 30, "Batsman", 120, 3377, 132.0, 0, 0.0, 30, 8.6));
        playerList.push_back(Player("Mohammed Shami", 140, 34, "Bowler", 116, 100, 120.0, 132, 7.0, 35, 8.5));
        playerList.push_back(Player("Mohammed Siraj", 90, 31, "Bowler", 99, 109, 125.0, 99, 7.3, 20, 8.3));
        playerList.push_back(Player("Ishan Kishan", 100, 26, "Wicket-Keeper", 111, 2780, 145.0, 1, 0.0, 25, 8.5));

        // Shuffle the players list using C++11 random facilities
        auto rng = default_random_engine(static_cast<unsigned>(time(nullptr)));
        shuffle(playerList.begin(), playerList.end(), rng);
    }

    // Display all players with stats
    void displayPlayers() {
        for (const auto &p : playerList) {
            cout << p.toString() << "\n";
        }
    }
};


// Teams Class: Manages team purse and squad information

class Teams {
private:
    // Four teams (indices 0 to 3)
    int actual_Purse[4];
public:
    vector<string> MI;
    vector<string> CSK;
    vector<string> RCB;
    vector<string> KKR;
    vector<string> UnSold;

    Teams() {
        for (int i = 0; i < 4; i++) {
            actual_Purse[i] = 0;
        }
    }

    int* getPurse() {
        return actual_Purse;
    }

    void setPurse(int i, int p) {
        if (i >= 0 && i < 4) {
            actual_Purse[i] = p;
        }
    }

    // Set team squad information.
    // i = 1: MI, 2: CSK, 3: RCB, 4: KKR, 0: Unsold Players.
    void TeamSquad(int i, const string &name, int price) {
        string entry = name + " for Rs. " + to_string(price);
        switch(i) {
            case 1: MI.push_back(entry); break;
            case 2: CSK.push_back(entry); break;
            case 3: RCB.push_back(entry); break;
            case 4: KKR.push_back(entry); break;
            case 0: UnSold.push_back(entry); break;
            default: break;
        }
    }

    // Print the squad for a given team indicator:
    // 1: MI, 2: CSK, 3: RCB, 4: KKR, 0: Unsold Players.
    void printSquad(int j) {
        switch (j) {
            case 1:
                cout << "MI Squad: ";
                for (const auto &s : MI) cout << s << ", ";
                cout << "\n";
                break;
            case 2:
                cout << "CSK Squad: ";
                for (const auto &s : CSK) cout << s << ", ";
                cout << "\n";
                break;
            case 3:
                cout << "RCB Squad: ";
                for (const auto &s : RCB) cout << s << ", ";
                cout << "\n";
                break;
            case 4:
                cout << "KKR Squad: ";
                for (const auto &s : KKR) cout << s << ", ";
                cout << "\n";
                break;
            case 0:
                cout << "Unsold Players: ";
                for (const auto &s : UnSold) cout << s << ", ";
                cout << "\n";
                break;
            default:
                cout << "Invalid option!\n";
        }
    }
};


// Calculate Class: Keeps track of team points

class Calculate {
public:
    vector<double> MIpoints;
    vector<double> CSKpoints;
    vector<double> RCBpoints;
    vector<double> KKRpoints;

    void TeamPoints(int j, double points) {
        switch (j) {
            case 1: MIpoints.push_back(points); break;
            case 2: CSKpoints.push_back(points); break;
            case 3: RCBpoints.push_back(points); break;
            case 4: KKRpoints.push_back(points); break;
            default: break;
        }
    }

    double getTotalPoints(const vector<double>& teamPoints) {
        double total = 0;
        for (double point : teamPoints) {
            total += point;
        }
        return total;
    }
};


// Auction Class: Handles bidding and auction logic

class Auction {
public:
    // Use a single static instance for the player image window.
    static PlayerImageWindow imageWindow;

    static void bidding(Player &player, Teams &t, Calculate &cal) {
        // Update the image in the SFML window.
        imageWindow.showPlayerImage(player.name);

        int currPrice = player.basePrice;
        int bid;
        int index = -1;
        int budget[4] = {0, 0, 0, 0};

        string winningTeam = "Unsold";

        cout << "\nNow bidding for '" << player.name << "'"
             << "\n Base Price : '" << (player.basePrice + 100) << "'"
             << "\n Age : " << player.age
             << "\n Role : " << player.role
             << "\n Matches : " << player.matches
             << "\n Runs : " << player.runs
             << "\n Strike Rate : " << player.strikeRate
             << "\n Wickets : " << player.wickets
             << "\n Bowling Economy : " << player.bowlingEconomy
             << "\n Catches : " << player.catches << "\n";

        while (true) {
            cout << "\nEnter a team number to bid (1: MI, 2: CSK, 3: RCB, 4: KKR, 0: Stop bidding, 5: View Squads): ";
            cin >> bid;

            if (bid == 0) {
                cout << player.name << " is sold to " << winningTeam << " for Rs. " << currPrice << "\n";
                if (index != -1) {
                    int *purse = t.getPurse();
                    purse[index] = budget[index];
                    cal.TeamPoints(index + 1, player.points);
                }
                // Update team squad information.
                t.TeamSquad(index + 1, player.name, currPrice);

                int *printMoney = t.getPurse();
                cout << "MI : "   << printMoney[0] << "\n"
                     << "CSK : "  << printMoney[1] << "\n"
                     << "RCB : "  << printMoney[2] << "\n"
                     << "KKR : "  << printMoney[3] << "\n";

                break;
            }

            if (bid == 5) {
                int j;
                cout << "\nWhich team squad do you want to see? (1: MI, 2: CSK, 3: RCB, 4: KKR, 0: Unsold Players): ";
                cin >> j;
                t.printSquad(j);
                continue;
            }

            int teamIndex = bid - 1;
            int *actual_Purse = t.getPurse();

            if (teamIndex == index) {
                cout << "Invalid Bid! Same team cannot bid twice in a row.\n";
                continue;
            }

            if (bid >= 1 && bid <= 4) {
                if (actual_Purse[teamIndex] >= currPrice + 100) {
                    currPrice += 100;
                    budget[teamIndex] = actual_Purse[teamIndex] - currPrice;
                    switch (bid) {
                        case 1: winningTeam = "MI"; break;
                        case 2: winningTeam = "CSK"; break;
                        case 3: winningTeam = "RCB"; break;
                        case 4: winningTeam = "KKR"; break;
                        default: winningTeam = "Unknown"; break;
                    }
                    cout << "The team '" << winningTeam << "' bid for " << player.name <<
                        " at Rs. " << currPrice << "\n(If they buy it, they will have Remaining Purse: Rs. " << budget[teamIndex] << ")\n";
                } else {
                    cout << "Team does not have enough purse left!\n";
                }
            } else {
                cout << "Invalid bid! Enter a number between 1-4, or 0 to stop.\n";
            }

            index = teamIndex;
        }
    }
};

// Define the static member for Auction (one instance of the image window)
PlayerImageWindow Auction::imageWindow;


// Main Function

int main() {
    Teams t;
    Players p;
    Calculate cal;

    int initialPurse;
    cout << "Enter an initial purse amount for each team: ";
    cin >> initialPurse;

    for (int i = 0; i < 4; i++) {
        t.setPurse(i, initialPurse);
    }

    int *purseArray = t.getPurse();
    cout << "Initial purse: [ " << purseArray[0] << ", " << purseArray[1] << ", "
         << purseArray[2] << ", " << purseArray[3] << " ]\n";

    // Run auction for up to 20 players (or fewer if less available)
    int nPlayers = p.playerList.size();
    int auctionCount = (nPlayers < 20) ? nPlayers : 20;
    for (int i = 0; i < auctionCount; i++) {
        Auction::bidding(p.playerList[i], t, cal);
    }

    cout << "\nTeam Squads: \n";
    t.printSquad(1);
    t.printSquad(2);
    t.printSquad(3);
    t.printSquad(4);
    t.printSquad(0);

    cout << "\nDo you want to know the analysis of each Team,\nwhich means which Team performed well in this Auction?\n"
         << "Type '8' for YES or '9' for NO: ";
    int input;
    cin >> input;
    if (input == 8) {
        cout << "MI -> "   << cal.getTotalPoints(cal.MIpoints)  << "\n"
             << "CSK -> "  << cal.getTotalPoints(cal.CSKpoints) << "\n"
             << "RCB -> "  << cal.getTotalPoints(cal.RCBpoints) << "\n"
             << "KKR -> "  << cal.getTotalPoints(cal.KKRpoints) << "\n";
    }

    // Give some time before ending so that the SFML window remains viewable.
    cout << "\nPress ENTER to exit the program.";
    cin.ignore();
    cin.get();

    return 0;
}
