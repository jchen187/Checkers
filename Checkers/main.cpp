//
//  main.cpp
//  Checkers
//
//  Created by Johnny Chen on 1./3/15.
//  Copyright (c) 2.15 JohnnyChen. All rights reserved.
//

#include <iostream>
#include <fstream> //reading from file
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

const unsigned int height = 8;
const unsigned int width = 4;
const unsigned int p1yDir = -1; //going up
const unsigned int p2yDir = 1;

//number of pieces you start out with
int numP1Pieces = 0;
int numP2Pieces = 0;
bool stuck = false;
vector<vector<int>> myBoard;
int goodDepth = 3;

//int myBoard[height][width];

/*
 Player 1 = 1, 3
 Player 2 = 2, 4
 */
enum Player {p1=1, p2=2};
enum Piece {neither, p1Man, p2Man, p1King, p2King};

struct pos{
    int y;
    int x;
};

int choice0;
int choice1;
//by default you are going first
int whoGoesFirst = 1;
string choice2;
clock_t start;
clock_t stop;
double elapsed;
int timeLimit = 5; //

//when you create a vector, you put number of elements and initial value
//if all your pieces become kings 9*4 + 3*2 max
//vector<vector<pos>> nonCapturingMoves(42, vector<pos>(2));
vector<vector<pos>> nonCapturingMoves;
vector<vector<pos>> CapturingMoves;
vector<vector<pos>> *displayedMoves;

//void createStandardBoard(int board[][width]);
//void drawBoard(int x[][4]);

void createStandardBoard();
void drawBoard(vector<vector<int>> x);

string removeSpaces(string input); //didnt use this

//adding all the legal moves you can make into a vector
void addToNonCapturingList(pos original, int newY, int newX);
vector<pos> addToCaptureVector(pos original, int newY, int newX, vector<pos> v);
void addToCapturingList(vector<pos> v);

void printList(vector<vector<pos>> list);
void clearList(vector<vector<pos>> list);
vector<vector<pos>> allLegalMoves(vector<vector<int>> board, int whoseTurn);

vector<vector<int>> makeMove(vector<vector<int>> b, vector<vector<pos>> listOfMoves, int response);
int switchPlayer(int oldPlayer);
bool isGameOver();

void play();
void legalMovesForPiece(vector<vector<int>> board, int y, int x, int player, bool isKing, bool jumpedOnceAlready, vector<pos> captureVector, vector<pos> whatYouAte);
int fToE(int y, int x);
int eToF(int y, int x);

int main(int argc, const char * argv[]) {
    myBoard.resize(height);
    for (int i = 0; i < height; ++i)
        myBoard[i].resize(width);
    
    //Using chars for user input so that only first letter that is entered matters
    cout << "What do you want to experience?\n"
            << "1. Human vs AI\n"
            << "2. AI vs AI\n"
            << "Please choose 1 or 2." << endl;
    //string input;
    //getline(cin, input);
    cin >> choice0;
    while (!(choice0 == 1 || choice0 == 2)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Choose 1 or 2." << endl;
        cin >> choice0;
    }
    cout << "\n";

    
    /*
        .2.2.2.2    2222        +===+   +---+---+
        2.2.2.2.    2222        | @ |   | C |||||
        .2.2.2.2    2222        +===+   +---+---+
        0.0.0.0.    0000
        .0.0.0.0    0000
        1.1.1.1.    1111
        .1.1.1.1    1111
        1.1.1.1.    1111
     */
    
    //Ask if want to start from regular board or own file
    cout << "How would you like to start?\n"
    << "1. Start with the original board.\n"
    << "2. Input your own file.\n"
    << "Please choose 1 or 2." << endl;
    
    cin >> choice1;
    while (!(choice1 == 1 || choice1 == 2)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Choose 1 or 2." << endl;
        cin >> choice1;
    }
    cout << "\n";
    
    //starting with standard board
    if (choice1 == 1){
        //human vs ai
        if (choice0 == 1){
            cout << "Would you like to go first? (y/n)\n";
            
            cin >> choice2;
            while (choice2.length() != 1 || !(choice2[0] == 'y' || choice2[0] == 'n')){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Choose y or n." << endl;
                cin >> choice2;
            }
            cout << "\n";
            
            whoGoesFirst = (choice2.at(0) == 'y') ? 1: 2;
        }
        
        cout << "How many seconds should the AI get per turn?\n";
        cin >> timeLimit;
        while (!(timeLimit > 0 && timeLimit <= 20)){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Pick a valid number from 1 to 20." << endl;
        }
        cout << "\n";
        
        
        createStandardBoard();
        play();
    }
    //start with own board
    else if (choice1 == 2){
        //ask to put file
        //read the file
        cout << "Please enter the name of your file.\n";
        string fileName;
        cin >> fileName;
        
        ifstream myFile;
        myFile.open(fileName);
        if (myFile.is_open()){
            //Put contents of file into array
            for (int i = 0; i <= height+1; i++){
                if (i == height){
                    myFile >> whoGoesFirst;
                }
                else if (i == height+1){
                    myFile >> timeLimit;
                }
                else {
                    for (int j = 0; j < width; j++){
                        myFile >> myBoard[i][j];
                        if (myBoard[i][j]==p1Man || myBoard[i][j]==p1King){
                            numP1Pieces++;
                        }
                        if (myBoard[i][j]==p2Man || myBoard[i][j]==p2King){
                            numP2Pieces++;
                        }
                    }
                }
            }
            myFile.close();

            cout << endl;
            if (whoGoesFirst == 1 && choice0 == 1){
                cout << "You are going first." << '\n';
            } else{
                cout << "You are going second." << '\n';
            }
            cout << endl;
            
            play();

            
            /*
             for (int i = 1; i <= 10; i++){
             string filename ="sampleCheckers"+to_string(i)+".txt";
             cout << filename << "\n";
             }
             
             string line;
             for (int i = 0; i < 8; i++){
             //read entire line
             getline(myFile,line);
             string lineNoSpaces = removeSpaces(line);
             cout << lineNoSpaces << "\n";
             //remove space and update board
             }
             */
        }
        else
            cout << "Unable to open file.\n";
    }
    return 0;
}

