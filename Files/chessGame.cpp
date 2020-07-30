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

    if((selectedPiece != NULL) && (selected)){
        for(int i=0; i<possibleMovesSquares.size();i++){
            target.draw(possibleMovesSquares.at(i));
        }
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
                    if((piecePos / 8) != 0){
                        tmpPiece->getPossibleMoves().push_back(piecePos - 8);
                        if((piecePos % 8) != 0)
                            tmpPiece->getPossibleMoves().push_back(piecePos - 9);
                        if((piecePos % 8) != 7)
                            tmpPiece->getPossibleMoves().push_back(piecePos - 7);
                    }
                    if((piecePos / 8) != 7){
                        tmpPiece->getPossibleMoves().push_back(piecePos + 8);
                        if((piecePos % 8) != 0)
                            tmpPiece->getPossibleMoves().push_back(piecePos + 7);
                        if((piecePos % 8) != 7)
                            tmpPiece->getPossibleMoves().push_back(piecePos + 9);
                    }
                    if((piecePos % 8) != 0)
                        tmpPiece->getPossibleMoves().push_back(piecePos - 1);
                    if((piecePos % 8) != 7)
                        tmpPiece->getPossibleMoves().push_back(piecePos + 1);


                    //TODO Castling
                    //TODO CheckMating
                }
                break;
            case 'Q':
                //TODO Normal Queen Moving
                {
                    int posCounter{1};
                    // Queen moves left on X axis
                    while( ((piecePos-posCounter) >= 0) && ((piecePos/8) == ((piecePos-posCounter)/8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                        posCounter += 1;
                    }
                    // Queen moves right on X axis
                    posCounter = 1;
                    while( (piecePos/8) == ((piecePos+posCounter)/8) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                        posCounter += 1;
                    }
                    //Queen moves up on Y axis
                    posCounter = 8;
                    while(((piecePos-posCounter) >= 0) && (posCounter < 64) && ((piecePos%8) == ((piecePos-posCounter)%8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                        posCounter += 8;
                    }
                    //Queen moves down on Y axis
                    posCounter = 8;
                    while(((piecePos+posCounter) <= 63) && (posCounter < 64) && ((piecePos%8) == ((piecePos+posCounter)%8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                        posCounter += 8;
                    }
                    // Queen moves towards top left
                    posCounter = 9;
                    while( ((piecePos-posCounter) >= 0) && (((piecePos-posCounter) % 8) < (piecePos % 8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                        posCounter += 9;
                    }
                    // Queen moves towards bottom right
                    posCounter = 9;
                    while( ((piecePos+posCounter) <= 63) && (((piecePos+posCounter) % 8) > (piecePos % 8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                        posCounter += 9;
                    }
                    // Queen moves towards top right
                    posCounter = 7;
                    while( ((piecePos-posCounter) >= 0) && (((piecePos-posCounter) % 8) > (piecePos % 8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                        posCounter += 7;
                    }
                    // Queen moves towards bottom left
                    posCounter = 7;
                    while( ((piecePos+posCounter) <= 63) && (((piecePos+posCounter) % 8) < (piecePos % 8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                        posCounter += 7;
                    }
                }
                break;
            case 'R':
                {
                    //Normal Rook Moving
                    int posCounter{1};
                    // Rook moves left on X axis
                    while( ((piecePos-posCounter) >= 0) && ((piecePos/8) == ((piecePos-posCounter)/8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                        posCounter += 1;
                    }
                    // Rook moves right on X axis
                    posCounter = 1;
                    while( (piecePos/8) == ((piecePos+posCounter)/8) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                        posCounter += 1;
                    }
                    // Rook moves up on Y axis
                    posCounter = 8;
                    while(((piecePos-posCounter) >= 0) && (posCounter < 64) && ((piecePos%8) == ((piecePos-posCounter)%8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                        posCounter += 8;
                    }
                    // Rook moves down on Y axis
                    posCounter = 8;
                    while(((piecePos+posCounter) <= 63) && (posCounter < 64) && ((piecePos%8) == ((piecePos+posCounter)%8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                        posCounter += 8;
                    }
                }
                break;
            case 'B':
                {
                    //Normal Bishop Moving
                    int posCounter{9};
                    // Bishop moves towards top left
                    while( ((piecePos-posCounter) >= 0) && (((piecePos-posCounter) % 8) < (piecePos % 8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                        posCounter += 9;
                    }
                    // Bishop moves towards bottom right
                    posCounter = 9;
                    while( ((piecePos+posCounter) <= 63) && (((piecePos+posCounter) % 8) > (piecePos % 8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                        posCounter += 9;
                    }
                    // Bishop moves towards top right
                    posCounter = 7;
                    while( ((piecePos-posCounter) >= 0) && (((piecePos-posCounter) % 8) > (piecePos % 8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                        posCounter += 7;
                    }
                    // Bishop moves towards bottom left
                    posCounter = 7;
                    while( ((piecePos+posCounter) <= 63) && (((piecePos+posCounter) % 8) < (piecePos % 8)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                        posCounter += 7;
                    }
                }
                break;
            case 'N':
                //TODO Normal Knight Moving
                {
                    if((piecePos / 8) != 0 ){
                        if((piecePos % 8) >= 2 )
                            tmpPiece->getPossibleMoves().push_back(piecePos - 10);
                        if( (piecePos % 8) <= 5 )
                            tmpPiece->getPossibleMoves().push_back(piecePos - 6);
                        if((piecePos / 8) != 1){
                            if((piecePos % 8) >= 1 )
                                tmpPiece->getPossibleMoves().push_back(piecePos - 17);
                            if((piecePos % 8) <= 6 )
                                tmpPiece->getPossibleMoves().push_back(piecePos - 15);
                        }
                    }
                    if((piecePos / 8) != 7){
                        if((piecePos % 8) >= 2 )
                            tmpPiece->getPossibleMoves().push_back(piecePos + 6);
                        if( (piecePos % 8) <= 5 )
                            tmpPiece->getPossibleMoves().push_back(piecePos + 10);
                        if((piecePos / 8) != 6){
                            if((piecePos % 8) >= 1 )
                                tmpPiece->getPossibleMoves().push_back(piecePos + 15);
                            if((piecePos % 8) <= 6 )
                                tmpPiece->getPossibleMoves().push_back(piecePos + 17);
                        }
                    }
                }
                break;
            case 'P':
                // TODO Pawn Moving Rules, Attackinh, Promotion
                {
                    if (tmpPiece->getPlayer()){ // If pawn is white
                        
                        if(!tmpPiece->getMoved())
                            tmpPiece->getPossibleMoves().push_back(piecePos - 16);

                        if((piecePos / 8) != 0){
                            tmpPiece->getPossibleMoves().push_back(piecePos - 8);
                            if((piecePos % 8) != 0){ // must check for black pieces
                                tmpPiece->getPossibleMoves().push_back(piecePos - 9);
                            }
                            if ((piecePos % 8) != 7){ // must check for black pieces
                                tmpPiece->getPossibleMoves().push_back(piecePos - 7);
                            }
                        }
                        else{ // MUST PROMOTE PAWN
                            std::cout << "MUST PROMOTE PAWN\n";
                        }
                        

                    }
                    else{ //if pawn is black
                        if(!tmpPiece->getMoved())
                            tmpPiece->getPossibleMoves().push_back(piecePos + 16);

                        if((piecePos / 8) != 7){
                            tmpPiece->getPossibleMoves().push_back(piecePos + 8);
                            if((piecePos % 8) != 0){ // must check for white pieces
                                tmpPiece->getPossibleMoves().push_back(piecePos + 7);
                            }
                            if ((piecePos % 8) != 7){ // must check for white pieces
                                tmpPiece->getPossibleMoves().push_back(piecePos + 9);
                            }
                        }
                        else{ // MUST PROMOTE PAWN
                            std::cout << "MUST PROMOTE PAWN\n";
                        }
                    }
                    
                    


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

    possibleMovesSquares.clear();

    for(int i=0; i<selectedPiece->getPossibleMoves().size();i++){
        sf::RectangleShape tmp;
        tmp.setPosition(sf::Vector2f((selectedPiece->getPossibleMoves().at(i) % 8) * 64.f , (selectedPiece->getPossibleMoves().at(i) / 8) * 64.f));
        tmp.setSize(sf::Vector2f(64.f, 64.f));
        tmp.setFillColor(sf::Color(0x66b4cc70));
        //tmp.setOutlineColor(sf::Color::Black);
        //tmp.setOutlineThickness(-5.f);
        possibleMovesSquares.push_back(tmp);
    }

    sf::RectangleShape tmp;
    tmp.setPosition(sf::Vector2f((selectedPiece->getPosition() % 8) * 64.f , (selectedPiece->getPosition() / 8) * 64.f));
    tmp.setSize(sf::Vector2f(64.f, 64.f));
    tmp.setFillColor(sf::Color(0x00000000));
    tmp.setOutlineColor(sf::Color::Red);
    tmp.setOutlineThickness(-3.f);
    possibleMovesSquares.push_back(tmp);

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
    std::cout << "TESTconstructor\n";

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
        possibleMovesSquares.clear();
        std::cout << "No piece on position "<< pos <<".\n";
        return selected;
    }

    std::cout << "Selected " << selectedPiece->toString();
    std::cout << "Possible moves : ";

    for(int i=0;i<selectedPiece->getPossibleMoves().size();i++)
        std::cout << selectedPiece->getPossibleMoves().at(i) << ' ';
    std::cout << '\n';

    createMovesSquares();

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