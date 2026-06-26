
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <map>

using namespace std;
//------------- AUTHENTICATION SYSTEM-----------------
enum UserRole { ADMIN, COACH, PLAYER, NONE }; //An enum is used to fixed set of related values.Instead of using numbers like 0, 1, 2, you use meaningful names.eg.value1,value2...
struct User {//create structure
    string username;
    string password;
    UserRole role;
    string linkedID;
    User() : role(NONE) {} //constructor + initialized list
    User(string u, string p, UserRole r, string id = "") 
        : username(u), password(p), role(r), linkedID(id) {}
};

class AuthSystem {//create class
private:
    map<string, User> users;//ye key value h ,You are storing users with a unique string key
    User currentUser;
public:
    AuthSystem() {
        users["admin"] = User("admin", "admin123", ADMIN); //fiwed stored adimin username and password
        users["coach1"] = User("coach1", "coach123", COACH);
    }
    
    bool signUp(string username, string password, UserRole role, string linkedID = "") { //condition either true or not for signup
        if(users.find(username) != users.end()) {
            cout << " Username already exists!\n";
            return false;
        }
        if(username.empty() || password.empty()) {
            cout << " Username and password cannot be empty!\n";
            return false;
        }
        users[username] = User(username, password, role, linkedID);
        cout << " Account created! Please sign in.\n";
        return true;
    }
    
    bool signIn(string username, string password) {//condition for signin
        auto it = users.find(username);
        if(it == users.end()) {
            cout << " Username not found!\n";
            return false;
        }
        if(it->second.password != password) {
            cout << " Incorrect password!\n";
            return false;
        }
        currentUser = it->second;
        cout << " Welcome, " << username << "!\n";
        return true;
    }
    
    void signOut() {//function for signout /exist
        currentUser = User();
        cout << " Signed out successfully.\n";
    }
    
    UserRole getCurrentRole() const { return currentUser.role; }
    string getCurrentUsername() const { return currentUser.username; }
    bool isLoggedIn() const { return currentUser.role != NONE; }
    
    void displayCurrentUser() const {
        cout << "\n+========== USER INFO ==========+\n";
        cout << "| User: " << left << setw(26) << currentUser.username << "|\n";
        cout << "| Role: " << left << setw(26) 
             << (currentUser.role == ADMIN ? "Administrator" : 
                 currentUser.role == COACH ? "Coach" : "Player") << "|\n";
        cout << "+===============================+\n";
    }
};

// Forward declarations
class Player;
class Team;
class Match;

// ==================== UTILITY CLASS ====================
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

// ==================== DATE CLASS ====================
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

// ==================== PLAYER STATS ====================
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

// ==================== PLAYER CLASS ====================
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
        cout << "\n+========== PLAYER ==========+\n";
        cout << "| ID: " << left << setw(25) << playerID << "|\n";
        cout << "| Name: " << left << setw(23) << name << "|\n";
        cout << "| Role: " << left << setw(23) << role << "|\n";
        cout << "| Country: " << left << setw(20) << country << "|\n";
        cout << "| Jersey: " << left << setw(21) << jerseyNumber << "|\n";
        if(stats) { cout << "| "; stats->display(); cout << "|\n"; }
        cout << "+============================+\n";
    }
};

int Player::totalPlayers = 0;

// ==================== TEAM CLASS ====================
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
        cout << "\n+========== TEAM ==========+\n";
        cout << "| ID: " << left << setw(23) << teamID << "|\n";
        cout << "| Name: " << left << setw(21) << teamName << "|\n";
        cout << "| Players: " << left << setw(18) << currentPlayers << "/" << maxPlayers << "|\n";
        cout << "+==========================+\n";
        for(int i = 0; i < currentPlayers; i++) {
            if(players[i]) {
                cout << "| " << (i+1) << ". " << left << setw(23) 
                     << players[i]->getName() << "|\n";
            }
        }
    }
    
    string getTeamID() const { return teamID; }
    string getTeamName() const { return teamName; }
};

// ==================== MATCH CLASS ====================
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
        cout << "\n+========== MATCH ==========+\n";
        cout << "| ID: " << left << setw(24) << matchID << "|\n";
        if(teamA && teamB) {
            cout << "| " << left << setw(12) << teamA->getTeamName() << ": " << scoreA << "|\n";
            cout << "| " << left << setw(12) << teamB->getTeamName() << ": " << scoreB << "|\n";
        }
        cout << "| Winner: " << left << setw(19) << winner << "|\n";
        cout << "+===========================+\n";
    }
};

