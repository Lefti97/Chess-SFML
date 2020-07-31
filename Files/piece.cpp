/*
    This code file contains member functions of piece.h
*/

#include "piece.h"

void Piece::setPiece(char type, bool player, int pos, bool moved){
    setType(type);
    setPlayer(player);
    setPosition(pos); //m_moved true
    setMoved(moved); // m_moved false
}

std::string Piece::toString(){
    std::string str;
    
    str += m_player?"White ":"Black ";

    switch (m_type)
    {
        case 'K':
            str += "King ";
            break;
        case 'Q':
            str += "Queen ";
            break;
        case 'R':
            str += "Rook ";
            break;
        case 'B':
            str += "Bishop ";
            break;
        case 'N':
            str += "Knight ";
            break;
        case 'P':
            str += "Pawn ";
            break;
        default:
            str += "??? ";
            break;
    }

    //str += "in position " + m_position + '\n';
    str.append("in position ");
    str += std::to_string(m_position);
    str += '\n';


    return str;

}

void Piece::move(){
    if(m_position<=-1 || 64<=m_position){
        m_position = -1;
        m_sprite.setColor(sf::Color(0x00000000));
        m_sprite.setPosition(sf::Vector2f((m_position % 8) * 64.f + 32.f, (m_position / 8) * 64.f + 32.f));        std::cout<<"Piece died.\n";
        possibleMoves.clear();
    }
    else{
        m_sprite.setPosition(sf::Vector2f((m_position % 8) * 64.f + 32.f, (m_position / 8) * 64.f + 32.f));
        m_moved = true;
    }
    return;
}

void Piece::setTexture(){
    switch (m_type)
    {
        //case '0':
        //    m_sprite.setTexture(*sf::RectangleShape().getTexture());
        case 'K':
            m_sprite.setTexture(m_player ? PieceTextures::whiteKing : PieceTextures::blackKing);
            break;
        case 'Q':
            m_sprite.setTexture(m_player ? PieceTextures::whiteQueen : PieceTextures::blackQueen);
            break;
        case 'R':
            m_sprite.setTexture(m_player ? PieceTextures::whiteRook : PieceTextures::blackRook);
            break;
        case 'B':
            m_sprite.setTexture(m_player ? PieceTextures::whiteBishop : PieceTextures::blackBishop);
            break;
        case 'N':
            m_sprite.setTexture(m_player ? PieceTextures::whiteKnight : PieceTextures::blackKnight);
            break;
        case 'P':
            m_sprite.setTexture(m_player ? PieceTextures::whitePawn : PieceTextures::blackPawn);
            break;
        default:
            std::cerr << "Error piece type does not exist.\n";
            break;
    }
    m_sprite.setOrigin(sf::Vector2f(m_sprite.getTexture()->getSize().x/2 , m_sprite.getTexture()->getSize().y/2));
    m_sprite.setScale(sf::Vector2f(0.375f,0.375f));
}