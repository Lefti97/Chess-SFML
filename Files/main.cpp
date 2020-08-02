/*
    This code file (main.cpp) contains the main function that runs the game
    all libraries that must be used have to be included here.
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "chessGame.h"


int main(){
    ChessGame chess(sf::Color(0xe6e6e6ff),sf::Color(0x808080ff));

    sf::RenderWindow window(sf::VideoMode(768,512), "Chess");
    window.setVerticalSyncEnabled(true);

    while(window.isOpen()){
        
        sf::Event event;

        while(window.pollEvent(event)){

            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button == sf::Mouse::Left){
                    if((0 <= event.mouseButton.x) && (event.mouseButton.x <= 512) && (0 <= event.mouseButton.y) && (event.mouseButton.y <= 512)){
                        int buttonPos{(event.mouseButton.x/64) + ((event.mouseButton.y/64) * 8)};
                                                
                        if(!chess.getSelected())
                            chess.selectPiece(buttonPos);
                        else
                            chess.moveSelected(buttonPos);
                    }
                    else if((517 <= event.mouseButton.x) && (event.mouseButton.x <= 763) && (5 <= event.mouseButton.y) && (event.mouseButton.y <= 45)){
                        chess.restart();
                    }
                }
            }
        }

        window.draw(chess);
        window.display();
    }
}