void createStandardBoard(){
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (i < 3){
                myBoard[i][j] = p2Man;
            }
            else if (i < 5)
                myBoard[i][j] = 0;
            else {
                myBoard[i][j] = p1Man;
            }
        }
    }
    numP1Pieces = 12;
    numP2Pieces = 12;
}

//removeSpaces from sample file
string removeSpaces(string input){
    input.erase(remove(input.begin(),input.end(),' '),input.end());
    //input.erase(remove_if(input.begin(), input.end(), isspace),input.end());
    return input;
}

//draw board
void drawBoard(vector<vector<int>> x){
    cout << "   +---+---+---+---+---+---+---+---+\n";
    for (int i = 0; i < height; i++){
        cout << 8 - i << "  ";
        for (int j = 0; j < width; j++){
            //EVEN ROW
            if (i % 2 == 0){
                cout << "||||| ";
                if (x[i][j] != 0){
                    cout << x[i][j] << " ";
                }
                else {
                    cout << "  ";
                }
            }
            //ODD ROW
            else {
                if (x[i][j] != 0){
                    cout << "| " << x[i][j];
                }
                else{
                    cout << "|  ";
                }
                cout << " ||||";
            }
        }
//            cout << '\b ';  //Cursor moves 1 position backwards
//            cout << " ";
        cout << "|\n";
        cout << "   +---+---+---+---+---+---+---+---+\n";
    }
    cout << "     1   2   3   4   5   6   7   8  \n" << endl;
}

void readBoardFromFile(string name, ifstream file){
    //go throught first 8 lines of code. remove the spaces and
}

int scoreFromGameState(vector<vector<int>> board, int whoseTurn){
    //go through board and see how many pieces of each there are
    int numP1King = 0;
    int numP1Man = 0;
    int numP2King = 0;
    int numP2Man = 0;
    
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (board[i][j] == p1King){
                numP1King++;
            }
            if (board[i][j] == p1Man){
                numP1Man++;
            }
            if (board[i][j] == p2King){
                numP2King++;
            }
            if (board[i][j] == p2Man){
                numP2Man++;
            }
        }
    }
    int numP1Pieces2 = 5*numP1King + 2*numP1Man;
    int numP2Pieces2 = 5*numP2King + 2*numP2Man;
    
    //opponent / own pieces
    int score = (whoseTurn == p1) ? numP1Pieces2 - numP2Pieces2 : numP2Pieces2 - numP1Pieces2;
//    int score = (whoseTurn == p1) ? numP1Pieces2 : numP2Pieces2;
//    cout << "player"<< whoseTurn <<"-" <<score <<endl;
    return score;
}

