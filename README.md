# **Two player chess made with C++ and SFML(2D Graphics).**


You need to have SFML installed to compile.
https://www.sfml-dev.org/download/sfml/2.5.1/

Compile Instructions:

Windows MinGW:
    
    1) g++ -c main.cpp board.cpp piece.cpp pieceTextures.cpp chessGame.cpp -I <sfml-install-path>\include -mwindows
    2) g++ main.o board.o piece.o pieceTextures.o chessGame.o -mwindows -L <sfml-install-path>\lib -lsfml-graphics -lsfml-window -lsfml-system
    3) You need to have sfml-graphics-2.dll , sfml-system-2.dll and sfml-window-2.dll in the same folder as the exe
    4) Open a.exe


Linux GCC:

    1) g++ -c main.cpp board.cpp piece.cpp pieceTextures.cpp chessGame.cpp
    2) g++ main.o board.o piece.o pieceTextures.o chessGame.o -lsfml-graphics -lsfml-window -lsfml-system
    3) ./a.out

CMake:

    1) mkdir build && cd build
    2) cmake ..
Your executable should now be in build/src/Chess-SFML. To properly run it you need to place it near the Textures folder.


## **Made By Lefti: https://github.com/LeftForRed**


Piece textures taken from: 
    https://opengameart.org/content/chess-pieces-and-board-squares
    Copyright/Attribution Notice: 
    JohnPablok's improved Cburnett chess set.

