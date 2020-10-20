set(FILES
    board.cpp
    board.h
    chessGame.cpp
    chessGame.h
    main.cpp
    piece.cpp
    piece.h
    pieceTextures.cpp
    pieceTextures.h
    )

add_executable(${CMAKE_PROJECT_NAME} ${FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} sfml-graphics sfml-window sfml-system)