int minimaxAB(vector<vector<int>> board, int depth, int player, int alpha, int beta){
    //is game over or did we reach depth
//    if (depth == 0 || isGameOver()){
//        return scoreFromGameState(board, player);
//    }
    stop = clock();
    elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
    if (timeLimit - elapsed < 0.01){
        return scoreFromGameState(board, player);
    }
    
    int bestMove = -1000;
    
//    cout << "Depth" << depth << endl;
    vector<vector<pos>> possibleMoves = allLegalMoves(board, player);
    //if allLegalMoves return nothin then you are stuck in that position or it
    if (depth == 0 ||possibleMoves.size() == 0){
        return scoreFromGameState(board, player);
    }
    
    //go through the possible moves and makemove for each, which returns a board
    for (int i=1; i <= possibleMoves.size();i++){
        vector<vector<int>> newBoard;
        newBoard = makeMove(board, possibleMoves, i);
        int score = -1 * minimaxAB(newBoard, depth-1, switchPlayer(player), -beta, - alpha);
        if (score > alpha){
            alpha = score;
            bestMove = i;
        }
        //multiple moves can have that score
        if (score == alpha){
            int randNum = rand() % 10 + 1;
            if (randNum > 5){
                alpha = score;
                bestMove = i;
            }
        }
        //prune branch
        if (alpha >= beta){
            break;
        }
    }
    
    //    cout << depth << bestScore << endl;
    if (depth == goodDepth){
        return bestMove;
    }
    else {
        return alpha;
    }
}

int minimax(vector<vector<int>> board, int depth, int player){
    /*
     Given the current board position, player-to-move, and search depth
     0.  If the search depth is 0, call an evaluator function to assign a value to given position, expressed as a positive number if the player-to-move has the better position, negative number if the player-to-move has a worse position, or zero if the position is equal, and return the value.
     1.  Generate a list of all possible legal moves
     2.  For each move:
        a.  Make the move
        b.  Recursively call this function, with the new board position, other player-to-move, and search depth - 1.
        c.  Store off the negative value of the board position (returned by the recursive call).
        d.  Unmake the move
     3.  Return the maximum of all the values you stored, along with the move that was associated with it.
     4.  Finally, after the initial call returns, make the move returned.
     */
    
    //is game over or did we reach depth
    if (depth == 0){
        cout << "you reached the end\n";
        drawBoard(board);
        return scoreFromGameState(board, player);
    }

    int bestMove = -1000;
    int bestScore = INT_MIN;
    //vector<int> scoreList;
    
    cout << "Depth" << depth << endl;
    vector<vector<pos>> possibleMoves = allLegalMoves(board, player);
    //go throught the possible moves and makemove for each, which returns a board

    for (int i=1; i <= possibleMoves.size();i++){
        vector<vector<int>> newBoard;
        newBoard = makeMove(board, possibleMoves, i);
        drawBoard(newBoard);
        int score = -1 * minimax(newBoard, depth-1, switchPlayer(player));
        if (score > bestScore){
            bestScore = score;
            bestMove = i;
        }
    }
    
//    cout << depth << bestScore << endl;
    if (depth == goodDepth){
        return bestMove;
    }
    else {
        return bestScore;
    }
}



int switchPlayer(int oldPlayer){
    int newPlayer;
    if (oldPlayer == p1){
        newPlayer = p2;
    }
    else if (oldPlayer == p2){
        newPlayer = p1;
    }
    return newPlayer;
}

//give a board after the move
vector<vector<int>> makeMove(vector<vector<int>> b, vector<vector<pos>> listOfMoves, int response){
    
    
    //need to minus one becuase the index of the last element is the size - 1
    int last =(int)(listOfMoves)[response-1].size() -1;
    //implement move and change board
    int y1 = (listOfMoves)[response-1][0].y;
    int x1 = (listOfMoves)[response-1][0].x;
    int y2 = (listOfMoves)[response-1][last].y;
    int x2 = (listOfMoves)[response-1][last].x;
    
    for (int i = 0; i < last; i++){
        //the opponent will between the initial and next move
        //before jump
        int yi =(listOfMoves)[response-1][i].y;
        int xi =(listOfMoves)[response-1][i].x;
        //after jump
        int yii = (listOfMoves)[response-1][i+1].y;
        int xii = (listOfMoves)[response-1][i+1].x;
        
        if (abs(yii-yi)==2){
            //y will be halfway between yi and yii
            int yRemove = (yi+yii)/2;
            //convert xi and xii to 8*8, take avg and convert back to 4*4
            int xRemove = eToF(yRemove, (fToE(yi, xi)+fToE(yii, xii))/2 );
            /*
            //remove the pieces
            if (b[yRemove][xRemove] == p1King || b[yRemove][xRemove] == p1Man){
                numP1Pieces--;
            }
            else {
                numP2Pieces--;
            }
             */
            b[yRemove][xRemove]=0;
        }
    }
    int piece = b[y1][x1];
    
    //update board
    b[y1][x1]=0;
    b[y2][x2]=piece;
    
    //If a man reaches an end it becomes a king
    if (b[y2][x2]==p1Man && y2==0){
        b[y2][x2]=p1King;
//        cout << "P1 now has a king. \n";
    }
    
    if (b[y2][x2]==p2Man && y2==7){
        b[y2][x2]=p2King;
//        cout << "P2 now has a king. \n";
    }
    
    return b;
}

