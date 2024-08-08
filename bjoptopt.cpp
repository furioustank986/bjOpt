#include <thread>
#include <random>
#include <mutex>
#include <vector>
#include <algorithm>
#include <iostream>
#include <limits.h>
using namespace std;
//variables
mutex mtx;
int hsh[22][11] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
int hss[22][11] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
int sp[11][11] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                  {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
                  {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
int sr[22][11];//surrender?
int dbh[22][11] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
int dbs[22][11] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
thread_local int shoe[1000];//shoe
int counts[11] = {0, -1, 1, 1, 1, 1, 1, 0, 0, 0, -1};
int spread[7];
thread_local int newHands[3][300];
thread_local int totals[2][300];
thread_local int hand[300];
thread_local int dealer[300];
//rules
int surrender = 0;//surrender allowed?
int doubleAfterSplit = 1;//double after split allowed?
int s17 = 1;//stand on all 17s?
int maxSplits = 1;//max number of splits allowed?
int numDecks = 6;//number of decks?
int penetration = 2;//number of decks under the shoe?
int numCards = 52 * numDecks;
int numPen = 52 * penetration;
thread_local auto rng = mt19937{random_device{}()};
double blackJackModifier = 1.5;
//parameters
int iterations = 1000;
int minBet = 1;
int maxBet = 10;
int bankroll = 500;
int goal = 1000;
double riskOfRuin = 1;
int debug = 0;
//functions
void simulate(int begin, int end, int &hands, int &fails){
    //cout << "\nthread made" << endl;
    rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count() + std::hash<std::thread::id>{}(std::this_thread::get_id()));
    double br = bankroll;
    int temp = 0;
    for (int i = 0; i < numDecks; i++){
        for (int i = 0; i < 4; i++){
            for (int i = 1; i <= 13; i++){
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
    for (begin; begin < end; begin++){
        sHands = 0;
        br = bankroll;
        while (br < goal && br >= spread[tc] * 8){
            shuffle(shoe, shoe + numCards, rng);
            si = 0;
            rc = 0;
            tc = 0;
            while (numCards - si > numPen && br < goal && br >= spread[tc] * 8 && tc > -1){
                if (debug) cout << "Tc: " << tc << endl;
                if (debug) cout << "rc: " << rc << endl;
                if (debug) cout << "used: " << si << endl;
                tc = rc / ((numCards - si) / 52);
                tc = min(6, tc);
                sHands++;
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
                rc += counts[newHands[1][0]] + counts[newHands[2][0]] + counts[dealer[0]] + counts[dealer[1]];
                if (debug) cout << "bet:" << bet << endl;
                for (ni = 0; ni < nn; ni++){
                    soft = 0;
                    hi = 2;
                    split = newHands[0][ni];
                    hand[0] = newHands[1][ni];
                    hand[1] = newHands[2][ni];
                    total = hand[0] + hand[1];
                    if (debug) {
                        cout << hand[0];
                        for (int i = 1; i < hi; i++) cout << "/" << hand[i];
                        cout << " vs " << dealer[0] << endl;
                    }
                    if (hand[0] == 1 || hand[1] == 1){
                        soft = 1;
                        total += 10;
                    }
                    //check blackjack
                    if (total == 21 && split == 0) {
                        if (dealer[0] == 1 && dealer[1] == 10 || dealer[0] == 10 && dealer[1] == 1){
                            if (debug) cout << "blackjack push" << endl;
                            nn = 0;
                            break;
                        }
                        br += blackJackModifier * bet;
                        if (debug) {cout << "blackjack" << endl;
                            cout << br << endl;
                        }
                        nn = 0;
                        break;
                    }
                    if (dealer[0] == 1) {
                        if (dealer[1] != 10 && tc >= 3) {
                            if (debug) cout << "insurance failed" << flush;
                            br -= 0.5 * bet;
                        }   
                        if (dealer[1] == 10 && tc >= 3) {
                            if (debug) {
                                cout << "insurance success" << endl;
                            }
                            nn = 0;
                            break;
                        } else if (tc >= 3) {
                            if (debug) cout << "failure" << endl;
                        }
                        if (dealer[1] == 10) {
                            br -= bet;
                            nn = 0;
                            if (debug) {
                                cout << "Dealer's blackjack" << endl;
                                cout << br << endl;
                            }
                            break;
                        }
                    }
                    if (dealer[0] == 10 && dealer[1] == 1){
                        br -= bet;
                        nn = 0;
                        if (debug) {
                            cout << "Dealer's blackajck" << endl;
                            cout << br << endl;
                        }
                        break;
                    }
                    if (surrender && sr[total][dealer[0]] && split == 0) {
                        br -= 0.5 * bet;
                        nn = 0;
                        break;
                    }
                    if (split < maxSplits && hand[0] == hand[1] && sp[hand[0]][dealer[0]]) {
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
                        if (debug){
                            cout << "split" << endl;
                            cout << hand[0];
                            for (int i = 1; i < hi; i++) cout << "/" << hand[i];
                            cout << " vs " << dealer[0] << endl;
                        }
                    }
                    if (doubleAfterSplit || split == 0) {
                        if (!soft && dbh[total][dealer[0]] || soft && dbs[total][dealer[0]]){
                            hand[2] = shoe[si++];
                            rc += counts[hand[2]];
                            total += hand[2];
                            if (hand[2] == 1 && total <= 11) {
                                total += 10;
                                soft = 1;
                            } else if (soft && total > 21) {
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
                    while (total < 21){
                        if (soft){
                            if (!hss[total][dealer[0]]) break;
                        } else {
                            if (!hsh[total][dealer[0]]) break;
                        }
                        hand[hi] = shoe[si++];
                        total += hand[hi];
                        if (!soft && total <= 11 && hand[hi] == 1){
                            total += 10;
                            soft = 1;
                        } else if (soft && total > 21) {
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
                for (int i = 0; i < nn; i++){
                    if (totals[0][i] > 21) {//player busts
                        br -= bet * totals[1][i];
                        if (debug){
                            cout << totals[0][i] << ": Player busts!" << endl;
                            cout << br << endl;
                        }
                    } else {
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
                                if (total <= 11 && dealer[di-1] == 1) {
                                    soft = 1;
                                    total += 10;
                                } else if (total > 21 && soft) {
                                    soft = 0;
                                    total -= 10;
                                }
                            }
                            if (debug) {
                                cout << dealer[0];
                                for (int j = 1; j < di; j++){
                                    cout << "/" << dealer[j];
                                }
                                cout << endl;
                            }
                        }
                        if (debug) cout << totals[0][i] << ":" << total << endl;
                        if (total > 21) {
                            br += bet * totals[1][i];//dealer bust
                            if (debug){
                                cout << "dealer busts" << endl;
                                cout << br << endl;
                            }
                        }
                        else if (totals[0][i] > total) {
                            br += bet * totals[1][i];
                            if (debug) {
                                cout << "player win" << endl;
                                cout << br << endl;
                            }
                        }
                        else if (totals[0][i] < total) {
                            br -= bet * totals[1][i];
                            if (debug) {
                                cout << "dealer win" << endl;
                                cout << br << endl;
                            }
                        }
                        else br = br;//push
                    }
                }
                tc = rc / ((numCards - si) / 52);
                tc = min(6, tc);
            }
        }
        if (br < bankroll) fails++;
        else hands += sHands;
    }
    //cout << "thread destroyed" << endl;
}
int main(){
    if (!s17){
        //h17 differences
    }
    unsigned int numThreads = thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 2;
    cout << numThreads << endl;
    int perThread = iterations / numThreads;
    int hands = 0;
    int fails = 0;
    int totalSpread = 0;
    int currentSpread = 1;
    int optHands = INT_MAX;
    int safeHands = INT_MAX;
    int safeFails = INT_MAX;
    int optSpread[7] = {0};
    int safeSpread[7] = {0};
    int possible = 0;
    if (debug) {
        bankroll = 1000;
        goal = 2000;
        numThreads = 2;
        perThread = iterations = 1;
        iterations = 2;
        maxBet = 1;
        minBet = 1;
    }
    for (int s6 = minBet; s6 <= maxBet; s6++){
        for (int s5 = minBet; s5 <= s6; s5++){
            for (int s4 = minBet; s4 <= s5; s4++){
                for (int s3 = minBet; s3 <= s4; s3++){
                    for (int s2 = minBet; s2 <= s3; s2++){
                        for (int s1 = minBet; s1 <= s2; s1++){
                            totalSpread++;
                        }
                    }
                }
            }
        }
    }
    vector<thread> threads;
    for (int s6 = minBet; s6 <= maxBet; s6++){
        for (int s5 = minBet; s5 <= s6; s5++){
            for (int s4 = minBet; s4 <= s5; s4++){
                for (int s3 = minBet; s3 <= s4; s3++){
                    for (int s2 = minBet; s2 <= s3; s2++){
                        for (int s1 = minBet; s1 <= s2; s1++){
                            cout << "\rSpread " << currentSpread++ << " of " << totalSpread << flush;
                            hands = fails = 0;
                            threads.clear();
                            spread[0] = minBet;
                            spread[1] = s1;
                            spread[2] = s2;
                            spread[3] = s3;
                            spread[4] = s4;
                            spread[5] = s5;
                            spread[6] = s6;
                            if (debug) {
                                spread[0] = 1;
                                spread[1] = 1;
                                spread[2] = 2;
                                spread[3] = 3;
                                spread[4] = 5;
                                spread[5] = 8;
                                spread[6] = 10;
                            }
                            for (int i = 0; i < numThreads; i++){
                                int start = i * perThread;
                                int end = i == numThreads - 1 ? iterations : start + perThread;
                                threads.emplace_back([start, end, &hands, &fails](){
                                    int threadFails = 0;
                                    int threadHands = 0;
                                    simulate(start, end, threadHands, threadFails);
                                    {
                                        lock_guard<mutex> lock(mtx);
                                        hands += threadHands;
                                        fails += threadFails;
                                    }
                                });
                            }
                            for (auto& t : threads){
                                t.join();
                            }
                            if (debug) cout << "\n" << fails << ":" << hands << endl;
                            if (fails <= iterations / 100 * riskOfRuin) {
                                possible = 1;
                                if (optHands >= hands){
                                    optHands = hands;
                                    optSpread[0] = 1;
                                    optSpread[1] = s1;
                                    optSpread[2] = s2;
                                    optSpread[3] = s3;
                                    optSpread[4] = s4;
                                    optSpread[5] = s5;
                                    optSpread[6] = s6;
                                }                                 
                            } else if (!possible) {
                                if (safeFails > fails) {
                                    safeFails = fails;
                                    safeHands = hands;
                                    safeSpread[0] = 1;
                                    safeSpread[1] = s1;
                                    safeSpread[2] = s2;
                                    safeSpread[3] = s3;
                                    safeSpread[4] = s4;
                                    safeSpread[5] = s5;
                                    safeSpread[6] = s6;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (possible) {
        cout << "\nPossible. Spread:";
        for (int i: optSpread) cout << " " << i;
        cout << "\nEV per Hand:" << (double)(goal - bankroll)/(optHands/(iterations));
    } else {
        cout << "\nNot possible. Safest spread:";
        for (int i:safeSpread) cout << " " << i;
        cout << "\nFails:" << (double)safeFails/iterations * 100 << "%";
        cout << "\nEV per Hand:" << (double)(goal - bankroll)/(safeHands/iterations);
    }
}
