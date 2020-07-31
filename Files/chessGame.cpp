/*
    This code file contains member functions of chessGame.h
*/

#include "chessGame.h"


ChessGame::ChessGame(sf::Color bordCol1 = sf::Color::White, sf::Color bordCol2 = sf::Color::Black)
: board(bordCol1,bordCol2) , selected{false} , playerTurn{true}
{
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
        tmp.setFillColor(sf::Color(0x66b4cc70));
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
                {
                // Normal King Moves
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



                    //TODO CheckMating
                }
                break;
            case 'Q':
                //TODO Normal Queen Moving
                {
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
                break;
            case 'R':
                {
                    //Normal Rook Moving
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
                        
                        std::cout << "Rook move found\n";

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
                break;
            case 'B':
                {
                    //Normal Bishop Moving
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
                break;
            case 'P':
                // TODO Pawn Moving Rules, Attackinh, Promotion
                {
                    if (tmpPiece->getPlayer()){ // If pawn is white
                        
                        if((piecePos / 8) != 0){
                            int i{0};
                            for(i = 0; i<16;i++){
                                if((whitePieces[i].getPosition() == (piecePos - 8) ) || (blackPieces[i].getPosition() == (piecePos - 8) ))
                                    break;
                            }
                            if(i == 16){
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
                                if(i != 16)
                                    tmpPiece->getPossibleMoves().push_back(piecePos - 9);
                            }

                            if((piecePos % 8) != 7){
                                for(i = 0; i<16; i++){
                                    if(blackPieces[i].getPosition() == (piecePos - 7))
                                        break;
                                }
                                if(i != 16)
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
                            if(i == 16){
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
                                if(i != 16)
                                    tmpPiece->getPossibleMoves().push_back(piecePos + 7);
                            }

                            if((piecePos % 8) != 7){
                                for(i = 0; i<16; i++){
                                    if(whitePieces[i].getPosition() == (piecePos + 9))
                                        break;
                                }
                                if(i != 16)
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

    // Check for Castling, etc
    for(int x=0; x<32; x++){
        
        if(x<16)
            tmpPiece = &whitePieces[x];
        else
            tmpPiece = &blackPieces[x-16];
        int piecePos = tmpPiece->getPosition();

        if(piecePos == -1)
            continue;

        if(tmpPiece->getType() == 'K'){
            

            //TODO Castling
            if(!tmpPiece->getMoved()){
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

        }

    }



}