void wait ( int seconds )
{
    clock_t endwait;
    endwait = clock () + seconds * CLOCKS_PER_SEC ;
    while (clock() < endwait) {}
}

//this is for the actual board when you are actually shown the board
void updateNumberOfPieces(vector<vector<int>> board){
    numP1Pieces = 0;
    numP2Pieces = 0;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (board[i][j] == p1King || board[i][j] == p1Man){
                numP1Pieces++;
            }
            if (board[i][j] == p2King || board[i][j] == p2Man){
                numP2Pieces++;
            }
        }
    }
}

bool isGameOver(){
    bool isOver = false;
    if (numP1Pieces<=0 || numP2Pieces<=0 || stuck){
        isOver = true;
    }
    return isOver;
}

void play(){

    //check to see whose turn it is
    int whoseTurn = whoGoesFirst;
    
    drawBoard(myBoard);
    cout << "P1 has " << numP1Pieces << endl;
    cout << "P2 has " << numP2Pieces << endl;
    
    srand ((unsigned)time(0));
    
    //if you can move
//    while (numP1Pieces>0 && numP2Pieces>0 && !stuck ) {
    
    while (!isGameOver()){
        
        //show legal moves
        vector<vector<pos>> movesToDisplay = allLegalMoves(myBoard, whoseTurn);
        if (movesToDisplay.size()==0){
            stuck = true;
            break;
        }
        
        string front;
        string back;
        //you vs ai
        if (choice0 == 1){
            front = (whoseTurn == p1) ? "You" : "AI-2";
            back = (whoseTurn == p1) ? "your opponent AI-2":"you";
        }
        //ai vs ai
        else {
            front = (whoseTurn == p1) ? "AI-1" : "AI-2";
            back = (whoseTurn == p1) ? "opponent AI-2":"opponent AI-1";
        }
        if (abs(movesToDisplay[0][0].y - movesToDisplay[0][1].y) == 2){
            cout << front << " can eat "<< back << ".\n";
        }
        else{
            cout << front << " cannot eat "<< back << ".\n";
        }
        printList(movesToDisplay);
        if (choice0 == 1 && whoseTurn==p1){
            cout << "Choose one of the above moves: ";
        }
        
        int response = 0;
        //if you choose human vs ai and it is your turn
        if (choice0 == 1 && whoseTurn == p1){
            //let you pick a move
            cin >> response;
            while (!(response > 0 && response <= movesToDisplay.size())){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Try again: ";
                cin >> response;
            }
            cout << "\n";
        }
        else {
            //time limit + minimax +alpha beta pruning

            //1 <-> size
            //if it is AI it picks move by itself
            //response = rand() % movesToDisplay.size() + 1;
            if (movesToDisplay.size() == 1){
                response = 1;
            }
            else {
                //int best = minimax(myBoard, goodDepth, whoseTurn);
                
                start = clock();
                //iterative deepening
                for (int d = 0; d < 20; d++){
                    stop = clock();
                    
                    
                    
                    
                    goodDepth = d;
                    int alpha = INT_MIN;
                    int beta = INT_MAX;
                    //minimaxab will return the best move
                    int best2 = minimaxAB(myBoard, goodDepth, whoseTurn, alpha, beta);
                    response = best2;
                    
                    if (timeLimit - elapsed < 0.01){
                        break;
                    }
                    
                    //check to see if you have enough time
                }
            }
            //clock_t stop = clock();
            //double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
            
            cout << "Time elapsed in sec: " << elapsed << endl;
            cout << "Depth reached: " << goodDepth << endl;
            //just to silence warning and in case not set
            if (response==0){
                response = rand() % movesToDisplay.size() + 1;
            }
            
            //cout << rand() % displayedMoves->size();
            cout << "AI-" << whoseTurn << " chooses move " << response << ".\n" << endl;
        }
    
        //update board after making a move
        myBoard = makeMove(myBoard, movesToDisplay, response);
        
        drawBoard(myBoard);
        updateNumberOfPieces(myBoard);
        cout << "P1 has " << numP1Pieces << endl;
        cout << "P2 has " << numP2Pieces << endl;
        
        //swtich turns
        whoseTurn = switchPlayer(whoseTurn);
    }
    
    //END GAME CONDITIONS
    if (numP1Pieces==0){
        //if you are playing then you have lost
        if (choice0 == 1){
            cout << "You have lost to AI-2.\n";
        }
        else {
            cout << "AI-1 has lost to AI-2\n";
        }
    }
    if (numP2Pieces==0){
        //if you are playing then you have defeated it
        if (choice0 == 1){
            cout << "You have defeated the AI-2. Good job!\n";
        }
        else {
            cout << "AI-1 has defeated AI-2\n";
        }
    }
    if (numP1Pieces > 0 && numP2Pieces > 0 && stuck){
        if (choice0 == 1 && whoseTurn == 1){
            cout << "You are stuck and cannot make anymore moves. Game over!\n";
        }
        else if (choice0 == 2) {
            cout << "AI-" << whoseTurn <<" is stuck and cannot make anymore moves.\n";
        }
        
    }
    
    //print score
    cout << "P1 Score = " << scoreFromGameState(myBoard, p1) << "\n";
    cout << "P2 Score = " << scoreFromGameState(myBoard, p2) << "\n";
    
    
}

