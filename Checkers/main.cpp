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

void drawBoard(int x[][4]);


int main(int argc, const char * argv[]) {

    //Ask if want to start from regular, some file or input own coordinates
    cout << "How would you like to start?\n"
                << "1. Start with the original board.\n"
                << "2. Input your own file.\n"
                << "3. Input your own coordinates.\n"
                << "Please choose a number from 1 - 3." << endl;
    /*
        .1.1.1.1    1111
        1.1.1.1.    1111
        .1.1.1.1    1111
        0.0.0.0.    0000
        .0.0.0.0    0000
        2.2.2.2.    2222
        .2.2.2.2    2222
        2.2.2.2.    2222
     */
    
    int standardBoard[height][width];
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (i < 3){
                standardBoard[i][j] = 1;
            }
            else if (i < 5)
                standardBoard[i][j] = 0;
            else {
                standardBoard[i][j] = 2;
            }
        }
    }
    
    //Get the user input. Must be a number
    int choice1;
    cin >> choice1;
    if (choice1 > 0 && choice1 < 4){
        if (choice1 == 1){
            //show them the real board
            drawBoard(standardBoard);
            cout << "Would you like to go first? (y/n)\n";
            char choice2;
            cin >> choice2;
            //take only one letter
            //cout << choice2;
        }
        else if (choice1 == 2){
            //ask to put file
            //read the file
//            cout << "Please enter the name of your file.\n";
//            string fileName;
//            cin >> fileName;
            
            ifstream myFile;
//            myFile.open(fileName);
            myFile.open("sampleCheckers1.txt");
            if (myFile.is_open()){
                string line;
                for (int i = 0; i < 8; i++){
                    getline(myFile,line);
                    cout << line << "\n";
                }
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

void readBoardFromFile(){
    //go throught first 8 lines of code. remove the spaces and
}

void play(){
    //1 goes up. 2 goes down
    //check to see whose turn it is
}

void legalMoves(int board[][width], bool yourTurn){
    
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; i++){
            //if your turn, look for a 2 or 4 in array
            if (yourTurn){
                if (board[i][j] == 2 || board[i][j] == 4){
                    
                }
            }
            //look for 1 or 3
        }
    }
    //else look for 1 or 3
}
