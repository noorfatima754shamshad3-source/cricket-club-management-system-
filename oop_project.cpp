//libraries
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <map>

using namespace std;

//  Only 4 Colors code
const string R = "\033[31m";  
const string G = "\033[32m";  
const string Y = "\033[33m";  
const string B = "\033[34m";  
const string X = "\033[0m";   

// AUTHENTICATION SYSTEM-
enum UserRole { ADMIN, COACH, PLAYER, NONE };
struct User {
    string username;
    string password;
    UserRole role;
    string linkedID;
    User() : role(NONE) {}
    User(string u, string p, UserRole r, string id = "") 
        : username(u), password(p), role(r), linkedID(id) {}
};

class AuthSystem { //signin/up menu display and functionality
private: 
    map<string, User> users;
    User currentUser;
public:
    AuthSystem() {
        users["admin"] = User("admin", "admin123", ADMIN);
        users["coach1"] = User("coach1", "coach123", COACH);
    }
    
    bool signUp(string username, string password, UserRole role, string linkedID = "") {
        if(users.find(username) != users.end()) {
            cout << R << " Username already exists!" << X << "\n";
            return false;
        }
        if(username.empty() || password.empty()) {
            cout << R << " Username and password cannot be empty!" << X << "\n";
            return false;
        }
        users[username] = User(username, password, role, linkedID);
        cout << G << " Account created! Please sign in." << X << "\n";
        return true;
    }
    
    bool signIn(string username, string password) {
        auto it = users.find(username);
        if(it == users.end()) {
            cout << R << " Username not found!" << X << "\n";
            return false;
        }
        if(it->second.password != password) {
            cout << R << " Incorrect password!" << X << "\n";
            return false;
        }
        currentUser = it->second;
        cout << G << " Welcome, " << username << "!" << X << "\n";
        return true;
    }
    
    void signOut() {
        currentUser = User();
        cout << G << " Signed out successfully." << X << "\n";
    }
    //const function
    UserRole getCurrentRole() const { return currentUser.role; }
    string getCurrentUsername() const { return currentUser.username; }
    bool isLoggedIn() const { return currentUser.role != NONE; }

    //menu
    void displayCurrentUser() const { 
        cout << "\n" << Y << "+========== USER INFO ==========+" << X << "\n";
        cout << Y << "|" << X << " User: " << left << setw(26) << currentUser.username << Y << "|" << X << "\n";
        cout << Y << "|" << X << " Role: " << left << setw(26) 
             << (currentUser.role == ADMIN ? "Administrator" : 
                 currentUser.role == COACH ? "Coach" : "Player") << Y << "|" << X << "\n";
        cout << Y << "+===============================+" << X << "\n";
    }
};

// Forward declarations
class Player;
class Team;
class Match;

//  UTILITY CLASS 
class IDGenerator {
private:
    static int playerCounter;
    static int teamCounter;
    static int matchCounter;
    
public:
    static string generatePlayerID() {
        return "PLR" + to_string(++playerCounter);
    }
    static string generateTeamID() {
        return "TM" + to_string(++teamCounter);
    }
    static string generateMatchID() {
        return "MCH" + to_string(++matchCounter);
    }
};

int IDGenerator::playerCounter = 1000;
int IDGenerator::teamCounter = 100;
int IDGenerator::matchCounter = 1;

//  DATE CLASS 
class Date {  
private:
    int day, month, year;
public:
    Date() : day(1), month(1), year(2024) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    void display() const {
        cout << day << "/" << month << "/" << year;
    }
};

// = PLAYER STATS 
class PlayerStats {
private:
    int* scores;
    int numScores;
    int totalRuns;
    int wickets;
public:
    PlayerStats() : scores(nullptr), numScores(0), totalRuns(0), wickets(0) {}
    PlayerStats(int n) : numScores(n), totalRuns(0), wickets(0) {
        scores = new int[n];
        for(int i = 0; i < n; i++) scores[i] = 0;
    }
    ~PlayerStats() { delete[] scores; }
    
