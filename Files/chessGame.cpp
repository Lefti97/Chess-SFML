/*
    This code file contains member functions of chessGame.h
*/

#include "chessGame.h"


ChessGame::ChessGame(sf::Color bordCol1 = sf::Color::White, sf::Color bordCol2 = sf::Color::Black)
: board(bordCol1,bordCol2) , selected{false} , playerTurn{true} , playerTurnCheck{false} , mate{false}
{
    // The code is taking account of these indexes.
    // Changing them may brake normal chess rules.
    // Comment out pieces if you want to remove some pieces at beggining.

    blackPieces[0].setPiece('R', false, 7);
    blackPieces[1].setPiece('N', false, 6);
    blackPieces[2].setPiece('B', false, 5);
    blackPieces[3].setPiece('K', false, 4);
    blackPieces[4].setPiece('Q', false, 3);
    blackPieces[5].setPiece('B', false, 2);
    blackPieces[6].setPiece('N', false, 1);
    blackPieces[7].setPiece('R', false, 0);

    whitePieces[0].setPiece('R', true, 56); 
    whitePieces[1].setPiece('N', true, 57); 
    whitePieces[2].setPiece('B', true, 58); 
    whitePieces[3].setPiece('Q', true, 59); 
    whitePieces[4].setPiece('K', true, 60); 
    whitePieces[5].setPiece('B', true, 61); 
    whitePieces[6].setPiece('N', true, 62); 
    whitePieces[7].setPiece('R', true, 63); 
    
    for(int i=8;i<16;i++){
        whitePieces[i].setPiece('P', true, 48 +(i-8));
        blackPieces[i].setPiece('P', false, 15 - (i-8) );
    }

    calcPossibleMoves();

}



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



