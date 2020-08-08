/*
    This code file contains member functions of chessGame.h
*/

#include "chessGame.h"


ChessGame::ChessGame(sf::Color bordCol1 = sf::Color::White, sf::Color bordCol2 = sf::Color::Black)
: board(bordCol1,bordCol2)
{
    // The code is taking account of these indexes.
    // Changing them may brake normal chess rules.
    // Comment out pieces if you want to remove some pieces at beggining.

    font.loadFromFile("Textures/arial.ttf");
    
    
    infoRestart.setFillColor(sf::Color::White);
    infoRestart.setOutlineThickness(-5.f);
    infoRestart.setOutlineColor(sf::Color::Black);
    infoRestart.setPosition(sf::Vector2f(512.f,0.f));
    infoRestart.setSize(sf::Vector2f(256.f, 50.f));

    textRestart.setFont(font);
    textRestart.setString("RESTART");
    textRestart.setCharacterSize(24);
    textRestart.setStyle(sf::Text::Bold);
    textRestart.setFillColor(sf::Color::Black);
    textRestart.setPosition(infoRestart.getPosition().x + 75.f, infoRestart.getPosition().y + 10.f);

    textTurn.setFont(font);
    textTurn.setCharacterSize(24);
    textTurn.setStyle(sf::Text::Bold);
    textTurn.setFillColor(sf::Color::White);
    textTurn.setPosition(530.f, 70.f);

    textSituation.setFont(font);
    textSituation.setCharacterSize(24);
    textSituation.setStyle(sf::Text::Bold);
    textSituation.setFillColor(sf::Color::White);
    textSituation.setPosition(530.f, 110.f);

    textLastMove.setFont(font);
    textLastMove.setCharacterSize(24);
    textLastMove.setStyle(sf::Text::Bold);
    textLastMove.setFillColor(sf::Color::White);
    textLastMove.setPosition(530.f, 200.f);


    restart();

}



void ChessGame::restart(){

    selected = false;
    playerTurn = true;
    playerTurnCheck = false;
    mate = false;
    turn = 1;

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

    textLastMove.setString(" ");


}

void ChessGame::updateInfo(){
    textTurn.setString("Turn: " + std::to_string(turn));
    textLastMove.setString(lastMove);

    if(!mate){
        if(playerTurn)
            textSituation.setString("White's Turn");
        else
            textSituation.setString("Blacks's Turn");
        
        if(playerTurnCheck)
            textSituation.setString(textSituation.getString() + "\nCheck");
    }
    else{
        if(playerTurnCheck){
            if(playerTurn)
                textSituation.setString("CHECKMATE\nBlack Wins");
            else
                textSituation.setString("CHECKMATE\nWhite Wins");
        }
        else{
            textSituation.setString("STALEMATE\nIts a DRAW");
        }

    }
}



void ChessGame::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.clear(sf::Color::Black);

    target.draw(board);
    target.draw(infoRestart);
    target.draw(textRestart);
    target.draw(textTurn);
    target.draw(textSituation);
    target.draw(textLastMove);

    if((selectedPiece != NULL) && (selected)){
        for(int i=0; i<possibleMovesSquares.size();i++){
            target.draw(possibleMovesSquares.at(i));
        }
    }

    for(int i=0;i<16;i++){
        target.draw(whitePieces[i]);
        target.draw(blackPieces[i]);
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
        return selected;
    }


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


        

        // If Pawn double move (set en passant)
        // White pawn -16, Black pawn +16
        if((selectedPiece->getType() == 'P')){
            if(!selectedPiece->getMoved()){
                if(pos == (selectedPiece->getPosition() - 16)){
                    selectedPiece->setEnPassant(selectedPiece->getPosition() - 8);
                }
                else if(pos == (selectedPiece->getPosition() + 16)){
                    selectedPiece->setEnPassant(selectedPiece->getPosition() + 8);
                }
            }
            else{
                for(int i=0; i<16; i++){
                    if(playerTurn){
                        if(pos == blackPieces[i].getEnPassant())
                            blackPieces[i].setPosition(pos);
                    }
                    else{
                        if(pos == whitePieces[i].getEnPassant())
                            whitePieces[i].setPosition(pos);
                    }
                }
            }
        }
        if(selectedPiece->getMoved()){
            for(int i=0; i<16; i++){
                whitePieces[i].setEnPassant(-1);
                blackPieces[i].setEnPassant(-1);
            }
        }


        selectedPiece->setPosition(pos);

        


        lastMove = "Last Turn:\n" + selectedPiece->toString();
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
        }

        playerTurn = !playerTurn; // Here player turn changes
        calcPossibleMoves();
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
        tmpPiece->getDangerMoves().clear();

        if(tmpPiece->getPosition() == -1)
            continue;

        // Calculate Moves for tmpPiece by piece type
        switch (tmpPiece->getType())
        {
            case 'K':
                calcKingMoves(tmpPiece);
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


    // Erase illegal moves on current player's pieces
    for(int x = 0; x < 16; x++){
        if(playerTurn){
            eraseMoves(&whitePieces[x]);
        }
        else{
            eraseMoves(&blackPieces[x]);
        }
    }



    checkMate();

    
    updateInfo();
    turn++;
}