    void addScore(int index, int runs) {
        if(index >= 0 && index < numScores) {
            scores[index] = runs;
            totalRuns += runs;
        }
    }
    void addWickets(int w) { wickets += w; }
    void display() const {
        cout << "Runs: " << totalRuns << ", Wickets: " << wickets;
    }
    int getTotalRuns() const { return totalRuns; }
    int getWickets() const { return wickets; }
};

// PLAYER CLASS
class Player {
private:
    string playerID;
    string name;
    int age;
    string role;
    string country;
    Team* currentTeam;
    Date birthDate;
    PlayerStats* stats;
    const int jerseyNumber;
    static int totalPlayers;

public:
    Player() : jerseyNumber(0), currentTeam(nullptr), stats(nullptr) {
        playerID = IDGenerator::generatePlayerID();
        name = "Unknown";
        age = 0;
        role = "TBD";
        country = "TBD";
        totalPlayers++;
    }
    
    Player(string n, int a, string r, string c, int jn) 
        : name(n), age(a), role(r), country(c), jerseyNumber(jn), 
          currentTeam(nullptr), stats(new PlayerStats(10)) {
        playerID = IDGenerator::generatePlayerID();
        totalPlayers++;
    }
    
    ~Player() {
        delete stats;
        totalPlayers--;
    }
    
    string getPlayerID() const { return playerID; }
    string getName() const { return name; }
    string getRole() const { return role; }
    string getCountry() const { return country; }
    int getJerseyNumber() const { return jerseyNumber; }
    Team* getTeam() const { return currentTeam; }
    static int getTotalPlayers() { return totalPlayers; }
    
    void setName(string n) { name = n; }
    void setAge(int a) { age = a; }
    void setTeam(Team* t) { currentTeam = t; }
    
    void updateStats(int runs, int wickets) {
        if(stats) {
            stats->addScore(0, runs);
            stats->addWickets(wickets);
        }
    }
    
    void display() const {
        cout << "\n" << Y << "+========== PLAYER ==========+" << X << "\n";
        cout << Y << "|" << X << " ID: " << left << setw(25) << playerID << Y << "|" << X << "\n";
        cout << Y << "|" << X << " Name: " << left << setw(23) << name << Y << "|" << X << "\n";
        cout << Y << "|" << X << " Role: " << left << setw(23) << role << Y << "|" << X << "\n";
        cout << Y << "|" << X << " Country: " << left << setw(20) << country << Y << "|" << X << "\n";
        cout << Y << "|" << X << " Jersey: " << left << setw(21) << jerseyNumber << Y << "|" << X << "\n";
        if(stats) { cout << Y << "|" << X << " "; stats->display(); cout << Y << "|" << X << "\n"; }
        cout << Y << "+============================+" << X << "\n";
    }
};

int Player::totalPlayers = 0;

//  TEAM CLASS 
class Team {
private:
    string teamID;
    string teamName;
    string country;
    Player** players;
    int maxPlayers;
    int currentPlayers;
    static const int MAX_TEAM_SIZE = 11;

public:
    Team(string name, string ctry) : teamName(name), country(ctry), currentPlayers(0) {
        teamID = IDGenerator::generateTeamID();
        maxPlayers = MAX_TEAM_SIZE;
        players = new Player*[maxPlayers];
        for(int i = 0; i < maxPlayers; i++) players[i] = nullptr;
    }
    
    ~Team() {
        for(int i = 0; i < maxPlayers; i++) delete players[i];
        delete[] players;
    }
    
    bool addPlayer(Player* p) {
        if(currentPlayers >= maxPlayers || p == nullptr) return false;
        players[currentPlayers] = p;
        p->setTeam(this);
        currentPlayers++;
        return true;
    }
    
