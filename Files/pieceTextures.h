/*
    This header file contains the PieceTextures class.
*/

#ifndef _PIECE_TEXTURES_H
#define _PIECE_TEXTURES_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class PieceTextures{
public:
    static sf::Texture blackKing;
    static sf::Texture blackQueen;
    static sf::Texture blackRook;
    static sf::Texture blackKnight;
    static sf::Texture blackBishop;
    static sf::Texture blackPawn;

    static sf::Texture whiteKing;
    static sf::Texture whiteQueen;
    static sf::Texture whiteRook;
    static sf::Texture whiteKnight;
    static sf::Texture whiteBishop;
    static sf::Texture whitePawn;

    static sf::Texture loadTexture(std::string str);
};

#endif