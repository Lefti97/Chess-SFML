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

void ChessGame::getPossibleMoves(){
    possibleMoves.clear();
    int pos{selectedPiece->getPosition()};

    switch (selectedPiece->getType())
    {
        case 'K':
            if((pos%8) != 0)
                possibleMoves.push_back(pos-1);
            if((pos%8) != 7)
                possibleMoves.push_back(pos+1);
            if((pos/8) != 0)
                possibleMoves.push_back(pos-8);
            if((pos/8) != 7)
                possibleMoves.push_back(pos+8);
            if(((pos%8) != 0) && ((pos/8) != 0))
                possibleMoves.push_back(pos-9);
            if(((pos%8) != 7) && ((pos/8) != 0))
                possibleMoves.push_back(pos-7);
            if(((pos%8) != 0) && ((pos/8) != 7))
                possibleMoves.push_back(pos+7);
            if(((pos%8) != 7) && ((pos/8) != 7))
                possibleMoves.push_back(pos+9);
            break;
        case 'Q':



            break;
        case 'R':




            break;
        case 'B':




            break;
        case 'N':




            break;
        case 'P':
            if(selectedPiece->getPlayer()){

            }else{
                
            }
            
            break;
        default:
            std::cerr << "Error piece type does not exist.\n";
            break;
    }

    for(int i=0;i<possibleMoves.size();i++)
        std::cout << possibleMoves.at(i) << ' ';
    std::cout << '\n';
}

ChessGame::ChessGame(sf::Color bordCol1 = sf::Color::White, sf::Color bordCol2 = sf::Color::Black)
: board(bordCol1,bordCol2) , selected{false}
{
    blackPieces[3].setPiece('K', false, 4);
    whitePieces[4].setPiece('K', true, 60); 

    
    blackPieces[0].setPiece('R', false, 7);
    blackPieces[1].setPiece('N', false, 6);
    blackPieces[2].setPiece('B', false, 5);
    
    blackPieces[4].setPiece('Q', false, 3);
    blackPieces[5].setPiece('B', false, 2);
    blackPieces[6].setPiece('N', false, 1);
    blackPieces[7].setPiece('R', false, 0);

    whitePieces[0].setPiece('R', true, 56); 
    whitePieces[1].setPiece('N', true, 57); 
    whitePieces[2].setPiece('B', true, 58); 
    whitePieces[3].setPiece('Q', true, 59); 
    
    whitePieces[5].setPiece('B', true, 61); 
    whitePieces[6].setPiece('N', true, 62); 
    whitePieces[7].setPiece('R', true, 63); 
    
    for(int i=8;i<16;i++){
        whitePieces[i].setPiece('P', true, 48 +(i-8));
        blackPieces[i].setPiece('P', false, 15 - (i-8) );
    }

}

bool ChessGame::selectPiece(int pos){

    for(int i=0; i<16; i++){
        if(whitePieces[i].getPosition() == pos){
            selectedPiece = &whitePieces[i];
            selected = true;
            break;
        }
        if(blackPieces[i].getPosition() == pos){
            selectedPiece = &blackPieces[i];
            selected = true;
            break;
        }
        selected = false;
    }

    if(!selected){
        selectedPiece = NULL;
        possibleMoves.clear();
        std::cout << "No piece there.\n";
        return selected;
    }

    getPossibleMoves();
    std::cout << "Selected " << selectedPiece->toString();
    return selected;
}

void ChessGame::moveSelected(int pos){
    bool validMove{false};

    if(selectedPiece == NULL)
        return;
    
    // Check pos with possibleMoves
    for(int i=0;i<possibleMoves.size();i++){
        if(pos == possibleMoves.at(i)){
            validMove = true;
            break;
        }
    }

    if(validMove){
        selectedPiece->setPosition(pos);
        std::cout << "Moved " << selectedPiece->toString();
    }
    else{
        std::cout << "Invalid move\n";
    }
    
    selectedPiece = NULL;
    possibleMoves.clear();
    selected = false;


}