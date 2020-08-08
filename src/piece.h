/*
    This header file contains the Piece class.
*/

#ifndef _PIECE_H
#define _PIECE_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "pieceTextures.h"

class Piece : public sf::Drawable{

public:

    Piece(char type='P', bool player=true, int pos=-1, bool moved=false)
    : m_type{type}, m_player{player},
    m_position{-1}, m_moved{true}, enPassant{-1}
    { }

    void setPiece(char type, bool player, int pos, bool moved=false);

    void setType(char ch){m_type = ch; setTexture();}
    char getType()       {return m_type;}

    void setPlayer(bool bl){m_player = bl; setTexture();}
    bool getPlayer()       {return m_player;}

    void setPosition(int pos){m_position = pos; move();}
    int getPosition()        {return m_position;}

    void setMoved(bool moved){m_moved = moved;}
    bool getMoved()           {return m_moved;}

    void setEnPassant(int x){enPassant = x;}
    int getEnPassant()      {return enPassant;}

    std::vector<int>& getPossibleMoves(){return possibleMoves;}
    std::vector<int>& getDangerMoves(){return dangerMoves;}

    std::string toString();

private:
    sf::Sprite m_sprite;
    std::vector<int> possibleMoves;
    std::vector<int> dangerMoves; // Moves that endanger opposite king

    char m_type; //'K'=King , 'Q' = Queen , 'R' = Rook , 'B' = Bishop , 'N' = Knight , 'P' = Pawn
    bool m_player; // true == White , false == Black
    int m_position; // 0-63 board, -1 dead
    int enPassant;
    bool m_moved;

    void setTexture();
    void move();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {   target.draw(m_sprite);  }
};

#endif