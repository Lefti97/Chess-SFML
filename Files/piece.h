/*
    This header file contains the Piece class.
*/

#ifndef _PIECE_H
#define _PIECE_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "pieceTextures.h"

class Piece : public sf::Drawable{

public:

    Piece(char type='P', bool player=true, int pos=-1)
    : m_type{type}, m_player{player}, m_position{pos}
    { }

    void setPiece(char type, bool player, int pos);

    void setType(char ch){m_type = ch; setTexture();}
    char getType()       {return m_type;}

    void setPlayer(bool bl){m_player = bl; setTexture();}
    bool getPlayer()       {return m_player;}

    void setPosition(int pos){m_position = pos; move();}
    int getPosition()        {return m_position;}

    std::string toString();

private:
    sf::Sprite m_sprite;
    char m_type; //'K'=King , 'Q' = Queen , 'R' = Rook , 'B' = Bishop , 'N' = Knight , 'P' = Pawn
    bool m_player; // true == White , false == Black
    int m_position; // 0-63 board, -1 dead

    void setTexture();
    void move();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {   target.draw(m_sprite);  }
};

#endif