void ChessGame::eraseMoves(Piece* tmpPiece){

    if(tmpPiece->getPosition() == -1)
        return;


    if(tmpPiece->getPlayer() == playerTurn){
        
        // Erase moves on same team pieces
        
        for(int i = 0; i<16; i++){
            for(int j = 0; j<tmpPiece->getPossibleMoves().size();j++){

                if(tmpPiece->getPlayer()){ // White
                    if(tmpPiece->getPossibleMoves().at(j) == whitePieces[i].getPosition()){
                        tmpPiece->getPossibleMoves().erase( tmpPiece->getPossibleMoves().begin() + j );
                        break;
                    }
                }
                else{ // Black
                    if(tmpPiece->getPossibleMoves().at(j) == blackPieces[i].getPosition()){
                        tmpPiece->getPossibleMoves().erase( tmpPiece->getPossibleMoves().begin() + j );
                        break;
                    } 
                }
            }
        }

        // Erase King moves on attacked squares
        if(tmpPiece->getType() == 'K'){
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


        // Erase moves that put current piece's king in check        
        if(tmpPiece->getType() != 'K'){
            for(int i=0; i<16; i++){
                if     ( playerTurn && (blackPieces[i].getDangerMoves().size() > 1) ){
                    for(int j=0; j<blackPieces[i].getDangerMoves().size(); j++){
                        if(blackPieces[i].getDangerMoves().at(j) == tmpPiece->getPosition()){
                            std::vector<int> tmpMoves;
                            for(int x=0; x<tmpPiece->getPossibleMoves().size(); x++){
                                for(int k=0; k<blackPieces[i].getDangerMoves().size(); k++){
                                    if(tmpPiece->getPossibleMoves().at(x) == blackPieces[i].getDangerMoves().at(k))
                                        tmpMoves.push_back( tmpPiece->getPossibleMoves().at(x) );
                                }
                            }
                            tmpPiece->getPossibleMoves().clear();
                            tmpPiece->getPossibleMoves() = tmpMoves;
                            break;
                        }
                    }
                }
                else if(!playerTurn && (whitePieces[i].getDangerMoves().size() > 1) ){
                    for(int j=0; j<whitePieces[i].getDangerMoves().size(); j++){
                        if(whitePieces[i].getDangerMoves().at(j) == tmpPiece->getPosition()){
                            std::vector<int> tmpMoves;
                            for(int x=0; x<tmpPiece->getPossibleMoves().size(); x++){
                                for(int k=0; k<whitePieces[i].getDangerMoves().size(); k++){
                                    if(tmpPiece->getPossibleMoves().at(x) == whitePieces[i].getDangerMoves().at(k))
                                        tmpMoves.push_back( tmpPiece->getPossibleMoves().at(x) );
                                }
                            }
                            tmpPiece->getPossibleMoves().clear();
                            tmpPiece->getPossibleMoves() = tmpMoves;
                            break;
                        }
                    }
                }
            }
        }


    }
}




void ChessGame::calcKingMoves(Piece* tmpPiece){

    int piecePos{tmpPiece->getPosition()};
    tmpPiece->getPossibleMoves().clear();

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


    //calcCastling(tmpPiece);
}





void ChessGame::calcQueenMoves(Piece* tmpPiece){

    int piecePos{tmpPiece->getPosition()};
    int posCounter{1};

    bool dangerMove{false};
    bool finishLoop{false};

    tmpPiece->getPossibleMoves().clear();
    tmpPiece->getDangerMoves().clear();

    // Queen moves left on X axis
    while( ((piecePos-posCounter) >= 0) && ((piecePos/8) == ((piecePos-posCounter)/8)) ){
        
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if(  ( whitePieces[i].getPosition() == (piecePos-posCounter) ) || ( blackPieces[i].getPosition() == (piecePos-posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }
            tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        }
        
        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos-posCounter);
            if(!playerTurn){
                if( (piecePos-posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos-posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }

        }
        
        posCounter += 1;
    }


    // Queen moves right on X axis
    if(!dangerMove) tmpPiece->getDangerMoves().clear();
    finishLoop = false;
    posCounter = 1;
    while( (piecePos/8) == ((piecePos+posCounter)/8) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos+posCounter)) || ( blackPieces[i].getPosition() == (piecePos+posCounter) ) ){
                    finishLoop = true;
                    break;
                }
            }
            tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        }

        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos+posCounter);
            if(!playerTurn){
                if( (piecePos+posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos+posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }

        }
        
        posCounter += 1;
    }
    //Queen moves up on Y axis
    if(!dangerMove) tmpPiece->getDangerMoves().clear();
    finishLoop = false;
    posCounter = 8;
    while(((piecePos-posCounter) >= 0) && (posCounter < 64) && ((piecePos%8) == ((piecePos-posCounter)%8)) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos-posCounter)) || (blackPieces[i].getPosition() == (piecePos-posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }        
            tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        }

        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos-posCounter);
            if(!playerTurn){
                if( (piecePos-posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos-posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }

        }

        posCounter += 8;
    }
    //Queen moves down on Y axis
    if(!dangerMove) tmpPiece->getDangerMoves().clear();
    finishLoop = false;
    posCounter = 8;
    while(((piecePos+posCounter) <= 63) && (posCounter < 64) && ((piecePos%8) == ((piecePos+posCounter)%8)) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos+posCounter)) || (blackPieces[i].getPosition() == (piecePos+posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }
            tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        }
        
        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos+posCounter);
            if(!playerTurn){
                if( (piecePos+posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos+posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }
        }

        posCounter += 8;
    }
    // Queen moves towards top left
    if(!dangerMove) tmpPiece->getDangerMoves().clear();
    finishLoop = false;
    posCounter = 9;
    while( ((piecePos-posCounter) >= 0) && (((piecePos-posCounter) % 8) < (piecePos % 8)) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos-posCounter)) || (blackPieces[i].getPosition() == (piecePos-posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }
            tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        }

        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos-posCounter);
            if(!playerTurn){
                if( (piecePos-posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos-posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }
        }

        posCounter += 9;
    }
    // Queen moves towards bottom right
    if(!dangerMove) tmpPiece->getDangerMoves().clear();
    finishLoop = false;
    posCounter = 9;
    while( ((piecePos+posCounter) <= 63) && (((piecePos+posCounter) % 8) > (piecePos % 8)) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos+posCounter)) || (blackPieces[i].getPosition() == (piecePos+posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }        
            tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        }

        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos+posCounter);
            if(!playerTurn){
                if( (piecePos+posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos+posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }
        }

        posCounter += 9;
    }
    // Queen moves towards top right
    if(!dangerMove) tmpPiece->getDangerMoves().clear();
    finishLoop = false;
    posCounter = 7;
    while( ((piecePos-posCounter) >= 0) && (((piecePos-posCounter) % 8) > (piecePos % 8)) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos-posCounter)) || (blackPieces[i].getPosition() == (piecePos-posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }        
            tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        }

        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos-posCounter);
            if(!playerTurn){
                if( (piecePos-posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos-posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }
        }

        posCounter += 7;
    }
    // Queen moves towards bottom left
    if(!dangerMove) tmpPiece->getDangerMoves().clear();
    finishLoop = false;
    posCounter = 7;
    while( ((piecePos+posCounter) <= 63) && (((piecePos+posCounter) % 8) < (piecePos % 8)) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos+posCounter)) || (blackPieces[i].getPosition() == (piecePos+posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }        
            tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        }

        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos+posCounter);
            if(!playerTurn){
                if( (piecePos+posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos+posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }
        }

        posCounter += 7;
    }

    if(!dangerMove) tmpPiece->getDangerMoves().clear();

    
    if(!tmpPiece->getDangerMoves().empty()){
        int collisions{0};
        for(int j=0; j<tmpPiece->getDangerMoves().size(); j++){
            for(int i=0; i<16; i++){
                if(tmpPiece->getDangerMoves().at(j) == blackPieces[i].getPosition()){
                    collisions++;
                    if(!tmpPiece->getPlayer())
                        collisions++;
                }
                if(tmpPiece->getDangerMoves().at(j) == whitePieces[i].getPosition()){
                    collisions++;
                    if(tmpPiece->getPlayer())
                        collisions++;
                }
            }
        }

        if(collisions > 2)
            tmpPiece->getDangerMoves().clear();
    }

    tmpPiece->getDangerMoves().push_back( tmpPiece->getPosition() );
    
}





