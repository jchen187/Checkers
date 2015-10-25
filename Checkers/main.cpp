//
//  main.cpp
//  Checkers
//
//  Created by Johnny Chen on 1./3/15.
//  Copyright (c) 2.15 JohnnyChen. All rights reserved.
//

#include <iostream>
#include <fstream> //reading from file

using namespace std;

const unsigned int height = 8;
const unsigned int width = 4;

void createStandardBoard(int board[][width]);
void drawBoard(int x[][4]);
string removeSpaces(string input); //didnt use this
void allLegalMoves(int board[][width], bool yourTurn);
void legalMovesForPiece(int board[][width], int y, int x);
int fToE(int y, int x);
int eToF(int y, int x);

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
    
    /*
     Player 1 = 1, 3
     Player 2 = 2, 4
     */
    int p1[2] = {1,3};
    int p2[2] = {2,4};
    
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
            allLegalMoves(myBoard, true);
            
        }
        else if (choice1 == '2'){
            //ask to put file
            //read the file
//            cout << "Please enter the name of your file.\n";
//            string fileName;
//            cin >> fileName;
            
            ifstream myFile;
//            myFile.open(fileName);
            myFile.open("sampleCheckers1.txt");
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
                board[i][j] = 2;
            }
            else if (i < 5)
                board[i][j] = 0;
            else {
                board[i][j] = 1;
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

void allLegalMoves(int board[][width], bool yourTurn){
    
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            
            //if your turn, look for a 1 or 3 in array
            if (yourTurn){
                if (board[i][j] == 1 || board[i][j] == 3){
                    legalMovesForPiece(board, i, j);
                }
            }
            //look for 2 or 4
        }
    }
}

//y and x gives you the position. y is how much you go down and x is how much you go right
void legalMovesForPiece(int board[][width], int y, int x){
    if (y % 2 == 0){
        //replace 1 with a constant depending on whose turn it is
        //if there is a blank space
        if (board[y-1][x]==0){
            cout << y << x << " -> " <<  y-1 << x << '\n';
        }
        if (x != 3 && board[y-1][x+1]==0){
            cout << y << x << " -> " <<  y-1 << x+1 << '\n';
        }
    }
    else {
        //if there is a blank space
        if (x != 0 && board[y-1][x-1]==0){
            cout << y << x << " -> " <<  y-1 << x-1 << '\n';
        }
        if (board[y-1][x]==0){
            cout << y << x << " -> " <<  y-1 << x << '\n';
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