    void display() const {
        cout << "\n" << Y << "+========== TEAM ==========+" << X << "\n";
        cout << Y << "|" << X << " ID: " << left << setw(23) << teamID << Y << "|" << X << "\n";
        cout << Y << "|" << X << " Name: " << left << setw(21) << teamName << Y << "|" << X << "\n";
        cout << Y << "|" << X << " Players: " << left << setw(18) << currentPlayers << "/" << maxPlayers << Y << "|" << X << "\n";
        cout << Y << "+==========================" << X << "+" << "\n";
        for(int i = 0; i < currentPlayers; i++) {
            if(players[i]) {
                cout << Y << "|" << X << " " << (i+1) << ". " << left << setw(23) 
                     << players[i]->getName() << Y << "|" << X << "\n";
            }
        }
    }
    
    string getTeamID() const { return teamID; }
    string getTeamName() const { return teamName; }
};

//  MATCH CLASS 
class Match {
private:
    string matchID;
    Team* teamA;
    Team* teamB;
    Date matchDate;
    int scoreA, scoreB;
    string winner;
    bool played;

public:
    Match(Team* a, Team* b, Date d) : teamA(a), teamB(b), matchDate(d), 
        scoreA(0), scoreB(0), winner("TBD"), played(false) {
        matchID = IDGenerator::generateMatchID();
    }
    
    void play() {
        if(!teamA || !teamB) return;
        srand(time(0));
        scoreA = rand() % 150 + 100;
        scoreB = rand() % 150 + 100;
        winner = (scoreA > scoreB) ? teamA->getTeamName() : 
                 (scoreB > scoreA) ? teamB->getTeamName() : "Draw";
        played = true;
    }

    void display() const {
        cout << "\n" << Y << "+========== MATCH ==========+" << X << "\n";
        cout << Y << "|" << X << " ID: " << left << setw(24) << matchID << Y << "|" << X << "\n";
        if(teamA && teamB) {
            cout << Y << "|" << X << " " << left << setw(12) << teamA->getTeamName() << ": " << scoreA << Y << "|" << X << "\n";
            cout << Y << "|" << X << " " << left << setw(12) << teamB->getTeamName() << ": " << scoreB << Y << "|" << X << "\n";
        }
        cout << Y << "|" << X << " Winner: " << left << setw(19) << winner << Y << "|" << X << "\n";
        cout << Y << "+===========================" << X << "+" << "\n";
    }
};

// CRICKET SYSTEM 
class CricketSystem {
private:
    Player** allPlayers;
    Team** allTeams;
    Match** allMatches;
    int playerCapacity, teamCapacity, matchCapacity;
    int playerCount, teamCount, matchCount;
    AuthSystem& auth;

public:
    CricketSystem(AuthSystem& a) : auth(a), playerCapacity(100), teamCapacity(20), 
        matchCapacity(50), playerCount(0), teamCount(0), matchCount(0) {
        
        allPlayers = new Player*[playerCapacity];
        allTeams = new Team*[teamCapacity];
        allMatches = new Match*[matchCapacity];
        
        for(int i = 0; i < playerCapacity; i++) allPlayers[i] = nullptr;
        for(int i = 0; i < teamCapacity; i++) allTeams[i] = nullptr;
        for(int i = 0; i < matchCapacity; i++) allMatches[i] = nullptr;
    }
    
    ~CricketSystem() {
        for(int i = 0; i < playerCount; i++) delete allPlayers[i];
        for(int i = 0; i < teamCount; i++) delete allTeams[i];
        for(int i = 0; i < matchCount; i++) delete allMatches[i];
        delete[] allPlayers;
        delete[] allTeams;
        delete[] allMatches;
    }
    
