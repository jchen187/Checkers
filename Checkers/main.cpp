//
//  main.cpp
//  Checkers
//
//  Created by Johnny Chen on 10/3/15.
//  Copyright (c) 2015 JohnnyChen. All rights reserved.
//

#include <iostream>

using namespace std;

int main(int argc, const char * argv[]) {

    //Ask if want to start from regular, some file or input own coordinates
    cout << "How would you like to start?\n"
                << "1. Start with the original board.\n"
                << "2. Input your own file.\n"
                << "3. Input your own coordinates.\n"
                << "Please choose a number from 1 - 3." << endl;
    //Get the user input. Must be a number
    int choice;
    cin >> choice;
    if (choice > 0 && choice < 4){
        if (choice == 1){
            
        }
        else if (choice == 2){
            
        }
        else {
            //ask for inputs
        }
    }

    return 0;
}

//draw board
