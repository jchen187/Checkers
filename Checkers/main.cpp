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

using namespace std;

const unsigned int height = 8;
const unsigned int width = 4;
const unsigned int p1yDir = -1; //going up
const unsigned int p2yDir = 1;

//number of pieces you start out with
int numP1Pieces = 0;
int numP2Pieces = 0;

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
void allLegalMoves(int board[][width], int yourTurn);


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
    
    //Create Standard Board
    int myBoard[height][width];
    
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
    
    if (choice0 == 1 && choice1 == 1){
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
//        if (choice2[0] == 'y'){
//            whoGoesFirst = 1;
//        }
//        else {
//            whoGoesFirst = 2;
//        }
        createStandardBoard(myBoard);
        
        //should be in another function
        drawBoard(myBoard);
        allLegalMoves(myBoard, whoGoesFirst);
        int response;
        cin >> response;

        //i still have to account for if you cant make any move and lose.
        while (!(response > 0 && response <= displayedMoves->size())){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Pick another number." << endl;
            cin >> response;
        }
        cout << "\n";
        
        //implement move and change board
        int y1 = (*displayedMoves)[response-1][0].y;
        int x1 = (*displayedMoves)[response-1][0].x;
        int y2 = (*displayedMoves)[response-1][1].y;
        int x2 = (*displayedMoves)[response-1][1].x;
        

//        int y1 = nonCapturingMoves[response-1][0].y;
//        int x1 = nonCapturingMoves[response-1][0].x;
//        
//        int y2 = nonCapturingMoves[response-1][1].y;
//        int x2 = nonCapturingMoves[response-1][1].x;
        //this should be where you actually become king
        myBoard[y1][x1]=0;
        myBoard[y2][x2]=2;
        drawBoard(myBoard);
    }
    else if (choice0 == 1 && choice1 == 2){
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
            
            drawBoard(myBoard);
            allLegalMoves(myBoard, whoGoesFirst);
            
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
    numP1Pieces = 12;
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
    cout << "     1   2   3   4   5   6   7   8  \n";
}

void readBoardFromFile(string name, ifstream file){
    //go throught first 8 lines of code. remove the spaces and
}

void play(){
    //check to see whose turn it is
    
    while (numP1Pieces>0 && numP2Pieces>0){
        //show board
        //show legal moves
        //let you pick a move
        //board changes
        //clear the lists
        //switch turns
    }
    if (numP1Pieces==0){
        //if you are playing then you have lost
        cout << "You have lost. Would you like to redeem yourself?\n";
    }
    if (numP2Pieces==0){
        //if you are playing then you have defeated it
        cout << "You have defeated the AI. Good job!\n";
    }
}

//shows all the legal moves that you can make without changing the board
void allLegalMoves(int board[][width], int yourTurn){
    
    //create a list to store moves to open spots
    //create a list to store moves where you eat opponent
    //visited spots for kings
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            int whatsAtPos = board[i][j];
            //if your turn which is 1, look for a 1 or 3 in array
            if (yourTurn == 1){
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
        cout << "You can eat your opponent. Choose one of the following moves " <<  CapturingMoves.size() <<"\n";
        displayedMoves = &CapturingMoves;
        printList(CapturingMoves);
    }
    else if (!nonCapturingMoves.empty()){
        cout << "You cannot eat your opponent. Choose one of the following moves " << nonCapturingMoves.size() << "\n";
        displayedMoves = &nonCapturingMoves;
        printList(nonCapturingMoves);
    }
    else {
        cout << "You have no moves to make. You lose.\n";
    }
    //get the user response
    //clear list
    
    clearList(CapturingMoves);
    clearList(nonCapturingMoves);
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

//after the user picks a move, we have to clear the list that has all the available moves
void clearList(vector<vector<pos>> list){
    for (int i = 0;i < list.size();i++){
        list.pop_back();
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
                //see if blank space or where your king was originally
                if (board[y+2*dir][x-1]==neither || (y+2*dir==captureVector[0].y && x-1==captureVector[0].x)){
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
                if (board[y+2*dir][x+1]==neither || (y+2*dir == captureVector[0].y && x+1==captureVector[0].x)){
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
                        if (board[y-2*dir][x-1]==neither || (y-2*dir == captureVector[0].y && x-1 == captureVector[0].x)){
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
                        if (board[y-2*dir][x+1]==neither || (y-2*dir == captureVector[0].y && x+1 == captureVector[0].x)){
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
                if (board[y+2*dir][x-1]==neither || (y+2*dir==captureVector[0].y && x-1==captureVector[0].x)){
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
                if (board[y+2*dir][x+1]==neither || (y+2*dir==captureVector[0].y && x+1 ==captureVector[0].x)){
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
                        if (board[y-2*dir][x-1]==neither || (y-2*dir==captureVector[0].y && x-1==captureVector[0].x)){
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
                        if (board[y-2*dir][x+1]==neither || (y-2*dir==captureVector[0].y && x+1 ==captureVector[0].x)){
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
        if (y+dir >= 0 && y+dir < 8){
            //on even row
            if (y % 2 == 0){
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
    
    //Become a king
    if (board[newY][newX]==p1Man && newY==0 && moved == true){
        cout << "P1 now has a king. \n";
        //You now have a king
        //board[y][x]=0;
        //board[newY][newX] = (player = p1) ? p1King: p2King;
    }
    
    if (board[newY][newX]==p2Man && newY==7 && moved == true){
        cout << "P2 now has a king. \n";
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
