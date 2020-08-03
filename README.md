Two player chess made with C++ and SFML(2D Graphics).


You need to have SFML installed to compile.
https://www.sfml-dev.org/download/sfml/2.5.1/

I have not tried it on Windows yet.

Linux GCC :
    g++ -c main.cpp board.cpp piece.cpp pieceTextures.cpp chessGame.cpp
    g++ main.o board.o piece.o pieceTextures.o chessGame.o -lsfml-graphics -lsfml-window -lsfml-system



Made By Lefti
        https://github.com/LeftForRed


Piece textures taken from: 
    https://opengameart.org/content/chess-pieces-and-board-squares
    Copyright/Attribution Notice: 
    JohnPablok's improved Cburnett chess set.