    int displayMenuOnce() {
        UserRole role = auth.getCurrentRole();
        int maxChoice = 0;

        cout << "\n" << Y << "+========== CRICKET SYSTEM ==========+" << X << "\n";
        
        if(role == ADMIN) {
            cout << Y << "|" << X << " 1. Create Player                   " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 2. Create Team                     " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 3. Add Player to Team              " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 4. Schedule Match                  " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 5. Play Match                      " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 6. View All Players                " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 7. View All Teams                  " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 8. View All Matches                " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 0. Logout                          " << Y << "|" << X << "\n";
            maxChoice = 8;
        }
        else if(role == COACH) {
            cout << Y << "|" << X << " 1. View All Players                " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 2. View All Teams                  " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 3. View Team Details               " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 4. Update Player Stats             " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 5. View All Matches                " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 0. Logout                          " << Y << "|" << X << "\n";
            maxChoice = 5;
        }
        else if(role == PLAYER) {
            cout << Y << "|" << X << " 1. View My Profile                 " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 2. View My Team                    " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 3. View My Stats                   " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 4. View All Matches                " << Y << "|" << X << "\n";
            cout << Y << "|" << X << " 0. Logout                          " << Y << "|" << X << "\n";
            maxChoice = 4;
        }
        
        cout << Y << "+====================================+" << X << "\n";
        return maxChoice;
    }
    
    void promptForChoice(int maxChoice) {
        cout << "\n" << B << "========== ENTER YOUR CHOICE ==========" << X << "\n";
        cout << "  Enter choice (0-" << maxChoice << ")  \n";
        cout << B << "======================================" << X << "\n";
        cout << B << ">> " << X;
    }
    
    bool handleChoice(int choice) {
        UserRole role = auth.getCurrentRole();
        bool continueSession = true;
        
        if(role == ADMIN) {
            switch(choice) {
                case 1: createPlayer(); break;
                case 2: createTeam(); break;
                case 3: addPlayerToTeam(); break;
                case 4: scheduleMatch(); break;
                case 5: playMatch(); break;
                case 6: displayAllPlayers(); break;
                case 7: displayAllTeams(); break;
                case 8: displayAllMatches(); break;
                case 0: 
                    auth.signOut(); 
                    continueSession = false;
                    break;
                default: 
                    cout << R << "[ERROR] Invalid choice!" << X << "\n";
            }
        }

        else if(role == COACH) {
            switch(choice) {
                case 1: displayAllPlayers(); break;
                case 2: displayAllTeams(); break;
                case 3: viewTeamDetails(); break;
                case 4: updatePlayerStats(); break;
                case 5: displayAllMatches(); break;
                case 0: 
                    auth.signOut(); 
                    continueSession = false;
                    break;
                default: 
                    cout << R << "[ERROR] Invalid choice!" << X << "\n";
            }
        }

        else if(role == PLAYER) {
            switch(choice) {
                case 1: viewMyProfile(); break;
                case 2: viewMyTeam(); break;
                case 3: viewMyStats(); break;
                case 4: displayAllMatches(); break;
                case 0: 
                    auth.signOut(); 
                    continueSession = false;
                    break;
                default: 
                    cout << R << "[ERROR] Invalid choice!" << X << "\n";
            }
        }
        
        return continueSession;
    }
    
    void createPlayer() {
        if(playerCount >= playerCapacity) {
            cout << R << "ERROR!! Maximum players reached." << X << "\n";
            return;
        }
        
        string name, role, country;
        int age, jersey;

        cout << "\n" << B << "--- Create New Player ---" << X << "\n";
        cout << B << "Name: " << X; getline(cin, name);
        cout << B << "Age: " << X; cin >> age;
        cout << B << "Jersey Number: " << X; cin >> jersey; cin.ignore();
        cout << B << "Role (Batsman/Bowler/All-rounder/WK): " << X; getline(cin, role);
        cout << B << "Country: " << X; getline(cin, country);
        
        allPlayers[playerCount] = new Player(name, age, role, country, jersey);
        cout << G << "Player created with ID: " << X << allPlayers[playerCount]->getPlayerID() << endl;
        playerCount++;
    }
    
    void createTeam() {
        if(teamCount >= teamCapacity) {
            cout << R << "ERROR!! Maximum teams reached." << X << "\n";
            return;
        }
        
        string name, country;
        cout << "\n" << B << "--- Create New Team ---" << X << "\n";
        cout << B << "Team Name: " << X; getline(cin, name);
        cout << B << "Country: " << X; getline(cin, country);

        allTeams[teamCount] = new Team(name, country);
        teamCount++;
        cout << G << "SUCCESS! Team created." << X << "\n";
    }
    
