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

/*
 Player 1 = 1, 3
 Player 2 = 2, 4
 */
enum Player {p1=1, p2=2};
enum Piece {neither, p1Man, p2Man, p1King, p2King};

void createStandardBoard(int board[][width]);
void drawBoard(int x[][4]);
string removeSpaces(string input); //didnt use this
void allLegalMoves(int board[][width], char yourTurn);
void legalMovesForPiece(int board[][width], int y, int x, int player, bool jumpedOnceAlready);
int fToE(int y, int x);
int eToF(int y, int x);

//when you create a vector, you put number of elements and initial value
//if all your pieces become kings 9*4 + 3*2 max
vector<vector<int>> nonCapturingMoves(42, vector<int>(2));
vector<vector<int>> CapturingMoves;

int main(int argc, const char * argv[]) {

    //Ask if want to start from regular, some file or input own coordinates
    cout << "How would you like to start?\n"
                << "1. Start with the original board.\n"
                << "2. Input your own file.\n"
                << "3. Input your own coordinates.\n"
                << "Please choose a number from 1 - 3." << endl;
    /*
        .2.2.2.2    2222
        2.2.2.2.    2222
        .2.2.2.2    2222
        0.0.0.0.    0000
        .0.0.0.0    0000
        1.1.1.1.    1111
        .1.1.1.1    1111
        1.1.1.1.    1111
     */
    
    //Create Standard Board
    int myBoard[height][width];
    
    //Get the user input. Using char so that only the first letter matters
    char choice1;
    cin >> choice1;
    //By default you are going first
    char choice2 = '1';
    if (choice1 > '0' && choice1 < '4'){
        if (choice1 == '1'){
            //show them the real board
            createStandardBoard(myBoard);
            drawBoard(myBoard);
            cout << "Would you like to go first? (y/n)\n";
            cin >> choice2;
            choice2 = (choice2 == 'y') ? '1': '2';
            allLegalMoves(myBoard, choice2);
            
        }
        else if (choice1 == '2'){
            //ask to put file
            //read the file
//            cout << "Please enter the name of your file.\n";
//            string fileName;
//            cin >> fileName;
            
            ifstream myFile;
//            myFile.open(fileName);
            myFile.open("mysample.txt");
            if (myFile.is_open()){
                //Put contents of file into array
                for (int i = 0; i <= height; i++){
                    if (i == height){
                        myFile >> choice2;
                    }
                    else {
                        for (int j = 0; j < width; j++){
                            myFile >> myBoard[i][j];
                        }
                    }
                }
                drawBoard(myBoard);
                if (choice2 == '1'){
                    cout << "You are going first." << '\n';
                } else{
                    cout << "You are going second." << '\n';
                }
                allLegalMoves(myBoard, choice2);
                
                /*
                string line;
                for (int i = 0; i < 8; i++){
                    //read entire line
                    getline(myFile,line);
                    string lineNoSpaces = removeSpaces(line);
                    
                    cout << lineNoSpaces << "\n";
                    //remove space and update board
                }
                 */
                myFile.close();
            }
            else
                cout << "Unable to open file.\n";
            
        }
        else {
            //ask for inputs
        }
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
}

//removeSpaces from sample file
string removeSpaces(string input){
    input.erase(remove(input.begin(),input.end(),' '),input.end());
    //input.erase(remove_if(input.begin(), input.end(), isspace),input.end());
    return input;
}

//draw board
void drawBoard(int x[][width]){
    for (int i = 0; i < height; i++){
        if (i % 2 == 0){
            cout << ".";
        }
        for (int j = 0; j < width; j++){
            cout << x[i][j];
            if (j != 3){
                cout << ".";
            }
            if (j == 3 && i % 2 != 0){
                cout << ".";
            }
        }

//        if (i % 2 == 0){
//            cout << '\b ';  //Cursor moves 1 position backwards
//            cout << " ";
//        }
        cout << "\n";
    }
}

void readBoardFromFile(string name, ifstream file){
    //go throught first 8 lines of code. remove the spaces and
}

void play(){
    //1 goes up. 2 goes down
    //check to see whose turn it is
}

void allLegalMoves(int board[][width], char yourTurn){
    
    //create a list to store moves to open spots
    //create a list to store moves where you eat opponent
    //visited spots for kings
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            int whatsAtPos = board[i][j];
            //if your turn which is 1, look for a 1 or 3 in array
            if (yourTurn == '1'){
                if (whatsAtPos == p1Man || whatsAtPos ==p1King){
                    legalMovesForPiece(board, i, j, p1, false);
                }
//                if (whatsAtPos == p1King){
//                    legalMovesForKing(board, i, j, p1, false);
//                }
            }
            //look for a 2 or 4
            else {
                if (whatsAtPos == p2Man || whatsAtPos == p2King){
                    legalMovesForPiece(board, i, j, p2, false);
                }
//                if (whatsAtPos == p2King){
//                    legalMovesForKing(board, i, j, p2, false);
//                }
            }
        }
    }
    
    //if you have list with moves where you eat opponent, print the move
}


