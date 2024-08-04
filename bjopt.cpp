#include <algorithm>
#include <limits.h>
#include <chrono>
#include <iostream>
#include <random>
#include <utility>
using namespace std;
//variables
auto rng = default_random_engine{std::chrono::system_clock::now().time_since_epoch().count()};//rng for shuffling
int shoe[1000];//the shoe
int hs[2][50][200] = {0};//should hit? is soft, my total, dealer upcard
int sp[50][200] = {0};//should split?
int db[2][50][200] = {0};//should double?
int sr[2][50][200] = {0};//should surrender?
int hand[50];//player hand
int dealer[50];//dealer hand
int newHand[3][50];//stores how many splits deep, card 1, card 2
int totals[2][50];//stores finished hand total and how much is riding on said hand
int spread[7];//stores the bet spread up to true count 6
int counts[11] = {0, -1, 1, 1, 1, 1, 1, 0, 0, 0, -1};
int hi = 0;//number of cards in player hand
int di = 0;//number of cards in dealer hand
int ti = 0;//how many hands are finished
int ni = 0;//how many hands have been played
int nn = 0;//how many hands are there in total ie. from splits
int si = 0;//how many cards have been dealt
int rc = 0;//running count
int cnt = 0;//true count
int total = 0;
int soft = 0;
//game rules
int surrender = 0;
int insurance = 1;
int doubleAfterSplit = 0;
int maxSplits = 1;
int s17 = 1;
double blackJackPayOff = 1.5;
int numDeck = 6;
int penetration = 2;
int numCards = numDeck * 52;
int numPen = penetration * 52;
//simulation parameters
int iterations = 1000;
double bankroll = 1000;
double goal = 2000;
int debug = 1;
int maxBet = 1;
int minBet = 1;
int maxRisk = 1;
//functions:try to keep number of functions as low as possible
void assignArray(int a[7], int b[7]);
void shuffle();//shuffles shoe
pair<int, double> simulate();//returns number of rounds played; then the bankroll remaining after the simulation
int doShoe(double &br);//returns number of rounds played
double doRound();//returns number of bets won or lost
int doHand();//plays a single hand, returns the total of the hand
int tc();//returns true count aka running count divided by number of decks remaining
int main(){
    char c;
    cout << "debug:";
    cin >> debug;
    if (!debug) {
        cout << "Surrender allowed[y/n]:";
        cin >> c;
        surrender = c == 'y';
        cout << "Insurance allowed[y/n]:";
        cin >> c;
        insurance = c == 'y';
        cout << "Double after split allowed[y/n]:";
        cin >> c;
        doubleAfterSplit = c == 'y';
        cout << "Max number of splits allowed:";
        cin >> maxSplits;
        cout << "Dealer stands on all 17[y/n]:";
        cin >> c;
        s17 = c == 'y';
        cout << "Blackjack payoff:";
        cin >> blackJackPayOff;
        cout << "Number of decks:";
        cin >> numDeck;
        cout << "Number of decks under cut card:";
        cin >> penetration;
        cout << "Iterations:";
        cin >> iterations;
        cout << "Bankroll:";
        cin >> bankroll;
        cout << "Goal:";
        cin >> goal;
        cout << "Max bet:";
        cin >> maxBet;
        cout << "Min bet:";
        cin >> minBet;
        cout << "Max risk in %:";
        cin >> maxRisk;
    }
    for (int i = 2; i <= 21; i++){//player hand total, initialize hard counts first. Hit or stand?
        for (int j = 1; j <= 10; j++){//dealer hand total
            switch (i) {
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                hs[0][i][j] = 1;
                break;
            case 12:
                if (j == 2 && j == 3) {
                    hs[0][i][j] = 1;
                    break;
                }
            case 13:
            case 14:
            case 15:
            case 16:
                if (j == 1 || j >= 7) hs[0][i][j] = 1;
                break;
            case 17:
            case 18:
            case 19:
            case 20:
                hs[0][i][j] = 0;
            default:
                break;
            }
        }
    }
    for (int i = 12; i <= 21; i++){//player hand total, this time soft counts. Hit or stand?
        for (int j = 1; j <= 10; j++){//dealer hand total
            switch (i) {
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
                hs[1][i][j] = 1;
                break;
            case 18:
                if (j <= 8 && j != 1) {
                    hs[1][i][j] = 0;
                    break;
                }
                hs[1][i][j] = 1;
                break;
            case 19:
            case 20:
            case 21:
                hs[1][i][j] = 0;
            }
        }
    }
    for (int i = 4; i <= 21; i++){//hard counts. double down?
        for (int j = 1; j <= 10; j++){
            switch (i) {
            case 9:
                if (j <= 6 && j >= 3) db[0][i][j] = 1;
                break;
            case 10:
                if (j != 1 && j != 10){
                    db[0][i][j] = 1;
                }
                break;
            case 11:
                if (s17 || j != 1) db[0][i][j] = 1;
                break;
            default:
                break;
            }
        }
    }
    for (int i = 12; i <= 21; i++){//soft counts. double down?
        for (int j = 1; j <= 10; j++){
            switch (i) {
                case 12:
                    break;
                case 13:
                case 14:
                    if (j == 5 || j == 6) db[1][i][j] = 1;
                    break;
                case 15:
                case 16:
                    if (j == 4 || j == 5 || j == 6) db[1][i][j] = 1;
                    break;
                case 17:
                    if (j == 3 || j == 4 || j == 5 || j == 6) db[1][i][j] = 1;
                    break;
                case 18:
                    if (j == 3 || j == 4 || j == 5 || j == 6) db[1][i][j] = 1;
                    if (j == 2 && s17 == 0) db[1][i][j] = 1;
                    break;
                case 19:
                    if (j == 6 && s17 == 0) db[1][i][j] = 1;
                case 20:
                case 21:
                default:
                    break;
            }
        }
    }
    for (int i = 1; i <= 10; i++){//split? no hard or soft count required.
        for (int j = 1; j <= 10; j++){
            switch (i) {
            case 1:
                sp[i][j] = 1;
                break;
            case 2:
            case 3:
                if (j >= 4 && j <= 7) sp[i][j] = 1;
                if (doubleAfterSplit && (j == 2 || j == 3)) sp[i][j] = 1;
                break;
            case 4:
                if (doubleAfterSplit && (j == 5 || j == 6)) sp[i][j] = 1;
                break;
            case 5:
                break;
            case 6:
                if (j != 1 && j <= 6) sp[i][j] = 1;
                break;
            case 7:
                if (j != 1 && j <= 7) sp[i][j] = 1;
                break;
            case 8:
                sp[i][j] = 1;
                break;
            case 9:
                if (j != 2 && j != 10 && j != 1) sp[i][j] = 1;
                break;
            case 10:
                break;
            }
        }
    }
    sr[0][15][10] = 1;
    sr[0][16][9] = 1;
    sr[0][16][10] = 1;
    sr[0][16][1] = 1;
    sr[0][15][1] = !s17;
    if (debug) {
        int temp[7] = {1, 1, 2, 3, 5, 8, 10};
        assignArray(spread, temp);
        cout << simulate().second;
        return 0;
    }
    int optimal[7] = {0};
    int safest[7] = {0};
    int possible = 0;
    int optHands = INT_MAX;
    double avg = 0;
    int fails = 0;
    int hands = 0;
    int safeFails = INT_MAX;
    int numSpread = 0;
    for (int s6 = minBet; s6 <= maxBet; s6++){
        for (int s5 = minBet; s5 <= s6; s5++){
            for (int s4 = minBet; s4 <= s5; s4++){
                for (int s3 = minBet; s3 <= s4; s3++){
                    for (int s2 = minBet; s2 <= s3; s2++){
                        for (int s1 = minBet; s1 <= s2; s1++){
                            numSpread++;
                        }
                    }
                }
            }
        }
    }
    int currentSpread = 0;
    int tens = 0;
    for (int s6 = minBet; s6 <= maxBet; s6++){
        for (int s5 = minBet; s5 <= s6; s5++){
            for (int s4 = minBet; s4 <= s5; s4++){
                for (int s3 = minBet; s3 <= s4; s3++){
                    for (int s2 = minBet; s2 <= s3; s2++){
                        for (int s1 = minBet; s1 <= s2; s1++){
                            //cout << "Spread " << currentSpread++ << " of " << numSpread << endl;
                            currentSpread++;
                            int temp[7] = {minBet, s1, s2, s3, s4, s5, s6};
                            assignArray(spread, temp);
                            fails = 0;
                            hands = 0;
                            tens = 0;
                            for (int i = 1; i <= iterations; i++){
                                tens = i / (iterations / 10);
                                cout << "\rSpread " << currentSpread << " of " << numSpread << " [";
                                for (int j = 0; j < tens; j++) cout << '*';
                                for (int j = 0; j < 10 - tens; j++) cout << " ";
                                cout << "]";
                                cout.flush();
                                pair<int, double> res = simulate();
                                if (res.second < bankroll) {
                                    fails++;
                                    continue;
                                }
                                hands += res.first;
                            }
                            //cout << endl;
                            if (fails <= iterations/100 * maxRisk) possible = 1;
                            if (possible && fails <= iterations/100 * maxRisk){
                                if (hands < optHands){
                                    avg = (double)(goal - bankroll)/(hands/(iterations-fails));
                                    optHands = hands;
                                    assignArray(optimal, spread);
                                }
                            } else {
                                if (fails < safeFails){ 
                                    avg = (double)(goal - bankroll)/(hands/(iterations-fails));
                                    assignArray(safest, spread);
                                    safeFails = fails;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    cout << endl;
    if (possible){
        cout << "It is possible to keep risk that low. Optimum spread:";
        cout << optimal[0];
        for (int i = 1; i <= 6; i++) cout << ", " << optimal[i];
        cout << " average per hand:" << avg << endl;
    } else {
        cout << "Not possible. Safest spread:";
        cout << safest[0];
        for (int i = 1; i <= 6; i++) cout << ", " << safest[i];
        cout << " average per hand:" << avg << endl;
    }
}
void shuffle(){
    si = 0;
    for (int x = 0; x < numDeck; x++){
        for (int i = 0; i < 4; i++){
            for (int j = 1; j <= 13; j++){//aces are treated as 1 initially
                shoe[si++] = min(10, j);
            }
        }
    }
    shuffle(shoe, shoe + numCards, rng);
    si = 0;
}
int tc(){
    int ret = rc;
    if (numCards == si) return ret;
    ret /= (numCards-si)/52;
    return min(6, ret);
}
int doShoe(double &br){
    cnt = 0;
    rc = 0;
    int ret  = 0;//number of rounds
    int bet = 0;
    if (debug) cout << "shoe starting bankroll: " << br << endl;
    shuffle();
    while (cnt > -1 && br >= 8 * spread[cnt] && br <= goal && numDeck * 52 - si >= penetration * 52){
        if (debug) cout << "true count: " << cnt << ", rc: " << rc << ", cards used:" << si <<endl;
        bet = spread[cnt];
        br += bet * doRound();
        ret++;
        if (debug) cout << "bankroll after the round: " << br << endl;
        cnt = tc();
    }
    if (debug) cout << "End shoe" << endl;
    return ret;
}
pair<int, double> simulate(){
    int ret1 = 0;
    double ret2 = bankroll;
    while (ret2 > spread[cnt] * 8 && ret2 < goal){
        ret1 += doShoe(ret2);
    }
    pair<int,double> ret;
    ret.first = ret1;
    ret.second = ret2;
    return ret;
}
double doRound(){
    di = 2;//number of cards dealer will have is 2
    nn = 1;//start with one hand
    ni = 0;//haven't done any hands yet
    double ret = 0;
    int splits = 0;//how many times you've split
    newHand[0][0] = 0;
    newHand[1][0] = shoe[si++];
    rc += counts[newHand[1][0]];
    dealer[0] = shoe[si++];
    rc += counts[dealer[0]];
    newHand[2][0] = shoe[si++];
    rc += counts[newHand[2][0]];
    dealer[1] = shoe[si++];
    rc += counts[dealer[1]];
    total = 0;
    int dealerRan = 0;
    int dealerTotal = 0;
    for (ni = 0; ni < nn; ni++){
        total = 0;
        soft = 0;
        hi = 2;
        splits = newHand[0][ni];
        hand[0] = newHand[1][ni];
        hand[1] = newHand[2][ni];
        total = hand[0] + hand[1];
        if (total <= 11 && *min_element(hand, hand + 2) == 1) {
            total += 10;
            soft = 1;
        }
        if (splits == 0 && *max_element(hand, hand + 2) == 10 && *min_element(hand, hand + 2) == 1){//in case player has a blackJack
            if (debug) cout << "blackjack" << endl;
            if (*max_element(dealer, dealer + 2) == 10 && *min_element(dealer, dealer + 2) == 1) return 0;//in case dealer has a blackJack, it's a push
            return blackJackPayOff;
        }
        if (insurance && dealer[0] == 1){//check for insurance
            if (cnt >= 3) {
                if (dealer[1] == 10) return 0;
                ret -= 0.5;
            } else if (dealer[1] == 10) return -1;
        }
        if (surrender && splits == 0 && sr[soft][total][dealer[0] + dealer[1]]){
            if (debug){
                cout << hand[0] << "/" << hand[1] << " vs " << dealer[0] << "\nsurrender\n";
            }
            return -0.5;
        }
        if (splits < maxSplits && hand[0] == hand[1] && sp[hand[0]][dealer[0]]){//if the strategy is to split
            if (debug) {
                cout << hand[0] << "/" << hand[1] << " vs " << dealer[0] << "\nsplit\n";
            }
            newHand[0][ni+1] = splits + 1;
            newHand[1][ni+1] = hand[1];
            newHand[2][ni+1] = shoe[si++];
            rc += counts[newHand[2][ni+1]];
            hand[1] = shoe[si++];
            rc += counts[hand[1]];
            splits += 1;
            nn++;
        }
        if (doubleAfterSplit || splits == 0) {
            if (db[soft][total][dealer[0]]){//if double down
                if (debug) {
                    cout << hand[0] << "/" << hand[1] << " vs " << dealer[0] << "\ndoubled\n" << endl;
                }
                hand[2] = shoe[si++];
                rc += counts[hand[2]];
                total += hand[2];
                if (hand[2] == 1 && total <= 11) total += 10;
                else if (soft && total > 21) total -= 10;
                totals[0][ni] = total;
                totals[1][ni] = 2;
                if (debug) {
                    cout << hand[0] << "/" << hand[1] << "/" << hand[2] <<  " vs " << dealer[0] << endl;
                }
                continue;
            }
        }
        totals[0][ni] = doHand();
        totals[1][ni] = 1;
    }
    //check victories
    for (int i = 0; i < nn; i++){
        if (totals[0][i] > 21) {//player bust
            if (debug) cout << "Player busts" << endl;
            ret -= totals[1][i];
            continue;
        }
        if (dealerRan == 0){//dealer only runs if there is a non-bust hand
            soft = 0;
            dealerRan = 1;
            dealerTotal = dealer[0] + dealer[1];
            if (*min_element(dealer, dealer + 2) == 1 && dealerTotal <= 11) {
                dealerTotal += 10;
                soft = 1;
            }
            while (dealerTotal <= 17){ 
                if (dealerTotal == 17){
                    if (s17) break;
                    if (soft == 0) break;
                }
                dealer[di] = shoe[si++];
                rc += counts[dealer[di]];
                dealerTotal += dealer[di];
                if (dealer[di++] == 1 && dealerTotal <= 11) {
                    dealerTotal += 10;
                    soft = 1;
                } else if (dealerTotal > 21 && soft){
                    dealerTotal -= 10;
                    soft = 0;
                }
            }
            if (debug) {
                cout << "dealer: " << dealer[0];
                for (int i = 1; i < di; i++) cout << "/" << dealer[i];
                cout << endl;
            }
        }
        if (debug) cout << totals[0][i] << ":" << dealerTotal << endl;
        if (dealerTotal > 21) {
            ret += totals[1][i];//dealer bust
            if (debug) cout << "Dealer bust" << endl;
        }
        else if (dealerTotal < totals[0][i]){
            ret += totals[1][i];//player win
            if (debug) cout << "player won" << endl;
        }
        else if (dealerTotal > totals[0][i]) {
            ret -= totals[1][i];//dealer win
            if (debug )cout << "dealer won" << endl;
        }
        else {
            if (debug) cout << "push" << endl;
        }
    }
    return ret;
}
int doHand(){
    hi = 2;
    int ret = 0;
    int decision = 0;
    int soft = 0; 
    if (debug){
        cout << hand[0];
        for (int i = 1; i < hi; i++) cout << "/" << hand[i];
        cout << " vs " << dealer[0] << endl;
    }
    ret = hand[0] + hand[1];
    if (ret <= 11 && *min_element(hand, hand + hi) == 1) {
        ret += 10;
        soft = 1;
    }
    while (ret <= 21){
        decision = hs[soft][ret][dealer[0]];
        if (decision == 0) {
            if (debug) cout << "stand" << endl;
            return ret;
        }
        hand[hi] = shoe[si++];
        rc += counts[hand[hi]];
        ret += hand[hi];
        if (ret <= 11 && hand[hi++] == 1) {
            ret += 10;
            soft = 1;
        } else if (ret > 21 && soft) {
            ret -= 10;
            soft = 0;
        }
        if (debug) cout << "hit " << shoe[si-1] << endl;
        if (debug){
            cout << hand[0];
            for (int i = 1; i < hi; i++) cout << "/" << hand[i];
            cout << " vs " << dealer[0] << endl;
        }
    }
    return ret;
}
void assignArray(int a[7], int b[7]){
    for (int i = 0; i < 7; i++){
        a[i] = b[i];
    }
}