    void addPlayerToTeam() {
        if(teamCount == 0 || playerCount == 0) {
            cout << R << "ERROR! Need teams and players first." << X << "\n";
            return;
        }
        
        displayAllTeams();
        int teamIdx;
        cout << B << "Select team (0-" << teamCount-1 << "): " << X; 
        cin >> teamIdx; cin.ignore();
        
        if(teamIdx < 0 || teamIdx >= teamCount) return;
        
        displayAllPlayers();
        int playerIdx;
        cout << B << "Select player (0-" << playerCount-1 << "): " << X;
        cin >> playerIdx; cin.ignore();
        
        if(playerIdx < 0 || playerIdx >= playerCount) return;
        
        if(allTeams[teamIdx]->addPlayer(allPlayers[playerIdx])) {
            cout << G << "SUCCESS! Player added to team." << X << "\n";
        }
    }
    
    void scheduleMatch() {
        if(teamCount < 2) {
            cout << R << "ERROR! Need at least 2 teams." << X << "\n";
            return;
        }
        
        displayAllTeams();
        int teamA, teamB;
        cout << B << "Select Team A: " << X; cin >> teamA;
        cout << B << "Select Team B: " << X; cin >> teamB;
        
        int d, m, y;
        cout << B << "Enter Date (DD MM YYYY): " << X;
        cin >> d >> m >> y; cin.ignore();
        
        allMatches[matchCount] = new Match(allTeams[teamA], allTeams[teamB], Date(d, m, y));
        matchCount++;
        cout << G << "SUCCESS! Match scheduled." << X << "\n";
    }
    
    void playMatch() {
        if(matchCount == 0) {
            cout << R << "ERROR! No matches." << X << "\n";
            return;
        }
        
        displayAllMatches();
        int choice;
        cout << B << "Select match to play: " << X; cin >> choice; cin.ignore();
        
        if(choice >= 0 && choice < matchCount) {
            allMatches[choice]->play();
            allMatches[choice]->display();
        }
    }
    
    void viewTeamDetails() {
        displayAllTeams();
        int choice;
        cout << B << "Select team: " << X; cin >> choice; cin.ignore();
        if(choice >= 0 && choice < teamCount && allTeams[choice]) {
            allTeams[choice]->display();
        }
    }
    
    void updatePlayerStats() {
        displayAllPlayers();
        int choice;
        cout << B << "Select player: " << X; cin >> choice; cin.ignore();
        
        if(choice >= 0 && choice < playerCount && allPlayers[choice]) {
            int runs, wickets;
            cout << B << "Enter runs scored: " << X; cin >> runs;
            cout << B << "Enter wickets taken: " << X; cin >> wickets; cin.ignore();
            allPlayers[choice]->updateStats(runs, wickets);
            cout << G << "SUCCESS! Stats updated." << X << "\n";
        }
    }
    
    void viewMyProfile() {
        cout << "\n" << B << "--- My Profile ---" << X << "\n";
        displayAllPlayers();
    }
    
    void viewMyTeam() {
        cout << "\n" << B << "--- My Team ---" << X << "\n";
        displayAllTeams();
    }
    
    void viewMyStats() {
        cout << "\n" << B << "--- My Stats ---" << X << "\n";
        cout << "INFO: Stats view functionality\n";
    }
    
    void displayAllPlayers() const {
        cout << "\n" << B << "========== ALL PLAYERS ==========" << X << "\n";
        for(int i = 0; i < playerCount; i++) {
            if(allPlayers[i]) {
                cout << "[" << i << "] " << allPlayers[i]->getName() 
                     << " (" << allPlayers[i]->getPlayerID() << ")\n";
            }
        }
    }
    
    void displayAllTeams() const {
        cout << "\n" << B << "========== ALL TEAMS ==========" << X << "\n";
        for(int i = 0; i < teamCount; i++) {
            if(allTeams[i]) {
                cout << "[" << i << "] " << allTeams[i]->getTeamName() << endl;
            }
        }
    }
    