void ChessGame::createMovesSquares(){

    if(selectedPiece == NULL)
        return;

    possibleMovesSquares.clear();

    for(int i=0; i<selectedPiece->getPossibleMoves().size();i++){
        sf::RectangleShape tmp;
        tmp.setPosition(sf::Vector2f((selectedPiece->getPossibleMoves().at(i) % 8) * 64.f , (selectedPiece->getPossibleMoves().at(i) / 8) * 64.f));
        tmp.setSize(sf::Vector2f(64.f, 64.f));
        tmp.setFillColor(sf::Color(0x66b4cc50));
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



bool ChessGame::selectPiece(int pos){

    for(int i=0; i<16; i++){
        if(playerTurn){ // If white turn
            if(whitePieces[i].getPosition() == pos){
                selectedPiece = &whitePieces[i];
                selected = true;
                break;
            }
        }
        else{ // If black turn
            if(blackPieces[i].getPosition() == pos){
                selectedPiece = &blackPieces[i];
                selected = true;
                break;
            }
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
        
        // If Castling Move
        if((selectedPiece->getType() == 'K') && (!selectedPiece->getMoved())){
            if(selectedPiece->getPlayer()){ // If white
                // whitePieces[0] Bot Left Rook, whitePieces[7] Bot Right Rook
                if(pos == 62)
                    whitePieces[7].setPosition(61);
                else if(pos == 58)
                    whitePieces[0].setPosition(59);
            }
            else{ // If Black
                // blackPieces[7] Top Left Rook, blackPieces[0] Top Right Rook
                if(pos == 6)
                    blackPieces[0].setPosition(5);
                else if(pos == 2)
                    blackPieces[7].setPosition(3);
            }
        }
        
        
        selectedPiece->setPosition(pos);


        std::cout << "Moved " << selectedPiece->toString();
        for(int i=0; i<16; i++){
            if(selectedPiece->getPlayer()){ // If White
                if(blackPieces[i].getPosition() == pos){
                    blackPieces[i].setPosition(-1);
                    break;
                }
            }
            else{ // If Black
                if(whitePieces[i].getPosition() == pos){
                    whitePieces[i].setPosition(-1);
                    break;
                }
            }
        }

        if(playerTurnCheck){
            playerTurnCheck = false;
            std::cout << "King Left Check\n";
        }

        playerTurn = !playerTurn; // Here player turn changes
        calcPossibleMoves();
    }
    else{
        std::cout << "Invalid move\n";
    }
    
    selectedPiece = NULL;
    selected = false;

}



void ChessGame::calcPossibleMoves(){

    Piece* tmpPiece;

    // LOOP for every piece
    for(int x=0; x<32; x++){
        
        if(x<16)
            tmpPiece = &whitePieces[x];
        else
            tmpPiece = &blackPieces[x-16];
        tmpPiece->getPossibleMoves().clear();
        int piecePos = tmpPiece->getPosition();

        if(piecePos == -1)
            continue;

        // Calculate Moves for tmpPiece by piece type
        switch (tmpPiece->getType())
        {
            case 'K':
                // Kings are checked last
                break;
            case 'Q':
                calcQueenMoves(tmpPiece);
                break;
            case 'R':
                calcRookMoves(tmpPiece);
                break;
            case 'B':
                calcBishopMoves(tmpPiece);
                break;
            case 'N':
                calcKnightMoves(tmpPiece);
                break;
            case 'P':
                calcPawnMoves(tmpPiece);
                break;
            default:
                std::cerr << "Error piece type does not exist.\n";
                break;
        }
    }

    // Check for Kings moves
    // whitePieces[4] is white King , blackPieces[3] is black King
    calcKingMoves(&whitePieces[4]);
    calcKingMoves(&blackPieces[3]);

    // Erase moves on positions that both kings can make
    // whitePieces[4] is white King , blackPieces[3] is black King
    if(playerTurn){
        for(int i = 0; i < blackPieces[3].getPossibleMoves().size(); i++){
            for(int j=0; j < whitePieces[4].getPossibleMoves().size(); j++){
                if(whitePieces[4].getPossibleMoves().at(j) == blackPieces[3].getPossibleMoves().at(i)){
                     whitePieces[4].getPossibleMoves().erase( whitePieces[4].getPossibleMoves().begin() + j );
                     break;
                }
            }
        }
    }
    else{
        for(int j=0; j < whitePieces[4].getPossibleMoves().size(); j++){
            for(int i = 0; i < blackPieces[3].getPossibleMoves().size(); i++){
                if(whitePieces[4].getPossibleMoves().at(j) == blackPieces[3].getPossibleMoves().at(i)){
                     blackPieces[4].getPossibleMoves().erase( whitePieces[4].getPossibleMoves().begin() + i );
                     break;
                }
            }
        }
    }


    checkMate();

    

}




void ChessGame::calcKingMoves(Piece* tmpPiece){

    int piecePos{tmpPiece->getPosition()};

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


    
    if(tmpPiece->getPlayer() == playerTurn){
        
        // Erase moves on same team pieces
        for(int j = 0; j<tmpPiece->getPossibleMoves().size();j++){
            for(int i = 0; i<16; i++){

                if(tmpPiece->getPlayer()){ // White
                    if(tmpPiece->getPossibleMoves().at(j) == whitePieces[i].getPosition()){
                        tmpPiece->getPossibleMoves().erase( tmpPiece->getPossibleMoves().begin() + j-- );
                        break;
                    }
                }

                else{ // Black
                    if(tmpPiece->getPossibleMoves().at(j) == blackPieces[i].getPosition()){
                        tmpPiece->getPossibleMoves().erase( tmpPiece->getPossibleMoves().begin() + j-- );
                        break;
                    } 
                }
            }
        }

        // Erase moves on attacked squares
        for(int j=0; j < tmpPiece->getPossibleMoves().size(); j++){
            for(int i=0; i < 16; i++){
                int o{0};
                if(tmpPiece->getPlayer()){ // White
                    for(o=0; o < blackPieces[i].getPossibleMoves().size();o++){
                        if(tmpPiece->getPossibleMoves().at(j) == blackPieces[i].getPossibleMoves().at(o)){
                            tmpPiece->getPossibleMoves().erase( tmpPiece->getPossibleMoves().begin() + j-- );
                            break;
                        }
                    }
                    if( (o != blackPieces[i].getPossibleMoves().size()))
                        break;;
                }
                else{ // Black
                    for(o=0; o < whitePieces[i].getPossibleMoves().size();o++){
                        if(tmpPiece->getPossibleMoves().at(j) == whitePieces[i].getPossibleMoves().at(o)){
                            tmpPiece->getPossibleMoves().erase( tmpPiece->getPossibleMoves().begin() + j-- );
                            break;
                        }
                    }
                    if( (o != whitePieces[i].getPossibleMoves().size()))
                        break;;
                }
            }
        }
    }

    calcCastling(tmpPiece);

    //TODO CheckMating

}





void ChessGame::calcQueenMoves(Piece* tmpPiece){

    int piecePos{tmpPiece->getPosition()};
    int posCounter{1};
    // Queen moves left on X axis
    while( ((piecePos-posCounter) >= 0) && ((piecePos/8) == ((piecePos-posCounter)/8)) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
        }
        if(i != 16) break;

        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        posCounter += 1;
    }
    // Queen moves right on X axis
    posCounter = 1;
    while( (piecePos/8) == ((piecePos+posCounter)/8) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
        }
        if(i != 16) break;
        
        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        posCounter += 1;
    }
    //Queen moves up on Y axis
    posCounter = 8;
    while(((piecePos-posCounter) >= 0) && (posCounter < 64) && ((piecePos%8) == ((piecePos-posCounter)%8)) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
        }
        if(i != 16) break;
        
        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        posCounter += 8;
    }
    //Queen moves down on Y axis
    posCounter = 8;
    while(((piecePos+posCounter) <= 63) && (posCounter < 64) && ((piecePos%8) == ((piecePos+posCounter)%8)) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
        }
        if(i != 16) break;
        
        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        posCounter += 8;
    }
    // Queen moves towards top left
    posCounter = 9;
    while( ((piecePos-posCounter) >= 0) && (((piecePos-posCounter) % 8) < (piecePos % 8)) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
        }
        if(i != 16) break;
        
        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        posCounter += 9;
    }
    // Queen moves towards bottom right
    posCounter = 9;
    while( ((piecePos+posCounter) <= 63) && (((piecePos+posCounter) % 8) > (piecePos % 8)) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
        }
        if(i != 16) break;
        
        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        posCounter += 9;
    }
    // Queen moves towards top right
    posCounter = 7;
    while( ((piecePos-posCounter) >= 0) && (((piecePos-posCounter) % 8) > (piecePos % 8)) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
        }
        if(i != 16) break;
        
        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        posCounter += 7;
    }
    // Queen moves towards bottom left
    posCounter = 7;
    while( ((piecePos+posCounter) <= 63) && (((piecePos+posCounter) % 8) < (piecePos % 8)) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
        }
        if(i != 16) break;
        
        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        posCounter += 7;
    }

}





