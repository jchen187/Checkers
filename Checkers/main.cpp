//
//  main.cpp
//  Checkers
//
//  Created by Johnny Chen on 1./3/15.
//  Copyright (c) 2.15 JohnnyChen. All rights reserved.
//

#include <iostream>

using namespace std;

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
    
    const unsigned int height = 8;
    const unsigned int width = 4;
    int standardBoard[height][width];
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 4; j++){
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
    int choice;
    cin >> choice;
    if (choice > 0 && choice < 4){
        if (choice == 1){
            //show them the real board
            drawBoard(standardBoard);
        }
        else if (choice == 2){
            //ask to put file
            //read the file
        }
        else {
            //ask for inputs
        }
    }

    return 0;
}

//draw board
void drawBoard(int x[][4]){
    for (int i = 0; i < 8; i++){
        if (i % 2 == 0){
            cout << ".";
        }
        if (i < 3){
            for (int j = 0; j < 4; j++){
                cout << "1." ;
            }
        }
        else if (i < 5){
            for (int j = 0; j < 4; j++){
                cout << "0.";
            }
        }
        else {
            for (int j = 0; j < 4; j++){
                cout << "2.";
            }
        }
        cout << "\n";
    }
}