    void displayAllMatches() const {
        cout << "\n" << B << "========== ALL MATCHES ==========" << X << "\n";
        for(int i = 0; i < matchCount; i++) {
            if(allMatches[i]) {
                cout << "[" << i << "] ";
                allMatches[i]->display();
            }
        }
    }
};

void showAuthMenu() {
    cout << "\n" << Y << "+========== WELCOME ==========+" << X << "\n";
    cout << Y << "|" << X << " 1. Sign In                  " << Y << "|" << X << "\n";
    cout << Y << "|" << X << " 2. Sign Up                  " << Y << "|" << X << "\n";
    cout << Y << "|" << X << " 0. Exit                     " << Y << "|" << X << "\n";
    cout << Y << "+=============================+" << X << "\n";
    cout << B << "Choice: " << X;
}

UserRole selectRole() {
    cout << "\n" << Y << "+========== SELECT ROLE ==========+" << X << "\n";
    cout << Y << "|" << X << " 1. Admin                        " << Y << "|" << X << "\n";
    cout << Y << "|" << X << " 2. Coach                        " << Y << "|" << X << "\n";
    cout << Y << "|" << X << " 3. Player                       " << Y << "|" << X << "\n";
    cout << Y << "+=================================+" << X << "\n";
    cout << B << "Choice: " << X;
    
    int choice;
    cin >> choice; cin.ignore();
    
    switch(choice) {
        case 1: return ADMIN;
        case 2: return COACH;
        case 3: return PLAYER;
        default: return NONE;
    }
}

int main() {
    AuthSystem authSystem;
    CricketSystem cricketSystem(authSystem);
    
    cout << Y << "+==================================================+" << X << "\n";
    cout << Y << "|" << X << "     CRICKET CLUB  MANAGEMENT SYSTEM    " << Y << "|" << X << "\n";
    cout << Y << "|           (DOMAIN: CRICKET )                      " << Y << "|" << X << "\n";
    cout << Y << "+==================================================+" << X << "\n";
    cout << G << "DEFAULT ACCOUNTS!" << X << "\n";
    cout << "Admin:  username='admin',   password='admin123'\n";
    cout << "Coach:  username='coach1',  password='coach123'\n";
    
    bool running = true;
    
    while(running) {
        if(!authSystem.isLoggedIn()) {
            showAuthMenu();
            int authChoice;
            cin >> authChoice;
            cin.ignore();
            
            string username, password;
            
            switch(authChoice) {
                case 1: {
                    cout << "\n" << B << "--- Sign In ---" << X << "\n";
                    cout << B << "Username: " << X; getline(cin, username);
                    cout << B << "Password: " << X; getline(cin, password);
                    
                    if(authSystem.signIn(username, password)) {
                        authSystem.displayCurrentUser();
                    }
                    break;
                }
                
                case 2: {
                    cout << "\n" << B << "--- Sign Up ---" << X << "\n";
                    cout << B << "Username: " << X; getline(cin, username);
                    cout << B << "Password: " << X; getline(cin, password);
                    
                    UserRole role = selectRole();
                    if(role == NONE) {
                        cout << R << "ERROR! Invalid role selection." << X << "\n";
                        break;
                    }
                    authSystem.signUp(username, password, role);
                    break;
                }
                
                case 0:
                    running = false;
                    cout << G << "Goodbye!" << X << "\n";
                    break;
                    
                default:
                    cout << R << "ERROR! Invalid choice." << X << "\n";
            }
        }
        else {
            int maxChoice = cricketSystem.displayMenuOnce();
            bool inSession = true;
            
            while(inSession && authSystem.isLoggedIn()) {
                cricketSystem.promptForChoice(maxChoice);
                
                int choice;
                cin >> choice;
                cin.ignore();
                
                if(choice < 0 || choice > maxChoice) {
                    cout << R << "ERROR! Invalid choice! Please enter 0-" << X << maxChoice << "\n";
                    continue;
                }
                
                inSession = cricketSystem.handleChoice(choice);
            }
        }
    }
    
    return 0;
}