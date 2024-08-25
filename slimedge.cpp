#include <thread>
#include <random>
#include <mutex>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <string>
#include <cmath>
#include <numeric>
using namespace std;
//rules
int surrender = 0;//surrender allowed?
int doubleAfterSplit = 1;//double after split allowed?
int s17 = 0;//stand on all 17s?
int maxSplits = 3;//max number of splits allowed?
int numDecks = 6;//number of decks?
double penetration = 1.5;//number of decks under the shoe?
int reSplitAces = 1;
double blackJackModifier = 1.5;
//parameters
int iterations = 1000000;
int minBet = 1;
int maxBet = 12;
//long double bankroll = 1000;
//int goal = 600;
int evCentric = 0;
//int risk = 25;
//double riskOfRuin = 1;
int debug = 0;
string fileName = "result.csv";
//variables
mutex mtx;
vector<double> totalHands;
thread_local vector<double> threadHands;
int hsh[22][11] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1},
                   {1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                   {1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                   {1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                   {1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };
int hss[22][11] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };
int sp[11][11] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                  {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
                  {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };
int sr[22][11];//surrender?
int dbh[22][11] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
                   {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                   {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };
int dbs[22][11] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
                   {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
                   {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
                   {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };
thread_local int shoe[1000];//shoe
int counts[11] = { 0, -1, 1, 1, 1, 1, 1, 0, 0, 0, -1 };
int spread[7] = { 1, 2, 3, 4, 5, 6, 7 };
thread_local int newHands[3][300];
thread_local int totals[2][300];
thread_local int hand[300];
thread_local int dealer[300];
int numThreads;
int perThread;
vector<thread> threads;
int numCards = 52 * numDecks;
int numPen = 52 * penetration;
thread_local auto rng = mt19937{ random_device{}() };
//struct for saving results
struct Spread {
    int c0 = 0;
    int c1 = 0;
    int c2 = 0;
    int c3 = 0;
    int c4 = 0;
    int c5 = 0;
    int c6 = 0;//bet at counts
    int hands = 0;
    long double ev = 0;//ev over however many hands
    long double risk = 0;//standard deviation over however many hands
};
vector<Spread> resultsVector;
int compare(Spread s1, Spread s2) {
    if (evCentric) {
        if (s1.ev != s2.ev) return s1.ev > s2.ev;
        else return s1.risk < s2.risk;
    }
    if (s1.risk != s2.risk) return s1.risk < s2.risk;
    else return s1.ev > s2.ev;
}
//functions
void simulate(int begin, int end) {
    //cout << "\nthread made" << endl;
    rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count() + std::hash<std::thread::id>{}(std::this_thread::get_id()));
    threadHands.clear();
    int temp = 0;
    for (int i = 0; i < numDecks; i++) {
        for (int i = 0; i < 4; i++) {
            for (int i = 1; i <= 13; i++) {
                shoe[temp++] = min(10, i);
            }
        }
    }
    int hi = 0;
    int si = 0;
    int di = 0;
    int ti = 0;
    int ni = 0;
    int nn = 0;
    int rc = 0;
    int tc = 0;
    int split = 0;
    int total = 0;
    int soft = 0;
    int dealerDone = 0;
    int bet = 0;
    int sHands = 0;
    for (begin; begin < end; begin++) {//each round of sims will increment this
        int handsDone = 0;
        double earned = 0;
        while (handsDone < 100) {
            shuffle(shoe, shoe + numCards, rng);
            si = 0;
            rc = 0;
            tc = 0;
            while (handsDone < 100 && numCards - si > numPen && tc > -1) { //simulate a single shoe
                if (debug) cout << "Tc: " << tc << endl;
                if (debug) cout << "rc: " << rc << endl;
                if (debug) cout << "used: " << si << endl;
                tc = rc / ((numCards - si) / 52);
                tc = min(6, tc);
                dealerDone = 0;
                hi = 2;
                di = 2;
                ti = 0;
                ni = 0;
                nn = 1;
                split = 0;
                bet = spread[tc];
                newHands[1][0] = shoe[si++];
                newHands[2][0] = shoe[si++];
                newHands[0][0] = 0;
                dealer[0] = shoe[si++];
                dealer[1] = shoe[si++];
                handsDone++;
                rc += counts[newHands[1][0]] + counts[newHands[2][0]] + counts[dealer[0]] + counts[dealer[1]];
                for (ni = 0; ni < nn; ni++) {
                    soft = 0;
                    hi = 2;
                    split = newHands[0][ni];
                    hand[0] = newHands[1][ni];
                    hand[1] = newHands[2][ni];
                    total = hand[0] + hand[1];
                    if (hand[0] == 1 || hand[1] == 1) {
                        soft = 1;
                        total += 10;
                    }
                    //check blackjack
                    if (total == 21 && split == 0) {
                        if (dealer[0] == 1 && dealer[1] == 10 || dealer[0] == 10 && dealer[1] == 1) {
                            if (debug) cout << "blackjack push" << endl;
                            //threadHands.push_back(0);
                            nn = 0;
                            break;
                        }
                        //br += blackJackModifier * bet;
                        //threadHands.push_back(blackJackModifier * bet);
                        earned += blackJackModifier * bet;
                        if (debug) {
                            cout << "blackjack" << endl;
                            //cout << br << endl;
                        }
                        nn = 0;
                        break;
                    }
                    if (dealer[0] == 1) {
                        if (dealer[1] != 10 && tc >= 3) {
                            //br -= 0.5 * bet;
                            //threadHands.push_back(-0.5 * bet);
                            earned -= 0.5 * bet;
                        }
                        if (dealer[1] == 10 && tc >= 3) {
                            nn = 0;
                            //threadHands.push_back(0);
                            break;
                        }
                        else if (tc >= 3) {
                        }
                        if (dealer[1] == 10) {
                            //br -= bet;
                            //threadHands.push_back(-1 * bet);
                            earned -= bet;
                            nn = 0;
                            break;
                        }
                    }
                    if (dealer[0] == 10 && dealer[1] == 1) {
                        //br -= bet;
                        //threadHands.push_back(-1 * bet);
                        earned -= bet;
                        nn = 0;
                        if (debug) {
                            cout << "Dealer's blackajck" << endl;
                            //cout << br << endl;
                        }
                        break;
                    }
                    if (surrender && sr[total][dealer[0]] && split == 0) {
                        //br -= 0.5 * bet;
                        //threadHands.push_back(-0.5 * bet);
                        earned -= 0.5 * bet;
                        nn = 0;
                        break;
                    }
                    if (split < maxSplits && hand[0] == hand[1] && sp[hand[0]][dealer[0]] && !(!reSplitAces && !split && hand[0] == 1)) {
                        nn++;
                        if (debug) {
                            cout << endl << endl << "SPLIT" << endl;
                            cout << sp[hand[0]][dealer[0]] << endl << endl;
                        }
                        newHands[0][ni + 1] = ++split;
                        newHands[1][ni + 1] = hand[1];
                        newHands[2][ni + 1] = shoe[si++];
                        rc += counts[hand[1]] + counts[hand[2]];
                        hand[1] = shoe[si++];
                        rc += counts[hand[1]];
                        soft = 0;
                        total = hand[0] + hand[1];
                        if (total <= 11 && (hand[0] == 1 || hand[1] == 1)) {
                            soft = 1;
                            total += 10;
                        }
                        if (debug) {
                            cout << "split" << endl;
                            cout << hand[0];
                            for (int i = 1; i < hi; i++) cout << "/" << hand[i];
                            cout << " vs " << dealer[0] << endl;
                        }
                    }
                    if (doubleAfterSplit || split == 0) {
                        if (!soft && dbh[total][dealer[0]] || soft && dbs[total][dealer[0]]) {
                            hand[2] = shoe[si++];
                            rc += counts[hand[2]];
                            total += hand[2];
                            if (hand[2] == 1 && total <= 11) {
                                total += 10;
                                soft = 1;
                            }
                            else if (soft && total > 21) {
                                total -= 10;
                                soft = 0;
                            }
                            totals[0][ni] = total;
                            totals[1][ni] = 2;
                            if (debug) {
                                cout << "doubled" << endl;
                                cout << hand[0];
                                for (int i = 1; i < hi; i++) cout << "/" << hand[i];
                                cout << " vs " << dealer[0] << endl;
                            }
                            continue;
                        }
                    }
                    while (total < 21) {
                        if (soft) {
                            if (!hss[total][dealer[0]]) break;
                        }
                        else {
                            if (!hsh[total][dealer[0]]) break;
                        }
                        hand[hi] = shoe[si++];
                        total += hand[hi];
                        if (!soft && total <= 11 && hand[hi] == 1) {
                            total += 10;
                            soft = 1;
                        }
                        else if (soft && total > 21) {
                            total -= 10;
                            soft = 0;
                        }
                        rc += counts[hand[hi++]];
                        if (debug) {
                            cout << hand[0];
                            for (int i = 1; i < hi; i++) cout << "/" << hand[i];
                            cout << " vs " << dealer[0] << endl;
                        }
                    }
                    totals[0][ni] = total;
                    totals[1][ni] = 1;
                }
                for (int i = 0; i < nn; i++) {
                    if (totals[0][i] > 21) {//player busts
                        //br -= bet * totals[1][i];
                        //threadHands.push_back(-1 * totals[1][i] * bet);
                        earned -= totals[1][i] * bet;
                        if (debug) {
                            cout << totals[0][i] << ": Player busts!" << endl;
                            //cout << br << endl;
                        }
                    }
                    else {
                        if (!dealerDone) {
                            dealerDone = 1;
                            soft = 0;
                            total = dealer[0] + dealer[1];
                            if ((dealer[0] == 1 || dealer[1] == 1) && total <= 11) {
                                soft = 1;
                                total += 10;
                            }
                            while (total <= 17) {
                                if (total == 17 && (!soft || s17)) break;
                                dealer[di] = shoe[si++];
                                total += dealer[di];
                                rc += counts[dealer[di++]];
                                if (total <= 11 && dealer[di - 1] == 1) {
                                    soft = 1;
                                    total += 10;
                                }
                                else if (total > 21 && soft) {
                                    soft = 0;
                                    total -= 10;
                                }
                            }
                            if (debug) {
                                cout << dealer[0];
                                for (int j = 1; j < di; j++) {
                                    cout << "/" << dealer[j];
                                }
                                cout << endl;
                            }
                        }
                        if (debug) cout << totals[0][i] << ":" << total << endl;
                        if (total > 21) {
                            //br += bet * totals[1][i];//dealer bust
                            //threadHands.push_back(totals[1][i] * bet);
                            earned += totals[1][i] * bet;
                            if (debug) {
                                cout << "dealer busts" << endl;
                                //cout << br << endl;
                            }
                        }
                        else if (totals[0][i] > total) {
                            //br += bet * totals[1][i];
                            //threadHands.push_back(totals[1][i] * bet);
                            earned += totals[1][i] * bet;
                            if (debug) {
                                cout << "player win" << endl;
                                //cout << br << endl;
                            }
                        }
                        else if (totals[0][i] < total) {
                            //br -= bet * totals[1][i];
                            //threadHands.push_back(-1 * totals[1][i] * bet);
                            earned -= totals[1][i] * bet;
                            if (debug) {
                                cout << "dealer win" << endl;
                                //cout << br << endl;
                            }
                        }
                        //else threadHands.push_back(0);//push
                    }
                }
                tc = rc / ((numCards - si) / 52);
                tc = min(6, tc);
            }
        }
        threadHands.push_back(earned);
    }
}
void runSim() {//average EV, standard deviation. perhaps it would be better to push all results to a single vector, then conduct the analysis on the vector.
    threads.clear();
    int hands = 0;
    int fails = 0;
    int earned = 0;
    for (int i = 0; i < numThreads; i++) {
        int start = i * perThread;
        int end = i == numThreads - 1 ? iterations : start + perThread;
        threads.emplace_back([start, end]() {
            int threadEarned = 0;
            simulate(start, end);
            {
                lock_guard<mutex> lock(mtx);
                for (auto a : threadHands) {
                    totalHands.push_back(a);
                }
            }
            });
    }
    for (auto& t : threads) {
        t.join();
    }
}
int main(int argC, char* argV[]) {
    numThreads = thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 2;
    //numThreads = 1;
    perThread = iterations / numThreads;
    int hands = 0;
    int fails = 0;
    string type = "";
    string arg = "";
    //cout << argC << ":" << argV[0] << endl;
    for (int i = 1; i < argC; i += 2) {
        type = argV[i];
        if (type == "-help") {
            cout << "parameters: -s17, -surrender, -das, -maxSplits, -numDecks, -penetration, -reSplitAces, -blackJackModifier, -iterations, -minBet, -maxBet, -bankroll, -goal, -debug, -evCentric, -o" << endl;
            return 0;
        }
        else {
            arg = argV[i + 1];
        }
        if (type == "-s17") {
            s17 = stoi(arg);
        }
        else if (type == "-surrender") {
            surrender = stoi(arg);
        }
        else if (type == "-das") {
            doubleAfterSplit = stoi(arg);
        }
        else if (type == "-maxSplits") {
            maxSplits = stoi(arg);
        }
        else if (type == "-numDecks") {
            numDecks = stoi(arg);
        }
        else if (type == "-penetration") {
            penetration = stod(arg);
        }
        else if (type == "-reSplitAces") {
            reSplitAces = stoi(arg);
        }
        else if (type == "-blackJackModifier") {
            blackJackModifier = stod(arg);
        }
        else if (type == "-iterations") {
            iterations = stoi(arg);
        }
        else if (type == "-minBet") {
            minBet = stoi(arg);
        }
        else if (type == "-maxBet") {
            maxBet = stoi(arg);
        }
        else if (type == "-bankroll") {
            //bankroll = stoi(arg);
        }
        else if (type == "-goal") {
            //goal = stoi(arg);
        }
        else if (type == "-debug") {
            debug = stoi(arg);
        }
        else if (type == "-o") {
            fileName = arg;
        }
        else if (type == "-evCentric") {
            //evCentric = stoi(arg);
        }
    }
    sr[15][10] = sr[16][10] = sr[16][9] = sr[16][1] = 1;
    if (debug) {
        //bankroll = 1000;
        //goal = 2000;
        numThreads = 2;
        perThread = iterations = 1;
        iterations = 2;
        maxBet = 1;
        minBet = 1;
    }
    if (!s17) {
        //h17 differences
        dbs[18][2] = 1;
        dbs[19][6] = 1;
        dbh[11][1] = 1;
        sr[15][1] = sr[17][1] = 1;
    }
    if (!doubleAfterSplit) {
        sp[2][2] = sp[2][3] = sp[3][2] = sp[3][3] = 0;
        sp[6][2] = 0;
        sp[4][5] = sp[4][6] = 0;
    }
    int fs, hs;
    //auto a = runSim();
    //fs = a.first;
    //hs = a.second;
    int totalSpread = 0;
    for (int s1 = minBet; s1 <= maxBet; s1++) {
        for (int s2 = s1; s2 <= maxBet; s2++) {
            for (int s3 = s2; s3 <= maxBet; s3++) {
                for (int s4 = s3; s4 <= maxBet; s4++) {
                    for (int s5 = s4; s5 <= maxBet; s5++) {
                        for (int s6 = s5; s6 <= maxBet; s6++) {
                            totalSpread++;
                        }
                    }
                }
            }
        }
    }
    //int safest = INT_MAX;
    //int optimal = INT_MAX;
    //risk = iterations / 100 * risk;
    int currentSpread = 1;
    //int possible = 0;
    //int optimals[7] = {1, 0, 0, 0, 0, 0, 0};
    //int optf = 0;
    perThread = iterations / numThreads;
    spread[0] = minBet;
    for (int s1 = minBet; s1 <= maxBet; s1++) {
        for (int s2 = s1; s2 <= maxBet; s2++) {
            for (int s3 = s2; s3 <= maxBet; s3++) {
                for (int s4 = s3; s4 <= maxBet; s4++) {
                    for (int s5 = s4; s5 <= maxBet; s5++) {
                        for (int s6 = s5; s6 <= maxBet; s6++) {
                            Spread s = { minBet, s1, s2, s3, s4, s5, s6, 0, 0, 0 };
                            cout << "\rSpread " << currentSpread++ << " of " << totalSpread << flush;
                            spread[1] = s1;
                            spread[2] = s2;
                            spread[3] = s3;
                            spread[4] = s4;
                            spread[5] = s5;
                            spread[6] = s6;
                            totalHands.clear();
                            runSim();
                            double sum = accumulate(totalHands.begin(), totalHands.end(), 0);
                            double avg = sum / totalHands.size();//this is the average EV per 100 hands
                            long double variance = 0;//this will store the variance in the ev per 100 hands
                            for (double ev : totalHands) {
                                variance += (ev - avg) * (ev - avg);
                            }
                            variance /= totalHands.size(); // Divide by the number of hands
                            long double sd = sqrt(variance);
                            s.ev = avg;
                            s.risk = sd;
                            s.hands = totalHands.size();
                            //s.risk = (double)(fs)/iterations * 100;
                            //double ev = (iterations - fs) * (goal - bankroll) - fs * (goal - bankroll);
                            //ev /= hs;
                            //if (fs != 0) s.ev = ev;
                            resultsVector.push_back(s);
                            //                            cout << endl;
                            //                            for (auto a: totalHands) cout << a << " ";
                        }
                    }
                }
            }
        }
    }
    /*
    if (!possible) cout << endl << "impossible for that risk." << endl;
    else {
        cout << endl;
        for (int i: optimals) cout << i << " ";
        cout << endl << double(goal - bankroll)/(optimal/(iterations-optf)) << endl;
    }
    */
    ofstream file;
    file.open(fileName);
    file << "count 0, count 1, count 2, count 3, count 4, count 5, count 6, ror before adjustment, sd, ev\n";
    for (Spread s : resultsVector) {
        long double riskOfRuin = 0;
        long double factor = s.ev / s.risk;
        riskOfRuin = 1 - factor;
        riskOfRuin *= 1.0L / (1 + factor);
        //file << riskOfRuin << ",";
        riskOfRuin = pow(riskOfRuin, 1 / s.risk);//Take this to the power of bankroll to get risk of ruin
        if (s.ev <= 0) riskOfRuin = 1;
        file << s.c0 << "," << s.c1 << "," << s.c2 << "," << s.c3 << "," << s.c4 << "," << s.c5 << "," << s.c6 << "," << riskOfRuin << "," << s.risk << "," << s.ev << "\n";
    }
    //cout << "EV per Hand:" << (double)(goal - bankroll)/(hs/(iterations-fs));
    //cout << "\nRisk of Ruin:" << (double)fs/iterations * 100 << "%";
}