//should return a list of moves that you can make
//shows all the legal moves that you can make without changing the board
vector<vector<pos>> allLegalMoves(vector<vector<int>> board, int whoseTurn){
    
    vector<vector<pos>> movesToDisplay;
    
    //create a list to store moves to open spots
    //create a list to store moves where you eat opponent
    //visited spots for kings
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            int whatsAtPos = board[i][j];
            //if your turn which is 1, look for a 1 or 3 in array
            if (whoseTurn == 1){
                if (whatsAtPos == p1Man){
                    //the first bool is to tell you if you are a king or not
                    //it is set to false because havent jump at all yet
                    //a is to keep track of plays you have stepped on
                    //b is to help kings ONLY keep track of what they ate, so no repeats
                    vector<pos> a, b;
                    legalMovesForPiece(board, i, j, p1, false, false, a, b);
                }
                else if (whatsAtPos == p1King){
                    vector<pos> a, b;
                    legalMovesForPiece(board, i, j, p1, true, false, a, b);
                }
            }
            //look for a 2 or 4
            else {
                if (whatsAtPos == p2Man){
                    vector<pos> a, b;
                    legalMovesForPiece(board, i, j, p2, false, false, a, b);
                }
                else if (whatsAtPos == p2King){
                    vector<pos> a, b;
                    legalMovesForPiece(board, i, j, p2, true, false, a, b);
                }
            }
        }
    }
    
    
    //do this only if you cant make any captures
    if (!CapturingMoves.empty()){
        displayedMoves = &CapturingMoves;
        movesToDisplay = CapturingMoves;
    }
    else if (!nonCapturingMoves.empty()){
        displayedMoves = &nonCapturingMoves;
        movesToDisplay = nonCapturingMoves;
    }
    else {
        //you have no moves. problem is when you are at a lower level and get stuck and ends game
        
        //stop play function
//        if (myBoard == board){
//            stuck = true;
//        }
    }
    
    //clear list
    CapturingMoves.clear();
    nonCapturingMoves.clear();
    //        clearList(CapturingMoves);
    //        clearList(nonCapturingMoves);
    
    return movesToDisplay;
}


void addToNonCapturingList(pos original, int newY, int newX){
    pos newPos;
    newPos.y = newY;
    newPos.x = newX;
    //cout << original.y << original.x << "->" << newPos.y << newPos.x << '\n';
    vector<pos> v;
    v.push_back(original);
    v.push_back(newPos);
//    cout << v[0].y << v[0].x << " ";
//    cout << v[1].y << v[1].x << "\n";
    nonCapturingMoves.push_back(v);
}

vector<pos> addToCaptureVector(pos original, int newY, int newX, vector<pos> v){
    //can also have done v.empty()
    if (v.size() == 0){
        v.push_back(original);
    }
    pos newPos;
    newPos.y = newY;
    newPos.x = newX;
    v.push_back(newPos);
    return v;
}

