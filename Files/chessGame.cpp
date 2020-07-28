/*
    This code file contains member functions of chessGame.h
*/

#include "chessGame.h"

void ChessGame::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.draw(board);
    for(int i=0;i<16;i++){
        target.draw(whitePieces[i]);
        target.draw(blackPieces[i]);
    }
}

ChessGame::ChessGame(sf::Color bordCol1 = sf::Color::White, sf::Color bordCol2 = sf::Color::Black)
: board(bordCol1,bordCol2)
{
    whitePieces[0].setPiece('R', true, 56); blackPieces[0].setPiece('R', false, 7);
    whitePieces[1].setPiece('N', true, 57); blackPieces[1].setPiece('N', false, 6);
    whitePieces[2].setPiece('B', true, 58); blackPieces[2].setPiece('B', false, 5);
    whitePieces[3].setPiece('Q', true, 59); blackPieces[3].setPiece('K', false, 4);
    whitePieces[4].setPiece('K', true, 60); blackPieces[4].setPiece('Q', false, 3);
    whitePieces[5].setPiece('B', true, 61); blackPieces[5].setPiece('B', false, 2);
    whitePieces[6].setPiece('N', true, 62); blackPieces[6].setPiece('N', false, 1);
    whitePieces[7].setPiece('R', true, 63); blackPieces[7].setPiece('R', false, 0);
    
    for(int i=8;i<16;i++){
        whitePieces[i].setPiece('P', true, 48 +(i-8));
        blackPieces[i].setPiece('P', false, 15 - (i-8) );
    }
}