void ChessGame::calcRookMoves(Piece* tmpPiece){

    int piecePos = tmpPiece->getPosition();
    int posCounter{1};
    // Rook moves left on X axis
    while( ((piecePos-posCounter) >= 0) && ((piecePos/8) == ((piecePos-posCounter)/8)) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
        }
        if(i != 16) break;

        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        posCounter += 1;
    }

    // Rook moves right on X axis
    posCounter = 1;
    while( (piecePos/8) == ((piecePos+posCounter)/8) ){
        
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
        }
        if(i != 16) break;
        
        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        posCounter += 1;
    }

    // Rook moves up on Y axis
    posCounter = 8;
    while(((piecePos-posCounter) >= 0) && (posCounter < 64) && ((piecePos%8) == ((piecePos-posCounter)%8)) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
        }
        if(i != 16) break;
        
        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        posCounter += 8;
    }

    // Rook moves down on Y axis
    posCounter = 8;
    while(((piecePos+posCounter) <= 63) && (posCounter < 64) && ((piecePos%8) == ((piecePos+posCounter)%8)) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
        }
        if(i != 16) break;
        
        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        posCounter += 8;
    }
}





void ChessGame::calcBishopMoves(Piece* tmpPiece){

    //Normal Bishop Moving
    int piecePos{tmpPiece->getPosition()};
    int posCounter{9};
    // Bishop moves towards top left
    while( ((piecePos-posCounter) >= 0) && (((piecePos-posCounter) % 8) < (piecePos % 8)) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
        }
        if(i != 16) break;

        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        posCounter += 9;
    }

    // Bishop moves towards bottom right
    posCounter = 9;
    while( ((piecePos+posCounter) <= 63) && (((piecePos+posCounter) % 8) > (piecePos % 8)) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
        }
        if(i != 16) break;

        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        posCounter += 9;
    }

    // Bishop moves towards top right
    posCounter = 7;
    while( ((piecePos-posCounter) >= 0) && (((piecePos-posCounter) % 8) > (piecePos % 8)) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos-posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
                break;
            }
        }
        if(i != 16) break;

        tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        posCounter += 7;
    }

    // Bishop moves towards bottom left
    posCounter = 7;
    while( ((piecePos+posCounter) <= 63) && (((piecePos+posCounter) % 8) < (piecePos % 8)) ){
        int i{0};
        for(i = 0; i<16; i++){
            if(whitePieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != whitePieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
            if(blackPieces[i].getPosition() == (piecePos+posCounter)){
                if(tmpPiece->getPlayer() != blackPieces[i].getPlayer())
                    tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
                break;
            }
        }
        if(i != 16) break;

        tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        posCounter += 7;
    }

}