// ==================== CRICKET SYSTEM ====================
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
    
    // Display menu only once - returns max choice number for validation
    int displayMenuOnce() {
        UserRole role = auth.getCurrentRole();
        int maxChoice = 0;
        
        cout << "\n+========== CRICKET SYSTEM ==========+\n";
        
        if(role == ADMIN) {
            cout << "| 1. Create Player                   |\n";
            cout << "| 2. Create Team                     |\n";
            cout << "| 3. Add Player to Team              |\n";
            cout << "| 4. Schedule Match                  |\n";
            cout << "| 5. Play Match                      |\n";
            cout << "| 6. View All Players                |\n";
            cout << "| 7. View All Teams                  |\n";
            cout << "| 8. View All Matches                |\n";
            cout << "| 0. Logout                          |\n";
            maxChoice = 8;
        }
        else if(role == COACH) {
            cout << "| 1. View All Players                |\n";
            cout << "| 2. View All Teams                  |\n";
            cout << "| 3. View Team Details               |\n";
            cout << "| 4. Update Player Stats             |\n";
            cout << "| 5. View All Matches                |\n";
            cout << "| 0. Logout                          |\n";
            maxChoice = 5;
        }
        else if(role == PLAYER) {
            cout << "| 1. View My Profile                 |\n";
            cout << "| 2. View My Team                    |\n";
            cout << "| 3. View My Stats                   |\n";
            cout << "| 4. View All Matches                |\n";
            cout << "| 0. Logout                          |\n";
            maxChoice = 4;
        }
        
        cout << "+====================================+\n";
        return maxChoice;
    }
    
    // Decorative prompt with == and ++
    void promptForChoice(int maxChoice) {
        cout << "\n========== ENTER YOUR CHOICE ==========\n";
        cout << "  Enter choice (0-" << maxChoice << ")  \n";
        cout << "======================================\n";
        cout << ">> ";
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
                    cout << "[ERROR] Invalid choice!\n";
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
                    cout << "[ERROR] Invalid choice!\n";
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
                    cout << "[ERROR] Invalid choice!\n";
            }
        }
        
        return continueSession;
    }
    
    // Admin functions
    void createPlayer() {
        if(playerCount >= playerCapacity) {
            cout << "[ERROR] Maximum players reached!\n";
            return;
        }
        
        string name, role, country;
        int age, jersey;
        
        cout << "\n--- Create New Player ---\n";
        cout << "Name: "; getline(cin, name);
        cout << "Age: "; cin >> age;
        cout << "Jersey Number: "; cin >> jersey; cin.ignore();
        cout << "Role (Batsman/Bowler/All-rounder/WK): "; getline(cin, role);
        cout << "Country: "; getline(cin, country);
        
        allPlayers[playerCount] = new Player(name, age, role, country, jersey);
        cout << "Player created with ID: " << allPlayers[playerCount]->getPlayerID() << endl;
        playerCount++;
    }
    
    void createTeam() {
        if(teamCount >= teamCapacity) {
            cout << "[ERROR] Maximum teams reached!\n";
            return;
        }
        
        string name, country;
        cout << "\n--- Create New Team ---\n";
        cout << "Team Name: "; getline(cin, name);
        cout << "Country: "; getline(cin, country);
        
        allTeams[teamCount] = new Team(name, country);
        teamCount++;
        cout << "[SUCCESS] Team created!\n";
    }
    
    void addPlayerToTeam() {
        if(teamCount == 0 || playerCount == 0) {
            cout << "[ERROR] Need teams and players first!\n";
            return;
        }
        
        displayAllTeams();
        int teamIdx;
        cout << "Select team (0-" << teamCount-1 << "): "; 
        cin >> teamIdx; cin.ignore();
        
        if(teamIdx < 0 || teamIdx >= teamCount) return;
        
        displayAllPlayers();
        int playerIdx;
        cout << "Select player (0-" << playerCount-1 << "): ";
        cin >> playerIdx; cin.ignore();
        
        if(playerIdx < 0 || playerIdx >= playerCount) return;
        
        if(allTeams[teamIdx]->addPlayer(allPlayers[playerIdx])) {
            cout << "[SUCCESS] Player added to team!\n";
        }
    }
    
    void scheduleMatch() {
        if(teamCount < 2) {
            cout << "[ERROR] Need at least 2 teams!\n";
            return;
        }
        
        displayAllTeams();
        int teamA, teamB;
        cout << "Select Team A: "; cin >> teamA;
        cout << "Select Team B: "; cin >> teamB;
        
        int d, m, y;
        cout << "Enter Date (DD MM YYYY): ";
        cin >> d >> m >> y; cin.ignore();
        
        allMatches[matchCount] = new Match(allTeams[teamA], allTeams[teamB], Date(d, m, y));
        matchCount++;
        cout << "[SUCCESS] Match scheduled!\n";
    }
    
    void playMatch() {
        if(matchCount == 0) {
            cout << "[ERROR] No matches!\n";
            return;
        }
        
        displayAllMatches();
        int choice;
        cout << "Select match to play: "; cin >> choice; cin.ignore();
        
        if(choice >= 0 && choice < matchCount) {
            allMatches[choice]->play();
            allMatches[choice]->display();
        }
    }
    
    // Coach functions
    void viewTeamDetails() {
        displayAllTeams();
        int choice;
        cout << "Select team: "; cin >> choice; cin.ignore();
        if(choice >= 0 && choice < teamCount && allTeams[choice]) {
            allTeams[choice]->display();
        }
    }
    
    void updatePlayerStats() {
        displayAllPlayers();
        int choice;
        cout << "Select player: "; cin >> choice; cin.ignore();
        
        if(choice >= 0 && choice < playerCount && allPlayers[choice]) {
            int runs, wickets;
            cout << "Enter runs scored: "; cin >> runs;
            cout << "Enter wickets taken: "; cin >> wickets; cin.ignore();
            allPlayers[choice]->updateStats(runs, wickets);
            cout << "[SUCCESS] Stats updated!\n";
        }
    }
    
    // Player functions
    void viewMyProfile() {
        cout << "\n--- My Profile ---\n";
        displayAllPlayers();
    }
    
    void viewMyTeam() {
        cout << "\n--- My Team ---\n";
        displayAllTeams();
    }
    
    void viewMyStats() {
        cout << "\n--- My Stats ---\n";
        cout << "[INFO] Stats view functionality\n";
    }
    
    // Common display functions
    void displayAllPlayers() const {
        cout << "\n========== ALL PLAYERS ==========\n";
        for(int i = 0; i < playerCount; i++) {
            if(allPlayers[i]) {
                cout << "[" << i << "] " << allPlayers[i]->getName() 
                     << " (" << allPlayers[i]->getPlayerID() << ")\n";
            }
        }
    }
    
    void displayAllTeams() const {
        cout << "\n========== ALL TEAMS ==========\n";
        for(int i = 0; i < teamCount; i++) {
            if(allTeams[i]) {
                cout << "[" << i << "] " << allTeams[i]->getTeamName() << endl;
            }
        }
    }
    
    void displayAllMatches() const {
        cout << "\n========== ALL MATCHES ==========\n";
        for(int i = 0; i < matchCount; i++) {
            if(allMatches[i]) {
                cout << "[" << i << "] ";
                allMatches[i]->display();
            }
        }
    }
};

