/*
    This code file (main.cpp) contains the main function that runs the game
    all libraries that must be used have to be included here.
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "chessGame.h"


int main(){
    ChessGame chess(sf::Color::White,sf::Color::Black);

    sf::RenderWindow window(sf::VideoMode(512,512), "The Chess");
    window.setVerticalSyncEnabled(true);

    while(window.isOpen()){
        
        sf::Event event;

        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
        }

        //window.clear(sf::Color::Black);
        window.draw(chess);
        window.display();

    }

}