void ChessGame::calcKnightMoves(Piece* tmpPiece){

    int piecePos{tmpPiece->getPosition()};

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

    // Erase moves on same team pieces
    for(int j = 0; j<tmpPiece->getPossibleMoves().size();j++){
        
        for(int i = 0; i<16; i++){

            if(tmpPiece->getPlayer()){ // White
                if(tmpPiece->getPossibleMoves().at(j) == whitePieces[i].getPosition()){
                    tmpPiece->getPossibleMoves().erase( tmpPiece->getPossibleMoves().begin() + j-- );
                    break;
                }
            }

            else{ // Black
                if(tmpPiece->getPossibleMoves().at(j) == blackPieces[i].getPosition()){
                    tmpPiece->getPossibleMoves().erase( tmpPiece->getPossibleMoves().begin() + j-- );
                    break;
                } 
            }
        }
    }

}




void ChessGame::calcPawnMoves(Piece* tmpPiece){

    //TODO check en passant

    int piecePos{tmpPiece->getPosition()};
    
    if (tmpPiece->getPlayer()){ // If pawn is white
        if((piecePos / 8) != 0){
            int i{0};
            for(i = 0; i<16;i++){
                if((whitePieces[i].getPosition() == (piecePos - 8) ) || (blackPieces[i].getPosition() == (piecePos - 8) ))
                    break;
            }
            if((i == 16) && playerTurn){
                tmpPiece->getPossibleMoves().push_back(piecePos - 8);

                if(!tmpPiece->getMoved()){
                    for(i = 0; i<16;i++){
                        if((whitePieces[i].getPosition() == (piecePos - 16) ) || (blackPieces[i].getPosition() == (piecePos - 16) ))
                            break;
                    }
                    if(i == 16)
                        tmpPiece->getPossibleMoves().push_back(piecePos - 16);
                }
            }

            if((piecePos % 8) != 0){
                for(i = 0; i<16; i++){
                    if(blackPieces[i].getPosition() == (piecePos - 9))
                        break;
                }
                if((i != 16) || !playerTurn)
                    tmpPiece->getPossibleMoves().push_back(piecePos - 9);
            }

            if((piecePos % 8) != 7){
                for(i = 0; i<16; i++){
                    if(blackPieces[i].getPosition() == (piecePos - 7))
                        break;
                }
                if((i != 16) || !playerTurn)
                    tmpPiece->getPossibleMoves().push_back(piecePos - 7);
            }

        }
        else{ // MUST PROMOTE PAWN
            std::cout << "MUST PROMOTE PAWN\n";
        }
        

    }
    else{ //if pawn is black

        if((piecePos / 8) != 7){
            int i{0};
            for(i = 0; i<16;i++){
                if((whitePieces[i].getPosition() == (piecePos + 8) ) || (blackPieces[i].getPosition() == (piecePos + 8) ))
                    break;
            }
            if((i == 16) && !playerTurn){
                tmpPiece->getPossibleMoves().push_back(piecePos + 8);

                if(!tmpPiece->getMoved()){
                    for(i = 0; i<16;i++){
                        if((whitePieces[i].getPosition() == (piecePos + 16) ) || (blackPieces[i].getPosition() == (piecePos + 16) ))
                            break;
                    }
                    if(i == 16)
                        tmpPiece->getPossibleMoves().push_back(piecePos + 16);
                }
            }

            if((piecePos % 8) != 0){
                for(i = 0; i<16; i++){
                    if(whitePieces[i].getPosition() == (piecePos + 7))
                        break;
                }
                if((i != 16) || playerTurn)
                    tmpPiece->getPossibleMoves().push_back(piecePos + 7);
            }

            if((piecePos % 8) != 7){
                for(i = 0; i<16; i++){
                    if(whitePieces[i].getPosition() == (piecePos + 9))
                        break;
                }
                if((i != 16) || playerTurn)
                    tmpPiece->getPossibleMoves().push_back(piecePos + 9);
            }

        }
        else{ // MUST PROMOTE PAWN
            std::cout << "MUST PROMOTE PAWN\n";
        }
    }
}