void ChessGame::calcRookMoves(Piece* tmpPiece){

    int piecePos = tmpPiece->getPosition();
    int posCounter{1};

    bool dangerMove{false};
    bool finishLoop{false};

    tmpPiece->getPossibleMoves().clear();
    tmpPiece->getDangerMoves().clear();

    // Rook moves left on X axis
    while( ((piecePos-posCounter) >= 0) && ((piecePos/8) == ((piecePos-posCounter)/8)) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos-posCounter)) || (blackPieces[i].getPosition() == (piecePos-posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }
            tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        }
        
        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos-posCounter);
            if(!playerTurn){
                if( (piecePos-posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos-posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }
        }

        posCounter += 1;
    }

    // Rook moves right on X axis
    if(!dangerMove) tmpPiece->getDangerMoves().clear();
    finishLoop = false;
    posCounter = 1;
    while( (piecePos/8) == ((piecePos+posCounter)/8) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos+posCounter)) || (blackPieces[i].getPosition() == (piecePos+posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }        
            tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        }

        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos+posCounter);
            if(!playerTurn){
                if( (piecePos+posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos+posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }
        }
        
        posCounter += 1;
    }

    // Rook moves up on Y axis
    if(!dangerMove) tmpPiece->getDangerMoves().clear();
    finishLoop = false;
    posCounter = 8;
    while(((piecePos-posCounter) >= 0) && (posCounter < 64) && ((piecePos%8) == ((piecePos-posCounter)%8)) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos-posCounter)) || (blackPieces[i].getPosition() == (piecePos-posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }
            tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        }

        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos-posCounter);
            if(!playerTurn){
                if( (piecePos-posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos-posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }
        }

        posCounter += 8;
    }

    // Rook moves down on Y axis
    if(!dangerMove) tmpPiece->getDangerMoves().clear();
    finishLoop = false;
    posCounter = 8;
    while(((piecePos+posCounter) <= 63) && (posCounter < 64) && ((piecePos%8) == ((piecePos+posCounter)%8)) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos+posCounter)) || (blackPieces[i].getPosition() == (piecePos+posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }
            tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        }

        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos+posCounter);
            if(!playerTurn){
                if( (piecePos+posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos+posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }
        }

        posCounter += 8;
    }

    if(!dangerMove) tmpPiece->getDangerMoves().clear();

    
    if(!tmpPiece->getDangerMoves().empty()){
        int collisions{0};
        for(int j=0; j<tmpPiece->getDangerMoves().size(); j++){
            for(int i=0; i<16; i++){
                if(tmpPiece->getDangerMoves().at(j) == blackPieces[i].getPosition()){
                    collisions++;
                    if(!tmpPiece->getPlayer())
                        collisions++;
                }
                if(tmpPiece->getDangerMoves().at(j) == whitePieces[i].getPosition()){
                    collisions++;
                    if(tmpPiece->getPlayer())
                        collisions++;
                }
            }
        }

        if(collisions > 2)
            tmpPiece->getDangerMoves().clear();
    }

    tmpPiece->getDangerMoves().push_back( tmpPiece->getPosition() );
    
}