// ============================================================================
// MAIN FUNCTION - MODIFIED FLOW
// ============================================================================

void showAuthMenu() {
    cout << "\n+========== WELCOME ==========+\n";
    cout << "| 1. Sign In                  |\n";
    cout << "| 2. Sign Up                  |\n";
    cout << "| 0. Exit                     |\n";
    cout << "+=============================+\n";
    cout << "Choice: ";
}

UserRole selectRole() {
    cout << "\n+========== SELECT ROLE ==========+\n";
    cout << "| 1. Admin                        |\n";
    cout << "| 2. Coach                        |\n";
    cout << "| 3. Player                       |\n";
    cout << "+=================================+\n";
    cout << "Choice: ";
    
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
    
    cout << "+==================================================+\n";
    cout << "|     SPORT CLUB  MANAGEMENT SYSTEM                |\n";
    cout << "|        (DOMAIN: CRICKET )                        |\n";
    cout << "+==================================================+\n";
    cout << "[DEFAULT ACCOUNTS]\n";
    cout << "Admin:  username='admin',   password='admin123'\n";
    cout << "Coach:  username='coach1',  password='coach123'\n";
    
    bool running = true;
    
    while(running) {
        if(!authSystem.isLoggedIn()) {
            // AUTHENTICATION MENU (shows every time when logged out)
            showAuthMenu();
            int authChoice;
            cin >> authChoice;
            cin.ignore();
            
            string username, password;
            
            switch(authChoice) {
                case 1: {
                    cout << "\n--- Sign In ---\n";
                    cout << "Username: "; getline(cin, username);
                    cout << "Password: "; getline(cin, password);
                    
                    if(authSystem.signIn(username, password)) {
                        authSystem.displayCurrentUser();
                    }
                    break;
                }
                
                case 2: {
                    cout << "\n--- Sign Up ---\n";
                    cout << "Username: "; getline(cin, username);
                    cout << "Password: "; getline(cin, password);
                    
                    UserRole role = selectRole();
                    if(role == NONE) {
                        cout << "[ERROR] Invalid role selection!\n";
                        break;
                    }
                    authSystem.signUp(username, password, role);
                    break;
                }
                
                case 0:
                    running = false;
                    cout << "Goodbye!\n";
                    break;
                    
                default:
                    cout << "[ERROR] Invalid choice!\n";
            }
        }
        else {
            // LOGGED IN - Show menu ONCE, then just prompt for choices
            int maxChoice = cricketSystem.displayMenuOnce();
            bool inSession = true;
            
            while(inSession && authSystem.isLoggedIn()) {
                cricketSystem.promptForChoice(maxChoice);
                
                int choice;
                cin >> choice;
                cin.ignore();
                
                if(choice < 0 || choice > maxChoice) {
                    cout << "[ERROR] Invalid choice! Please enter 0-" << maxChoice << "\n";
                    continue;
                }
                
                inSession = cricketSystem.handleChoice(choice);
                // Menu is NOT redisplayed - just loops back to prompt
            }
        }
    }
    
    return 0;
}