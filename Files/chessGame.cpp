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



void ChessGame::calcPossibleMoves(){

    // LOOP for every piece
    for(int i=0; i<32; i++){

        Piece* tmpPiece;
        if(i<16)
            tmpPiece = &whitePieces[i];
        else
            tmpPiece = &blackPieces[i-16];
        tmpPiece->getPossibleMoves().clear();
        int piecePos = tmpPiece->getPosition();

        // Calculate Moves for tmpPiece by piece type
        switch (tmpPiece->getType())
        {
            case 'K':
                {// Normal King Moves
                    if((piecePos%8) != 0){
                        tmpPiece->getPossibleMoves().push_back(piecePos-1);
                    }
                    if((piecePos%8) != 7){
                        tmpPiece->getPossibleMoves().push_back(piecePos+1);
                    }
                    if((piecePos/8) != 0){
                        tmpPiece->getPossibleMoves().push_back(piecePos-8);
                    }
                    if((piecePos/8) != 7){
                        tmpPiece->getPossibleMoves().push_back(piecePos+8);
                    }
                    if(((piecePos%8) != 0) && ((piecePos/8) != 0)){
                        tmpPiece->getPossibleMoves().push_back(piecePos-9);
                    }
                    if(((piecePos%8) != 7) && ((piecePos/8) != 0)){
                        tmpPiece->getPossibleMoves().push_back(piecePos-7);
                    }
                    if(((piecePos%8) != 0) && ((piecePos/8) != 7)){
                        tmpPiece->getPossibleMoves().push_back(piecePos+7);
                    }
                    if(((piecePos%8) != 7) && ((piecePos/8) != 7)){
                        tmpPiece->getPossibleMoves().push_back(piecePos+9);
                    }
                    //TODO Castling
                    //TODO CheckMating
                }
                break;
            case 'Q':
                //TODO Normal Queen Moving


                break;
            case 'R':
                {
                    //Normal Rook Moving
                    int posCounter{1};
                    // Rook moves on X axis
                    while( ((piecePos-posCounter) >= 0) && ((piecePos/8) == ((piecePos-posCounter)/8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                        posCounter += 1;
                    }
                    posCounter = 1;
                    while( (piecePos/8) == ((piecePos+posCounter)/8) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                        posCounter += 1;
                    }
                    //Rook moves on Y axis
                    posCounter = 8;
                    while(((piecePos-posCounter) >= 0) && (posCounter < 64) && ((piecePos%8) == ((piecePos-posCounter)%8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                        posCounter += 8;
                    }
                    posCounter = 8;
                    while(((piecePos+posCounter) <= 63) && (posCounter < 64) && ((piecePos%8) == ((piecePos+posCounter)%8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                        posCounter += 8;
                    }

                    //TODO Castling

                }
                break;
            case 'B':
                //TODO Normal Bishop Moving



                break;
            case 'N':
                //TODO Normal Knight Moving



                break;
            case 'P':
                // TODO Pawn Moving Rules, Attackinh, Promotion
                if(selectedPiece->getPlayer()){

                }else{
                    
                }
                
                break;
            default:
                std::cerr << "Error piece type does not exist.\n";
                break;
        }
    }

}



void ChessGame::createMovesSquares(){

    if(selectedPiece == NULL)
        return;

    


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

    calcPossibleMoves();
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
        std::cout << "No piece on position "<< pos <<".\n";
        return selected;
    }

    std::cout << "Selected " << selectedPiece->toString();
    std::cout << "Possible moves : ";

    for(int i=0;i<selectedPiece->getPossibleMoves().size();i++)
        std::cout << selectedPiece->getPossibleMoves().at(i) << ' ';
    std::cout << '\n';

    return selected;
}



void ChessGame::moveSelected(int pos){
    bool validMove{false};

    if((selectedPiece == NULL) || !selected ) //Probably doesnt need both
        return;
    
    // Check pos with the Piece's possibleMoves
    for(int i=0;i<selectedPiece->getPossibleMoves().size();i++){
        if(pos == selectedPiece->getPossibleMoves().at(i)){
            validMove = true;
            break;
        }
    }

    if(validMove){
        selectedPiece->setPosition(pos);
        std::cout << "Moved " << selectedPiece->toString();
        calcPossibleMoves();
    }
    else{
        std::cout << "Invalid move\n";
    }
    
    selectedPiece = NULL;
    selected = false;


}