//once capture vector is fully updated(when you cant jump anymore), add it to the list
void addToCapturingList(vector<pos> v){
    CapturingMoves.push_back(v);
}

void printList(vector<vector<pos>> list){
    for (int i = 0;i < list.size();i++){
        //Give the number
        cout << i+1 << ". ";
        for (int j = 0; j < list[i].size();j++){
            //take coordinates from 8x4 and convert to 8x8
            int x = 1 + fToE(list[i][j].y, list[i][j].x);
            int y = 8 - list[i][j].y;
            
            cout << "(" << x << "," << y << ")";

            //what i had originally
//            int y2 = list[i][j].y;
//            int x2 = list[i][j].x;
//            cout << x2 << y2;
            if (j < list[i].size() - 1 ){
                cout << " -> ";
            }
            else {
                cout << "\n";
            }
        }
    }
}

//not working right now so use given clear() function
//after the user picks a move, we have to clear the list that has all the available moves
void clearList(vector<vector<pos>> *list){
    for (int i = 0;i < displayedMoves->size();i++){
        //list->pop_back();
    }
}

//kings need to keep track of what pieces they ate and where
vector<pos> addToWhatYouAte(int y, int x, vector<pos> v){
    pos newPos;
    newPos.y = y;
    newPos.x = x;
    v.push_back(newPos);
    return v;
}
//checks if position is already in
bool isPosInVector(int y, int x, vector<pos> v){
    bool found = false;
    for (int i = 0; i < v.size(); i++){
        if (v[i].y == y && v[i].x == x){
            found = true;
        }
    }
    return found;
}

