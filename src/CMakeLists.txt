
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "../include/*.h")
add_executable (SudokuSolver ${SOURCES} ${HEADERS} )
