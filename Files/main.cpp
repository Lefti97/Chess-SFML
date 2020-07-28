/*
    This code file (main.cpp) contains the main function that runs the game
    all libraries that must be used have to be included here.
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "chessGame.h"


int main(){
    ChessGame chess(sf::Color(0xe6e6e6ff),sf::Color(0x808080ff));

    sf::RenderWindow window(sf::VideoMode(512,512), "The Chess");
    window.setVerticalSyncEnabled(true);

    while(window.isOpen()){
        
        sf::Event event;

        while(window.pollEvent(event)){

            if(event.type == sf::Event::Closed)
                window.close();
                
            if(event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button == sf::Mouse::Left){
                    int buttonPos{(event.mouseButton.x/64) + ((event.mouseButton.y/64) * 8)};
                    std::cout << "--Left button was Pressed.\n";
                    //std::cout << "X: " << event.mouseButton.x/64 << "\tY: " << event.mouseButton.y/64 << '\n';
                    std::cout << "Mouse Board Pos: " << buttonPos <<  '\n' ;
                    chess.checkPiecePos(buttonPos);
                }
            }
        }

        window.draw(chess);
        window.display();

    }

}