void ChessGame::calcBishopMoves(Piece* tmpPiece){

    //Normal Bishop Moving
    int piecePos{tmpPiece->getPosition()};
    int posCounter{9};

    bool dangerMove{false};
    bool finishLoop{false};

    tmpPiece->getPossibleMoves().clear();
    tmpPiece->getDangerMoves().clear();

    // Bishop moves towards top left
    while( ((piecePos-posCounter) >= 0) && (((piecePos-posCounter) % 8) < (piecePos % 8)) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos-posCounter)) || (blackPieces[i].getPosition() == (piecePos-posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }
            tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        }

        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos-posCounter);
            if(!playerTurn){
                if( (piecePos-posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos-posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }
        }

        posCounter += 9;
    }

    // Bishop moves towards bottom right
    if(!dangerMove) tmpPiece->getDangerMoves().clear();
    finishLoop = false;
    posCounter = 9;
    while( ((piecePos+posCounter) <= 63) && (((piecePos+posCounter) % 8) > (piecePos % 8)) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos+posCounter)) || (blackPieces[i].getPosition() == (piecePos+posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }
            tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        }

        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos+posCounter);
            if(!playerTurn){
                if( (piecePos+posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos+posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }
        }

        posCounter += 9;
    }

    // Bishop moves towards top right
    if(!dangerMove) tmpPiece->getDangerMoves().clear();
    finishLoop = false;
    posCounter = 7;
    while( ((piecePos-posCounter) >= 0) && (((piecePos-posCounter) % 8) > (piecePos % 8)) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos-posCounter)) || (blackPieces[i].getPosition() == (piecePos-posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }
            tmpPiece->getPossibleMoves().push_back(piecePos-posCounter);
        }

        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos-posCounter);
            if(!playerTurn){
                if( (piecePos-posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos-posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }
        }

        posCounter += 7;
    }

    // Bishop moves towards bottom left
    if(!dangerMove) tmpPiece->getDangerMoves().clear();
    finishLoop = false;
    posCounter = 7;
    while( ((piecePos+posCounter) <= 63) && (((piecePos+posCounter) % 8) < (piecePos % 8)) ){
        if(!finishLoop){
            for(int i = 0; i<16; i++){
                if( (whitePieces[i].getPosition() == (piecePos+posCounter)) || (blackPieces[i].getPosition() == (piecePos+posCounter)) ){
                    finishLoop = true;
                    break;
                }
            }
            tmpPiece->getPossibleMoves().push_back(piecePos+posCounter);
        }

        if(!dangerMove){
            // whitePieces[4] is white King , blackPieces[3] is black King
            tmpPiece->getDangerMoves().push_back(piecePos+posCounter);
            if(!playerTurn){
                if( (piecePos+posCounter) == blackPieces[3].getPosition() )
                    dangerMove = true;
            }
            else{
                if( (piecePos+posCounter) == whitePieces[4].getPosition() )
                    dangerMove = true;
            }
        }

        posCounter += 7;
    }

    if(!dangerMove) tmpPiece->getDangerMoves().clear();
    
    
    if(!tmpPiece->getDangerMoves().empty()){
        int collisions{0};
        for(int j=0; j<tmpPiece->getDangerMoves().size(); j++){
            for(int i=0; i<16; i++){
                if(tmpPiece->getDangerMoves().at(j) == blackPieces[i].getPosition()){
                    collisions++;
                    if(!tmpPiece->getPlayer())
                        collisions++;
                }
                if(tmpPiece->getDangerMoves().at(j) == whitePieces[i].getPosition()){
                    collisions++;
                    if(tmpPiece->getPlayer())
                        collisions++;
                }
            }
        }

        if(collisions > 2)
            tmpPiece->getDangerMoves().clear();
    }
    
    tmpPiece->getDangerMoves().push_back( tmpPiece->getPosition() );
    

}