//y and x gives you the position. y is how much you go down and x is how much you go right
//player tells you whose turn it is
//the first time you call this function, jumpedOnceAlready should be false. need this because after you jump once, it's either you jump again. you cannot move just one square
//we have a capturevector to keep track of all the positions that we have been on.
void legalMovesForPiece(vector<vector<int>> board, int y, int x, int player, bool isKing, bool jumpedOnceAlready, vector<pos> captureVector, vector<pos> whatYouAte){
    
    pos currentPos;
    currentPos.y = y;
    currentPos.x = x;
    
    //going up or down
    int dir = (player == p1) ? p1yDir: p2yDir;
    
    //set your opponents depending on what player you are
    int opp1 = (player == p1) ? p2Man: p1Man;
    int opp2 = (player == p1) ? p2King: p1King;
    int ownKing = (player == p1) ? p1King: p2King;
    
    //if you can eat, you must eat
    bool ateOpponent = false;
    bool moved = false;
    
    //your new coordinates if you have
    int newY = y;
    int newX = x;
    
    //newVector will include all the pos that you have been on
    vector<pos> newVector;
    //I can keep captureVector and remove oldvector
    vector<pos> oldVector = captureVector;
    
    //kings have to keep track of the positions of the enemies they eat because they
    vector<pos> newWhatYouAte;
    vector<pos> oldWhatYouAte = whatYouAte;
    
    //can you even make a jump?
    //if you are on row 0, it automatically assumes you cant jump at all bc 0 -2 is < 0
    
    //On Even Row
    if (y % 2 == 0){
        if (y+2*dir >= 0 && y+2*dir < 8){
            //is there an opponent to your left?
            //are you not on the left border?
            //did you eat this already?
            if ( (board[y+dir][x]==opp1 || board[y+dir][x]==opp2) && x!=0 && !isPosInVector(y+dir, x, whatYouAte)){
                //see if blank space or where your king was originally if you already jumped
                //you check to see where king not on your first jump
                if (board[y+2*dir][x-1]==neither || (jumpedOnceAlready&& y+2*dir==captureVector[0].y && x-1==captureVector[0].x)){
                    ateOpponent = true;
                    /*
                     if (!isPosInVector(y+dir, x, whatYouAte)){
                     newWhatYouAte = addToWhatYouAte(y+dir, x, whatYouAte);
                     newVector = addToCaptureVector(currentPos, newY, newX, oldVector);
                     legalMovesForPiece(board, newY, newX, player, true, newVector, newWhatYouAte);
                     }
                     */
                    moved = true;
                    newY = y+2*dir;
                    newX = x-1;
                    newWhatYouAte = addToWhatYouAte(y+dir, x, whatYouAte);
                    newVector = addToCaptureVector(currentPos, newY, newX, oldVector);
                    legalMovesForPiece(board, newY, newX, player, isKing, true, newVector, newWhatYouAte);
                }
            }
            //is there an opponent to your right?
            //are you not on the right border?
            //did you eat this already?
            if ((board[y+dir][x+1]==opp1 || board[y+dir][x+1]==opp2)&& x!=3 && !isPosInVector(y+dir, x+1, whatYouAte)){
                //see if blank space or where king was originally
                if (board[y+2*dir][x+1]==neither || (jumpedOnceAlready&& y+2*dir == captureVector[0].y && x+1==captureVector[0].x)){
                    ateOpponent = true;
                    moved = true;
                    newY = y+2*dir;
                    newX = x+1;
                    newWhatYouAte = addToWhatYouAte(y+dir, x+1, whatYouAte);
                    newVector = addToCaptureVector(currentPos, newY, newX, oldVector);
                    legalMovesForPiece(board, newY, newX, player, isKing, true, newVector, newWhatYouAte);
                }
            }
        }
        //If you are a king then you HAVE TO CHECK THE OTHER DIRECTION
        
        //if this is first move and your position has a king
        //you made a jump already and your position is a blank
        //if (board[y][x]==ownKing || (isKing && jumpedOnceAlready && board[y][x]==neither)){
        if (isKing){
            if (y-2*dir >= 0 && y-2*dir < 8){
                //if opponent is to the left
                
                if ((board[y-dir][x] == opp1 || board[y-dir][x] == opp2) && x != 0 && !isPosInVector(y-dir, x, whatYouAte)){
                    //check for blank space or if original place of king
                    if (board[y-2*dir][x-1]==neither || (jumpedOnceAlready&& y-2*dir == captureVector[0].y && x-1 == captureVector[0].x)){
                        ateOpponent = true;
                        moved = true;
                        newY = y-2*dir;
                        newX = x-1;
                        newWhatYouAte = addToWhatYouAte(y-dir, x, whatYouAte);
                        newVector = addToCaptureVector(currentPos, newY, newX, oldVector);
                        legalMovesForPiece(board, newY, newX, player, isKing, true, newVector, newWhatYouAte);
                    }
                }
                //if opponent to right
                if ((board[y-dir][x+1]== opp1 || board[y-dir][x+1] == opp2)&& x != 3 && !isPosInVector(y-dir, x+1, whatYouAte)){
                    //see if blank space or where original king is
                    if (board[y-2*dir][x+1]==neither || (jumpedOnceAlready&& y-2*dir == captureVector[0].y && x+1 == captureVector[0].x)){
                        ateOpponent = true;
                        moved = true;
                        newY = y-2*dir;
                        newX = x+1;
                        newWhatYouAte = addToWhatYouAte(y-dir, x+1, whatYouAte);
                        newVector = addToCaptureVector(currentPos, newY, newX, oldVector);
                        legalMovesForPiece(board, newY, newX, player, isKing, true, newVector, newWhatYouAte);
                    }
                }
            }
        }
    }
    //On Odd Row
    else {
        if (y+2*dir >= 0 && y+2*dir < 8){
            //are you not on the left border
            //if opponent to left
            //did you eat already
            if (x!=0 && (board[y+dir][x-1]==opp1 || board[y+dir][x-1]==opp2) && !isPosInVector(y+dir, x-1, whatYouAte)){
                if (board[y+2*dir][x-1]==neither || (jumpedOnceAlready&& y+2*dir==captureVector[0].y && x-1==captureVector[0].x)){
                    ateOpponent = true;
                    moved = true;
                    newY = y+2*dir;
                    newX = x-1;
                    newWhatYouAte = addToWhatYouAte(y+dir, x-1, whatYouAte);
                    newVector = addToCaptureVector(currentPos, newY, newX, oldVector);
                    legalMovesForPiece(board, newY, newX, player, isKing, true, newVector, newWhatYouAte);
                }
            }
            //if opponent to right
            if (x!=3 && (board[y+dir][x]==opp1 || board[y+dir][x]==opp2) && !isPosInVector(y+dir, x, whatYouAte)){
                if (board[y+2*dir][x+1]==neither || (jumpedOnceAlready&& y+2*dir==captureVector[0].y && x+1 ==captureVector[0].x)){
                    ateOpponent = true;
                    moved = true;
                    newY = y+2*dir;
                    newX = x+1;
                    newWhatYouAte = addToWhatYouAte(y+dir, x, whatYouAte);
                    newVector = addToCaptureVector(currentPos, newY, newX, oldVector);
                    legalMovesForPiece(board, newY, newX, player, isKing, true, newVector, newWhatYouAte);
                    
                }
            }
        }
            if (isKing){
                if (y-2*dir >= 0 && y-2*dir < 8){
                    //if opponent on left
                    if (x!=0 && (board[y-dir][x-1]==opp1 || board[y-dir][x-1]==opp2) && !isPosInVector(y-dir, x-1, whatYouAte)){
                        if (board[y-2*dir][x-1]==neither || (jumpedOnceAlready&& y-2*dir==captureVector[0].y && x-1==captureVector[0].x)){
                            ateOpponent = true;
                            moved = true;
                            newY = y-2*dir;
                            newX = x-1;
                            newWhatYouAte = addToWhatYouAte(y-dir, x-1, whatYouAte);
                            newVector = addToCaptureVector(currentPos, newY, newX, oldVector);
                            legalMovesForPiece(board, newY, newX, player, isKing, true, newVector, newWhatYouAte);
                        }
                    }
                    //if opponent to right
                    if (x != 3 && (board[y-dir][x]==opp1 || board[y-dir][x]==opp2) && !isPosInVector(y-dir, x, whatYouAte)){
                        if (board[y-2*dir][x+1]==neither || (jumpedOnceAlready&& y-2*dir==captureVector[0].y && x+1 ==captureVector[0].x)){
                            ateOpponent = true;
                            moved = true;
                            newY = y-2*dir;
                            newX = x+1;
                            newWhatYouAte = addToWhatYouAte(y-dir, x, whatYouAte);
                            newVector = addToCaptureVector(currentPos, newY, newX, oldVector);
                            legalMovesForPiece(board, newY, newX, player, isKing, true, newVector, newWhatYouAte);
                        }
                    }
                }
            }
        }
        
    
    //you jumped once already but you cannot eat anymore
    if (jumpedOnceAlready && !moved){
        addToCapturingList(oldVector);
    }
    
    
    //You cannot eat an opponent but you might be still able to move one space ahead
    if (!jumpedOnceAlready && !ateOpponent){
        
        //on even row
        if (y % 2 == 0){
            if (y+dir >= 0 && y+dir < 8){
                //if there is a blank space. MAKE SURE DONT GO OUT OF BOUND
                if (board[y+dir][x]==neither){
                    moved = true;
                    newY = y+dir;
                    newX = x;
                    addToNonCapturingList(currentPos,newY,newX);
                }
                if (x != 3 && board[y+dir][x+1]==neither){
                    moved = true;
                    newY = y+dir;
                    newX = x+1;
                    addToNonCapturingList(currentPos,newY,newX);
                }
            }
            //if you are king check the opposite direction
            if (board[y][x]==ownKing){
                if (y-dir >= 0 && y-dir < 8){
                    if (board[y-dir][x]==neither){
                        moved = true;
                        newY = y-dir;
                        newX = x;
                        addToNonCapturingList(currentPos,newY,newX);
                    }
                    if (x != 3 && board[y-dir][x+1]==neither){
                        moved = true;
                        newY = y-dir;
                        newX = x+1;
                        addToNonCapturingList(currentPos,newY,newX);
                    }
                }
            }
        }
        
        
        //on odd row
        else {
            if (y+dir >= 0 && y+dir < 8){
                //if there is a blank space
                if (x != 0 && board[y+dir][x-1]==neither){
                    moved = true;
                    newY = y+dir;
                    newX = x-1;
                    addToNonCapturingList(currentPos,newY,newX);
                }
                if (board[y+dir][x]==neither){
                    moved = true;
                    newY = y+dir;
                    newX = x;
                    addToNonCapturingList(currentPos,newY,newX);
                }
            }
            //if you are king check the opposite direction
            if (board[y][x]==ownKing){
                if (y-dir >= 0 && y-dir < 8){
                    if (x != 0 && board[y-dir][x-1]==neither){
                        moved = true;
                        newY = y-dir;
                        newX = x-1;
                        addToNonCapturingList(currentPos,newY,newX);
                    }
                    if (board[y-dir][x]==neither){
                        moved = true;
                        newY = y-dir;
                        newX = x;
                        addToNonCapturingList(currentPos,newY,newX);
                    }
                }
            }
        }
        
        
    }
}

//change the x coordinate depending on whether you want 8*4 or 8*8 FOUR to EIGHT
int fToE(int y, int x){
    if (y % 2 == 0){
        x = x*2 + 1;
    }
    else {
        x *= 2;
    }
    return x;
}
int eToF(int y, int x) {
    if (y % 2 == 0){
        x = (x-1)/2;
    }
    else {
        x /= 2;
    }
    return x;
}


