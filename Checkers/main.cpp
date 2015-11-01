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
int myBoard[height][width];

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
//when you create a vector, you put number of elements and initial value
//if all your pieces become kings 9*4 + 3*2 max
//vector<vector<pos>> nonCapturingMoves(42, vector<pos>(2));
vector<vector<pos>> nonCapturingMoves;
vector<vector<pos>> CapturingMoves;
vector<vector<pos>> *displayedMoves;

void createStandardBoard(int board[][width]);
void drawBoard(int x[][4]);
string removeSpaces(string input); //didnt use this

void addToNonCapturingList(pos original, int newY, int newX);
void printList(vector<vector<pos>> list);
void clearList(vector<vector<pos>> list);
void allLegalMoves(int board[][width], int whoseTurn, int choice0);

void play(int whoGoesFirst, int choice0);
void legalMovesForPiece(int board[][width], int y, int x, int player, bool isKing, bool jumpedOnceAlready, vector<pos> captureVector, vector<pos> whatYouAte);
int fToE(int y, int x);
int eToF(int y, int x);

int main(int argc, const char * argv[]) {
    //Using chars for user input so that only first letter that is entered matters
    cout << "What do you want to experience?\n"
            << "1. Human vs AI\n"
            << "2. AI vs AI\n"
            << "Please choose 1 or 2." << endl;
    //string input;
    //getline(cin, input);
    int choice0;
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
    
    int choice1;
    cin >> choice1;
    while (!(choice1 == 1 || choice1 == 2)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Choose 1 or 2." << endl;
        cin >> choice1;
    }
    cout << "\n";
    
    
    //By default you are going first
    int whoGoesFirst = 1;
    
    //starting with own board
    if (choice1 == 1){
        //human vs ai
        if (choice0 == 1){
            string choice2;
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
        createStandardBoard(myBoard);
        play(whoGoesFirst, choice0);
    }

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
            for (int i = 0; i <= height; i++){
                if (i == height){
                    myFile >> whoGoesFirst;
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

//            if (whoGoesFirst == 1){
//                cout << "You are going first." << '\n';
//            } else{
//                cout << "You are going second." << '\n';
//            }
            
            play(whoGoesFirst, choice0);

            
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

void createStandardBoard(int board[][width]){
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (i < 3){
                board[i][j] = p2Man;
            }
            else if (i < 5)
                board[i][j] = 0;
            else {
                board[i][j] = p1Man;
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
void drawBoard(int x[][width]){
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

void play(int whoseTurn, int choice0){

    //check to see whose turn it is
    
    drawBoard(myBoard);
    cout << "P1 has " << numP1Pieces << endl;
    cout << "P2 has " << numP2Pieces << endl;
    
    srand ((unsigned)time(0));
    //int random = rand();
    //cout << "random number " << random;
    
    //if you can move
    while (numP1Pieces>0 && numP2Pieces>0 && !stuck ){
        //show board
        
        //show legal moves
        allLegalMoves(myBoard, whoseTurn,choice0);
        if (stuck){
            break;
        }
        
        int response;
        //if you choose human vs ai and it is your turn
        if (choice0 == 1 && whoseTurn == p1){
            //let you pick a move
            cin >> response;
            while (!(response > 0 && response <= displayedMoves->size())){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Try again: ";
                cin >> response;
            }
            cout << "\n";
        }
        else {
            //1 <-> size
            //if it is AI it picks move by itself
            response = rand() % displayedMoves->size() + 1;
            //cout << rand() % displayedMoves->size();
            cout << "AI-" << whoseTurn << " chooses move " << response << ".\n" << endl;
        }
    
        //what about removing pieces that are eaten
        //should i make another vector
        
        //need to minus one becuase the index of the last element is the size - 1
        int last = (int)(*displayedMoves)[response-1].size() - 1;
        //implement move and change board
        int y1 = (*displayedMoves)[response-1][0].y;
        int x1 = (*displayedMoves)[response-1][0].x;
        int y2 = (*displayedMoves)[response-1][last].y;
        int x2 = (*displayedMoves)[response-1][last].x;
        
        int piece = myBoard[y1][x1];

        
        //remove opponents if ate opponents
        if (displayedMoves == &CapturingMoves){
            
            for (int i = 0; i < (int)(*displayedMoves)[response-1].size() - 1; i++){
                //the opponent will between the initial and next move
                //before jump
                int yi =(*displayedMoves)[response-1][i].y;
                int xi =(*displayedMoves)[response-1][i].x;
                //after jump
                int yii = (*displayedMoves)[response-1][i+1].y;
                int xii = (*displayedMoves)[response-1][i+1].x;
                
                //y will be halfway between yi and yii
                int yRemove = (yi+yii)/2;
                //convert xi and xii to 8*8, take avg and convert back to 4*4
                int xRemove = eToF(yRemove, (fToE(yi, xi)+fToE(yii, xii))/2 );
                
                //remove the pieces
                if (myBoard[yRemove][xRemove] == p1King || myBoard[yRemove][xRemove] == p1Man){
                    numP1Pieces--;
                }
                else {
                    numP2Pieces--;
                }
                myBoard[yRemove][xRemove]=0;
            }
        }
        
        //        int y1 = nonCapturingMoves[response-1][0].y;
        //        int x1 = nonCapturingMoves[response-1][0].x;
        //
        //        int y2 = nonCapturingMoves[response-1][1].y;
        //        int x2 = nonCapturingMoves[response-1][1].x;
        
        //update board
        myBoard[y1][x1]=0;
        myBoard[y2][x2]=piece;

        //If a man reaches an end it becomes a king
        if (myBoard[y2][x2]==p1Man && y2==0){
            myBoard[y2][x2]=p1King;
            cout << "P1 now has a king. \n";
        }
        
        if (myBoard[y2][x2]==p2Man && y2==7){
            myBoard[y2][x2]=p2King;
            cout << "P2 now has a king. \n";
        }
        
        drawBoard(myBoard);
        cout << "P1 has " << numP1Pieces << endl;
        cout << "P2 has " << numP2Pieces << endl;
        
        //clear list
        CapturingMoves.clear();
        nonCapturingMoves.clear();
//        clearList(CapturingMoves);
//        clearList(nonCapturingMoves);
        
        //swtich turns
        whoseTurn = switchPlayer(whoseTurn);
    }
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
    
    
}

//shows all the legal moves that you can make without changing the board
void allLegalMoves(int board[][width], int whoseTurn, int choice0){
    
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
    //do this only if you cant make any captures
    if (!CapturingMoves.empty()){
        cout << front << " can eat "<< back << ".\n";
        printList(CapturingMoves);
        //only if you are playing
        if (choice0 == 1 && whoseTurn==p1){
            cout << "Choose one of the following moves: ";
        }
        //cout <<  CapturingMoves.size() <<"\n";
        displayedMoves = &CapturingMoves;
        
    }
    else if (!nonCapturingMoves.empty()){
        cout << front << " cannot eat " << back << ".\n";
        printList(nonCapturingMoves);
        if (choice0 == 1 && whoseTurn==p1){
            cout << "Choose one of the above moves: ";
        }
        //cout << nonCapturingMoves.size() << "\n";
        displayedMoves = &nonCapturingMoves;
    }
    else {
        //stop play function
        stuck = true;
    }
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
void legalMovesForPiece(int board[][width], int y, int x, int player, bool isKing, bool jumpedOnceAlready, vector<pos> captureVector, vector<pos> whatYouAte){
    
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