void ChessGame::calcKnightMoves(Piece* tmpPiece){

    tmpPiece->getPossibleMoves().clear();

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

    tmpPiece->getDangerMoves().clear();

    for(int i = 0; i < tmpPiece->getPossibleMoves().size(); i++){

        if(!playerTurn){
            if( (tmpPiece->getPossibleMoves().at(i)) == blackPieces[3].getPosition() )
                tmpPiece->getDangerMoves().push_back( tmpPiece->getPossibleMoves().at(i) );
        }
        else{
            if( (tmpPiece->getPossibleMoves().at(i)) == whitePieces[4].getPosition() )
                tmpPiece->getDangerMoves().push_back( tmpPiece->getPossibleMoves().at(i) );
        }

    }

    tmpPiece->getDangerMoves().push_back( tmpPiece->getPosition() );

}




void ChessGame::calcPawnMoves(Piece* tmpPiece){

    
    tmpPiece->getPossibleMoves().clear();

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
                    if( !playerTurn || (blackPieces[i].getPosition() == (piecePos - 9)) || (whitePieces[i].getPosition() == (piecePos - 9)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos - 9);
                        break;
                    }
                    else if((blackPieces[i].getEnPassant() == (piecePos - 9)) && (blackPieces[i].getEnPassant() != -1)){
                        tmpPiece->getPossibleMoves().push_back(piecePos - 9);
                        break;
                    }
                }
            }

            if((piecePos % 8) != 7){
                for(i = 0; i<16; i++){
                    if( !playerTurn || (blackPieces[i].getPosition() == (piecePos - 7)) || (whitePieces[i].getPosition() == (piecePos - 7)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos - 7);
                        break;
                    }
                    else if((blackPieces[i].getEnPassant() == (piecePos - 7)) && (blackPieces[i].getEnPassant() != -1)){
                        tmpPiece->getPossibleMoves().push_back(piecePos - 7);
                        break;
                    }
                }                    
            }

        }
        else{ // MUST PROMOTE PAWN
            tmpPiece->setPiece('Q', tmpPiece->getPlayer(), tmpPiece->getPosition(), true);
            calcQueenMoves(tmpPiece);
            return;
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
                    if( playerTurn || (whitePieces[i].getPosition() == (piecePos + 7)) || (blackPieces[i].getPosition() == (piecePos + 7)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos + 7);
                        break;
                    }
                    else if((whitePieces[i].getEnPassant() == (piecePos + 7)) && (whitePieces[i].getEnPassant() != -1)){
                        tmpPiece->getPossibleMoves().push_back(piecePos + 7);
                        break;
                    }
                }                    
            }

            if((piecePos % 8) != 7){
                for(i = 0; i<16; i++){
                    if( playerTurn || (whitePieces[i].getPosition() == (piecePos + 9)) || (blackPieces[i].getPosition() == (piecePos + 9)) ){
                        tmpPiece->getPossibleMoves().push_back(piecePos + 9);
                        break;
                    }
                    else if((whitePieces[i].getEnPassant() == (piecePos + 9)) && (whitePieces[i].getEnPassant() != -1)){
                        tmpPiece->getPossibleMoves().push_back(piecePos + 9);
                        break;
                    }
                }                    
            }

        }
        else{ // MUST PROMOTE PAWN
            tmpPiece->setPiece('Q', tmpPiece->getPlayer(), tmpPiece->getPosition(), true);
            calcQueenMoves(tmpPiece);
            return;
        }
    }

    tmpPiece->getDangerMoves().clear();

    for(int i = 0; i < tmpPiece->getPossibleMoves().size(); i++){

        if(!playerTurn){
            if( (tmpPiece->getPossibleMoves().at(i)) == blackPieces[3].getPosition() )
                tmpPiece->getDangerMoves().push_back( tmpPiece->getPossibleMoves().at(i) );
        }
        else{
            if( (tmpPiece->getPossibleMoves().at(i)) == whitePieces[4].getPosition() )
                tmpPiece->getDangerMoves().push_back( tmpPiece->getPossibleMoves().at(i) );
        }

    }

    tmpPiece->getDangerMoves().push_back( tmpPiece->getPosition() );

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
        if(playerTurn){ // White turn
            for(int j=0; j < blackPieces[i].getPossibleMoves().size(); j++){
                if(whitePieces[4].getPosition() == blackPieces[i].getPossibleMoves().at(j)){
                    if(check1 == NULL){
                        playerTurnCheck = true;
                        check1 = &blackPieces[i];
                        break;
                    }
                    else if(check2 == NULL){
                        //playerTurnCheck = true;
                        check2 = &blackPieces[i];
                        break;
                    }
                }
            }
        }
        else{ // Black turn
            for(int j=0; j < whitePieces[i].getPossibleMoves().size(); j++){
                if(blackPieces[3].getPosition() == whitePieces[i].getPossibleMoves().at(j)){
                    if(check1 == NULL){
                        playerTurnCheck = true;
                        check1 = &whitePieces[i];
                        break;
                    }
                    else if(check2 == NULL){
                        //playerTurnCheck = true;
                        check2 = &whitePieces[i];
                        break;
                    }
                }
            }
        }

        if(check2 != NULL)
            break;
    }

    // Check which current player pieces moves put its King out of check
    // If no moves then Check Mate, current player loses
    if(playerTurnCheck){
        if(check2 != NULL){ // If double check, clear current player's pieces moves except king's
            if(playerTurn)
                for(int i=0; i<16; i++)
                    if(whitePieces[i].getType() != 'K')
                        whitePieces[i].getPossibleMoves().clear();
            else
                for(int i=0; i<16; i++)
                    if(blackPieces[i].getType() != 'K')
                        blackPieces[i].getPossibleMoves().clear();
        }
        else{ // If single check

            for(int j=0; j<16; j++){ // pieces array counter
                std::vector<int> tmpMoves;

                if(playerTurn){ // If White turn
                    if(whitePieces[j].getType() == 'K')
                        continue;
                    for(int o = 0; o < whitePieces[j].getPossibleMoves().size(); o++){
                        if(whitePieces[j].getPossibleMoves().empty())
                            break;
                        for(int i=0; i < check1->getDangerMoves().size(); i++){ // Checking piece moves counter
                            if((whitePieces[j].getPossibleMoves().at(o) == check1->getDangerMoves().at(i)) ){
                                tmpMoves.push_back( whitePieces[j].getPossibleMoves().at(o) );
                                break;
                            }
                        }
                    }

                    whitePieces[j].getPossibleMoves().clear();
                    whitePieces[j].getPossibleMoves() = tmpMoves;
                }
                else{ // If Black turn
                    if(blackPieces[j].getType() == 'K')
                        continue;
                    for(int o = 0; o < blackPieces[j].getPossibleMoves().size(); o++){
                        if(blackPieces[j].getPossibleMoves().empty())
                            break;
                        for(int i=0; i < check1->getDangerMoves().size(); i++){ // Checking piece moves counter
                            if((blackPieces[j].getPossibleMoves().at(o) == check1->getDangerMoves().at(i)) ){
                                tmpMoves.push_back( blackPieces[j].getPossibleMoves().at(o) );
                                break;
                            }
                        }
                    }
                    blackPieces[j].getPossibleMoves().clear();
                    blackPieces[j].getPossibleMoves() = tmpMoves;
                }
            }
        }


        //Bug fix
        // whitePieces[4] is white King , blackPieces[3] is black King
        Piece* curKing;
        if(playerTurn){
            curKing = &whitePieces[4];
        }
        else{
            curKing = &blackPieces[3];
        }
        if(check1 != NULL){
            if((check1->getType() == 'Q') || (check1->getType() == 'R' || (check1->getType() == 'B'))){
                int checkPos{check1->getPosition()};

                if((check1->getType() == 'Q') || (check1->getType() == 'R')){
                    for(int i=0; i< curKing->getPossibleMoves().size(); i++){
                        if(curKing->getPossibleMoves().at(i) != checkPos){
                            if     ((curKing->getPossibleMoves().at(i) % 8) == (checkPos % 8))
                                curKing->getPossibleMoves().erase( curKing->getPossibleMoves().begin() + i--);
                            else if((curKing->getPossibleMoves().at(i) / 8) == (checkPos / 8))
                                curKing->getPossibleMoves().erase( curKing->getPossibleMoves().begin() + i--);
                        }
                    }
                }

                for(int i=0; i< curKing->getPossibleMoves().size(); i++){
                    if(curKing->getPossibleMoves().at(i) != checkPos){
                        if((curKing->getPosition()%8) < (checkPos%8)){ // King left of Check
                            if((curKing->getPosition()/8) < (checkPos/8)){ // King top of Check
                                if( ((curKing->getPossibleMoves().at(i)%8) < (curKing->getPosition()%8)) && ((curKing->getPossibleMoves().at(i)/8) <  (curKing->getPosition()/8))  )
                                    curKing->getPossibleMoves().erase( curKing->getPossibleMoves().begin() + i--);
                            }
                            else if((checkPos/8) < (curKing->getPosition()/8)){ // King under Check
                                if( ((curKing->getPossibleMoves().at(i)%8) < (curKing->getPosition()%8)) && ((curKing->getPossibleMoves().at(i)/8) >  (curKing->getPosition()/8))  )
                                    curKing->getPossibleMoves().erase( curKing->getPossibleMoves().begin() + i--);
                            }
                        }
                        else if((checkPos%8) < (curKing->getPosition()%8) ){ // King right of Check
                            if((curKing->getPosition()/8) < (checkPos/8)){ // King top of Check
                                if( ((curKing->getPossibleMoves().at(i)%8) > (curKing->getPosition()%8)) && ((curKing->getPossibleMoves().at(i)/8) <  (curKing->getPosition()/8))  )
                                    curKing->getPossibleMoves().erase( curKing->getPossibleMoves().begin() + i--);
                            }
                            else if((checkPos/8) < (curKing->getPosition()/8)){ // King under Check
                                if( ((curKing->getPossibleMoves().at(i)%8) > (curKing->getPosition()%8)) && ((curKing->getPossibleMoves().at(i)/8) >  (curKing->getPosition()/8))  )
                                    curKing->getPossibleMoves().erase( curKing->getPossibleMoves().begin() + i--);
                            }
                        }
                    }
                }
            }
        }
        //Bug fix
        if(check2 != NULL){
            if((check2->getType() == 'Q') || (check2->getType() == 'R' || (check2->getType() == 'B'))){
                int checkPos{check2->getPosition()};

                if((check2->getType() == 'Q') || (check2->getType() == 'R')){
                    for(int i=0; i< curKing->getPossibleMoves().size(); i++){
                        if(curKing->getPossibleMoves().at(i) != checkPos){
                            if     ((curKing->getPossibleMoves().at(i) % 8) == (checkPos % 8))
                                curKing->getPossibleMoves().erase( curKing->getPossibleMoves().begin() + i--);
                            else if((curKing->getPossibleMoves().at(i) / 8) == (checkPos / 8))
                                curKing->getPossibleMoves().erase( curKing->getPossibleMoves().begin() + i--);
                        }
                    }
                }

                for(int i=0; i< curKing->getPossibleMoves().size(); i++){
                    if(curKing->getPossibleMoves().at(i) != checkPos){
                        if((curKing->getPosition()%8) < (checkPos%8)){ // King left of Check
                            if((curKing->getPosition()/8) < (checkPos/8)){ // King top of Check
                                if( ((curKing->getPossibleMoves().at(i)%8) < (curKing->getPosition()%8)) && ((curKing->getPossibleMoves().at(i)/8) <  (curKing->getPosition()/8))  )
                                    curKing->getPossibleMoves().erase( curKing->getPossibleMoves().begin() + i--);
                            }
                            else if((checkPos/8) < (curKing->getPosition()/8)){ // King under Check
                                if( ((curKing->getPossibleMoves().at(i)%8) < (curKing->getPosition()%8)) && ((curKing->getPossibleMoves().at(i)/8) >  (curKing->getPosition()/8))  )
                                    curKing->getPossibleMoves().erase( curKing->getPossibleMoves().begin() + i--);
                            }
                        }
                        else if((checkPos%8) < (curKing->getPosition()%8) ){ // King right of Check
                            if((curKing->getPosition()/8) < (checkPos/8)){ // King top of Check
                                if( ((curKing->getPossibleMoves().at(i)%8) > (curKing->getPosition()%8)) && ((curKing->getPossibleMoves().at(i)/8) <  (curKing->getPosition()/8))  )
                                    curKing->getPossibleMoves().erase( curKing->getPossibleMoves().begin() + i--);
                            }
                            else if((checkPos/8) < (curKing->getPosition()/8)){ // King under Check
                                if( ((curKing->getPossibleMoves().at(i)%8) > (curKing->getPosition()%8)) && ((curKing->getPossibleMoves().at(i)/8) >  (curKing->getPosition()/8))  )
                                    curKing->getPossibleMoves().erase( curKing->getPossibleMoves().begin() + i--);
                            }
                        }
                    }
                }
            }
        }

    }
    else{
        calcCastling(&whitePieces[4]);
        calcCastling(&blackPieces[3]);
    }

    // Check if current player has any available moves
    int i{0};
    for(i=0; i<16; i++){
        if(playerTurn){
            if(!whitePieces[i].getPossibleMoves().empty())
                break;
        }
        else{
            if(!blackPieces[i].getPossibleMoves().empty())
                break;
        }
    }
    if(i==16){ 
        mate = true;
    }

}