void ChessGame::calcCastling(Piece* tmpPiece){


    if( playerTurnCheck || (tmpPiece->getType() != 'K') || tmpPiece->getMoved() || (tmpPiece->getPlayer() != playerTurn))
        return;

    
    if(tmpPiece->getPlayer()){ // If White King
        // whitePieces[0] Bot Left Rook, whitePieces[7] Bot Right Rook
        if(!whitePieces[7].getMoved()){
            int i{0};
            for(i=0;i<16;i++){
                if((whitePieces[i].getPosition() == 61) || (whitePieces[i].getPosition() == 62)){
                    i=17;
                    break;
                }
                if((blackPieces[i].getPosition() == 61) || (blackPieces[i].getPosition() == 62)){
                    i = 17;
                    break;
                }
            }
            if(i == 16){
                for(i=0; i<16; i++){
                    for(int j=0; j<blackPieces[i].getPossibleMoves().size(); j++){
                        std::cout << "Castle gut" << j <<"\n";
                        if((blackPieces[i].getPossibleMoves().at(j) == 61) || (blackPieces[i].getPossibleMoves().at(j) == 62) || (blackPieces[i].getPossibleMoves().at(j) == 60)){
                            i = 17;
                            break;
                        }
                    }
                    if(i == 17) break;
                }
            }
            if(i != 17){
                
                tmpPiece->getPossibleMoves().push_back(62);
            }
        }

        if(!whitePieces[0].getMoved()){
            int i{0};
            for(i=0;i<16;i++){
                if((whitePieces[i].getPosition() == 57) || (whitePieces[i].getPosition() == 58) || (whitePieces[i].getPosition() == 59)){
                    i=17;
                    break;
                }
                if((blackPieces[i].getPosition() == 57) || (blackPieces[i].getPosition() == 58) || (blackPieces[i].getPosition() == 59)){
                    i =17;
                    break;
                }
            }
            if(i == 16){
                for(i=0; i<16; i++){
                    for(int j=0; j<blackPieces[i].getPossibleMoves().size(); j++){
                        if((blackPieces[i].getPossibleMoves().at(j) == 58) || (blackPieces[i].getPossibleMoves().at(j) == 59) || (blackPieces[i].getPossibleMoves().at(j) == 60)){
                            i = 17;
                            break;
                        }
                    }
                    if(i == 17) break;
                }
            }
            if(i != 17){
                tmpPiece->getPossibleMoves().push_back(58);
            }
        }
    }
    else{ // If Black King
        // blackPieces[7] Top Left Rook, blackPieces[0] Top Right Rook
        if(!blackPieces[7].getMoved()){
            int i{0};
            for(i=0;i<16;i++){
                if((whitePieces[i].getPosition() == 3) || (whitePieces[i].getPosition() == 2) || (whitePieces[i].getPosition() == 1)){
                    i=17;
                    break;
                }
                if((blackPieces[i].getPosition() == 3) || (blackPieces[i].getPosition() == 2) || (blackPieces[i].getPosition() == 1)){
                    i=17;
                    break;
                }
            }
            if(i == 16){
                for(i=0; i<16; i++){
                    for(int j=0; j<whitePieces[i].getPossibleMoves().size(); j++){
                        if((whitePieces[i].getPossibleMoves().at(j) == 3) || (whitePieces[i].getPossibleMoves().at(j) == 2) || (whitePieces[i].getPossibleMoves().at(j) == 4)){
                            i = 17;
                            break;
                        }
                    }
                    if(i == 17) break;
                }
            }
            if(i != 17){
                tmpPiece->getPossibleMoves().push_back(2);
            }
        }

        if(!blackPieces[0].getMoved()){
            int i{0};
            for(i=0;i<16;i++){
                if((whitePieces[i].getPosition() == 5) || (whitePieces[i].getPosition() == 6)){
                    i=17;
                    break;
                }
                if((blackPieces[i].getPosition() == 5) || (blackPieces[i].getPosition() == 6)){
                    i=17;
                    break;
                }
            }
            if(i == 16){
                for(i=0; i<16; i++){
                    for(int j=0; j<whitePieces[i].getPossibleMoves().size(); j++){
                        if((whitePieces[i].getPossibleMoves().at(j) == 5) || (whitePieces[i].getPossibleMoves().at(j) == 6) || (whitePieces[i].getPossibleMoves().at(j) == 4)){
                            i = 17;
                            break;
                        }
                    }
                    if(i == 17) break;
                }
            }
            if(i != 17){
                tmpPiece->getPossibleMoves().push_back(6);
            }
        }
    }
}