//y and x gives you the position. y is how much you go down and x is how much you go right
//player tells you whose turn it is
//the first time you call this function, jumpedOnceAlready should be false. need this because after you jump once, it's either you jump again. you cannot move just one square
void legalMovesForPiece(int board[][width], int y, int x, int player, bool jumpedOnceAlready){
    //going up or down
    int dir = (player == p1) ? p1yDir: p2yDir;
    
    //set your opponents depending on what player you are
    int opp1 = (player == p1) ? p2Man: p1Man;
    int opp2 = (player == p1) ? p2King: p1King;
    int ownKing = (player == p1) ? p1King: p2King;
    
    //if you can eat, you must eat
    bool ateOpponent = false;
    bool moved = false;
    int newY = y;
    int newX = x;
    
    if (y+2*dir >= 0 && y+2*dir < 8){
        //On Even Row
        if (y % 2 == 0){
            //if opponent to left
            if ((board[y+dir][x]== opp1 || board[y+dir][x] == opp2)&& x != 0){
                //see if blank space
                if (board[y+2*dir][x-1]==neither){
                    ateOpponent = true;
                    cout << y << x << " -> " <<  y+2*dir << x-1 << '\n';
                    moved = true;
                    newY = y+2*dir;
                    newX = x-1;
                    legalMovesForPiece(board, newY, newX, player, true);
                }
            }
            //if opponent to right
            if ((board[y+dir][x+1]== opp1 || board[y+dir][x+1] == opp2)&& x != 3){
                //see if blank space
                if (board[y+2*dir][x+1]==neither){
                    ateOpponent = true;
                    cout << y << x << " -> " <<  y+2*dir << x+1 << '\n';
                    moved = true;
                    newY = y+2*dir;
                    newX = x+1;
                    legalMovesForPiece(board, newY, newX, player, true);
                }
            }
            //if you are king check the opposite direction
            if (board[y][x]==ownKing){
                
            }
        }
        //On Odd Row
        else {
            //if opponent to left
            if (x != 0 && (board[y+dir][x-1]==opp1 || board[y+dir][x-1]==opp2)){
                if (board[y+2*dir][x-1]==neither){
                    ateOpponent = true;
                    cout << y << x << " -> " <<  y+2*dir << x-1 << '\n';
                    moved = true;
                    newY = y+2*dir;
                    newX = x-1;
                    legalMovesForPiece(board, newY, newX, player, true);
                }
            }
            //if opponent to right
            if (x != 3 && (board[y+dir][x]==opp1 || board[y+dir][x]==opp2)){
                if (board[y+2*dir][x+1]==neither){
                    ateOpponent = true;
                    cout << y << x << " -> " <<  y+2*dir << x+1 << '\n';
                    moved = true;
                    newY = y+2*dir;
                    newX = x+1;
                    legalMovesForPiece(board, newY, newX, player, true);
                }
            }
        }
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
                    cout << y << x << " -> " <<  y+dir << x << '\n';
                }
                if (x != 3 && board[y+dir][x+1]==neither){
                    moved = true;
                    newY = y+dir;
                    newX = x+1;
                    cout << y << x << " -> " <<  y+dir << x+1 << '\n';
                }
                //if you are king check the opposite direction
                if (board[y][x]==ownKing){
                    if (y-dir >= 0 && y-dir < 8){
                        if (board[y-dir][x]==neither){
                            moved = true;
                            newY = y-dir;
                            cout << y << x << " -> " <<  y-dir << x << '\n';
                        }
                        if (x != 3 && board[y-dir][x+1]==neither){
                            moved = true;
                            newY = y-dir;
                            newX = x+1;
                            cout << y << x << " -> " <<  y-dir << x+1 << '\n';
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
                    cout << y << x << " -> " <<  y+dir << x-1 << '\n';
                }
                if (board[y+dir][x]==neither){
                    moved = true;
                    newY = y+dir;
                    cout << y << x << " -> " <<  y+dir << x << '\n';
                }
                //if you are king check the opposite direction
                if (board[y][x]==ownKing){
                    if (y-dir >= 0 && y-dir < 8){
                        if (x != 0 && board[y-dir][x-1]==neither){
                            moved = true;
                            newY = y-dir;
                            newX = x-1;
                            cout << y << x << " -> " <<  y-dir << x-1 << '\n';
                        }
                        if (board[y-dir][x]==neither){
                            moved = true;
                            newY = y-dir;
                            cout << y << x << " -> " <<  y-dir << x << '\n';
                        }
                    }
                }

            }
        }
    }
    
    //Become a king
    if (newY==0 && moved == true){
        cout << "You now have a king. \n";
        //You now have a king
        //board[y][x]=0;
        //board[newY][newX] = (player = p1) ? p1King: p2King;
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
