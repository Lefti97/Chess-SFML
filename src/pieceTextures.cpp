/*
    This code file contains static member initialisations
    of pieceTextures.h
*/

#include "pieceTextures.h"

sf::Texture PieceTextures::loadTexture(std::string str){
    sf::Texture tmp;
    if (!tmp.loadFromFile(str))
        std::cout << "Error loading file\n";
    return tmp;
}

sf::Texture PieceTextures::blackKing = PieceTextures::loadTexture("../../src/Textures/b_king.png");
sf::Texture PieceTextures::blackQueen = PieceTextures::loadTexture("../../src/Textures/b_queen.png");
sf::Texture PieceTextures::blackRook = PieceTextures::loadTexture("../../src/Textures/b_rook.png");
sf::Texture PieceTextures::blackKnight = PieceTextures::loadTexture("../../src/Textures/b_knight.png");
sf::Texture PieceTextures::blackBishop = PieceTextures::loadTexture("../../src/Textures/b_bishop.png");
sf::Texture PieceTextures::blackPawn = PieceTextures::loadTexture("../../src/Textures/b_pawn.png");

sf::Texture PieceTextures::whiteKing = PieceTextures::loadTexture("../../src/Textures/w_king.png");
sf::Texture PieceTextures::whiteQueen = PieceTextures::loadTexture("../../src/Textures/w_queen.png");
sf::Texture PieceTextures::whiteRook = PieceTextures::loadTexture("../../src/Textures/w_rook.png");
sf::Texture PieceTextures::whiteKnight = PieceTextures::loadTexture("../../src/Textures/w_knight.png");
sf::Texture PieceTextures::whiteBishop = PieceTextures::loadTexture("../../src/Textures/w_bishop.png");
sf::Texture PieceTextures::whitePawn = PieceTextures::loadTexture("../../src/Textures/w_pawn.png");