void ChessGame::checkMate(){
    // No more than two piece can check a King.
    // A single check can be unchecked either by the King moving to
        // a square that is not attacked, or if another piece blocks/kills the attacking piece
    // A double check can be unchecked only by moving the checked King

    Piece* check1{NULL};
    Piece* check2{NULL};

    // Check if current player's King is in check
    // whitePieces[4] is white King , blackPieces[3] is black King
    for(int i = 0; i<16; i++){
        if(playerTurn){
            for(int j=0; j < blackPieces[i].getPossibleMoves().size(); j++){
                if(whitePieces[4].getPosition() == blackPieces[i].getPossibleMoves().at(j)){
                    if(check1 == NULL){
                        playerTurnCheck = true;
                        check1 = &blackPieces[i];
                        std::cout << "White King is in check.\n";
                        break;
                    }
                    else if(check2 == NULL){
                        //playerTurnCheck = true;
                        check2 = &blackPieces[i];
                        std::cout << "White King is Double Checked\n";
                        break;
                    }
                }
            }
        }
        else{
            for(int j=0; j < whitePieces[i].getPossibleMoves().size(); j++){
                if(blackPieces[3].getPosition() == whitePieces[i].getPossibleMoves().at(j)){
                    if(check1 == NULL){
                        playerTurnCheck = true;
                        check1 = &whitePieces[i];
                        std::cout << "Black King is in check.\n";
                        break;
                    }
                    else if(check2 == NULL){
                        //playerTurnCheck = true;
                        check2 = &whitePieces[i];
                        std::cout << "Black King is Double Checked\n";
                        break;
                    }
                }
            }
        }

        if(check2 != NULL)
            break;
    }

    if(check1 != NULL)
        std::cout << "Check 1: " << check1->toString();
    if(check2 != NULL)
        std::cout << "Check 2: " << check2->toString();

    // Check which current player pieces moves put its King out of check
    // If no moves then Check Mate, current player loses
    /*if(playerTurnCheck){ 

    }
    else{ 
        // Check if current player pieces moves put its King in check
        // Also if current player is not checked and has no available moves then Stalemate (AKA Draw)
        


    }*/




}