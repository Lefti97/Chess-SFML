/*
    This header file contains th ChessGame class.
    contains the whole game.
*/

#ifndef _CHESSGAME_H
#define _CHESSGAME_H

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <iostream>
#include "board.h"
#include "piece.h"

class ChessGame: public sf::Drawable{
private:
    Board board;
    std::array<Piece, 16> whitePieces;
    std::array<Piece, 16> blackPieces;
    Piece* selectedPiece;
    std::vector<sf::RectangleShape> possibleMovesSquares;

    bool selected;
    bool playerTurn; // true = White turn, false = Black Turn
    bool whiteCheck;
    bool blackCheck;

    void createMovesSquares();

    void calcPossibleMoves();
    void calcKingMoves(Piece* tmpPiece);
    void calcQueenMoves(Piece* tmpPiece);
    void calcRookMoves(Piece* tmpPiece);
    void calcBishopMoves(Piece* tmpPiece);
    void calcKnightMoves(Piece* tmpPiece);
    void calcPawnMoves(Piece* tmpPiece);
    void calcCastling(Piece* tmpPiece);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    ChessGame(sf::Color bordCol1, sf::Color bordCol2);

    bool getSelected(){return selected;}

    bool selectPiece(int pos);

    void moveSelected(int pos);

    


};


#endif