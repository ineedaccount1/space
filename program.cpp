
#include <algorithm>
#include <iostream>
#include <random>
#include <cstdint>
#include <chrono>

class PuzzleGenerator
{
private:
    std::int32_t m_playArray[16]{};
    std::int16_t m_arrayLength{16};

public:
    void initalizeAndShuffle()
    {
        for (int i = 0; i < m_arrayLength; i++)
        {
            m_playArray[i] = i;
        }

        std::mt19937 mt{std::random_device{}()};

        std::cout << "\nGenerating random directions......\n\n";

        std::shuffle(&m_playArray[0], &m_playArray[m_arrayLength], mt);
    }

    friend std::ostream &operator<<(std::ostream &stream, const PuzzleGenerator &array)
    {
        // get the squere root of the array length because its 4x4 (16) its 4.
        int row_length = std::sqrt(array.m_arrayLength);
        // for loop through the elements of the array.
        for (int i = 0; i < array.m_arrayLength; i++)
        {
            // if its an single digit. (its between 1 and 10)
            if (array.m_playArray[i] > 0 && array.m_playArray[i] < 10)
            {
                stream << "  " << array.m_playArray[i];
            }// if it has more than 1 digit
            else if (array.m_playArray[i] >= 10)
            {
                stream << " " << array.m_playArray[i];
            }
            else // if none of these. then that means that its 0
            {
                stream << "   ";
            }// print a new line every 4 elements.
            if ((i + 1) % row_length == 0)
            {
                stream << std::endl;
            }
            else
            {
                stream << " ";
            }
        }

        return stream;
    }

    friend class PuzzleBoard;
    friend class Play;
};

class PuzzleBoard
{
private:
    PuzzleGenerator puzzle;

public:
    PuzzleBoard(const PuzzleGenerator &value) : puzzle{value} {}
    // player is always the value of 0, so we can search for it.
    int searchPlayer()
    {
        int player;
        auto arr = puzzle.m_playArray;
        auto find{std::find(arr, arr + puzzle.m_arrayLength, 0)};
        if (find != arr + puzzle.m_arrayLength)
        {
            player = find - arr;
        }
        else
        {
            std::cerr << "Cannot find 0, an error with the program has occurred!!\n";
        }

        return player;
    }

    void controlLeft(int index)
    {
        // check if the player cant go left
        if (index == 0 || index == 4 || index == 8 || index == 12)
        {
            std::cerr << "Cannot move left!!\n";
        }
        else
        {
            // swap the element with the one before it (its left unless your braindead)
            std::swap(puzzle.m_playArray[index], puzzle.m_playArray[index - 1]);
        }
    }

    void controlRight(int index)
    {
        // check if the player cant go right
        if (index == 3 || index == 7 || index == 11 || index == 15)
        {
            std::cerr << "Cannot move Right!!\n";
        }
        else
        {
            // swap the element with the one after it.
            std::swap(puzzle.m_playArray[index], puzzle.m_playArray[index + 1]);
        }
    }

    void controlTop(int index)
    {
        // check if the player cant go Top
        if (index == 0 || index == 1 || index == 2 || index == 3)
        {
            std::cerr << "Cannot move Top!!\n";
        }
        else
        {
            // by substracting 4 to it (because its a 4 by 4), we can go top
            std::swap(puzzle.m_playArray[index], puzzle.m_playArray[index - 4]);
        }
    }

    void controlBottom(int index)
    {
        // check if the player cant go bottom
        if (index == 12 || index == 15 || index == 14 || index == 13)
        {
            std::cerr << "Cannot move Bottom!!\n";
        }
        else
        {
            // by adding 4 to it (because its a 4 by 4), we can go bottom
            std::swap(puzzle.m_playArray[index], puzzle.m_playArray[index + 4]);
        }
    }

    bool checkIfWon(const PuzzleGenerator &source)
    {
        bool won = false;
        if (source.m_playArray == puzzle.m_playArray)
        {
            won = true;
        }

        return won;
    }

    friend class Play;
};

class Play
{
private:
    PuzzleBoard m_board;
    PuzzleGenerator Wpuzzle;

public:
    Play(const PuzzleBoard &puzzle) : m_board{puzzle} {}

    auto winingPuzzle()
    {

        for (int i{0}; i < Wpuzzle.m_arrayLength; i++)
        {
            Wpuzzle.m_playArray[i] = i;
        }

        return Wpuzzle.m_playArray;
    }

     void play()
    {
        std::cout << '\n';
        int player = m_board.searchPlayer();
        while (true)
        {
            std::cout << "Enter control: ";
            char input;
            std::cin >> input;
            std::cin.ignore(100, '\n');

            std::cout << "\n\n\n";

            switch (input)
            {
            case 'w':
                m_board.controlTop(player);
                std::cout << '\n';
                player = m_board.searchPlayer();
                std::cout << m_board.puzzle << "\n\n";
                if (winingPuzzle() == m_board.puzzle.m_playArray)
                {
                    std::cout << "YOU WON!!\n";
                    return;
                }

                break;

            case 's':
                m_board.controlBottom(player);
                std::cout << '\n';
                player = m_board.searchPlayer();
                std::cout << m_board.puzzle << "\n\n";
                if (winingPuzzle() == m_board.puzzle.m_playArray)
                {
                    std::cout << "YOU WON!!\n";
                    return;
                }

                break;

            case 'd':
                m_board.controlRight(player);
                std::cout << '\n';
                player = m_board.searchPlayer();
                std::cout << m_board.puzzle << "\n\n";
                if (winingPuzzle() == m_board.puzzle.m_playArray)
                {
                    std::cout << "YOU WON!!\n";
                    return;
                }

                break;

            case 'a':
                m_board.controlLeft(player);
                std::cout << '\n';
                player = m_board.searchPlayer();
                std::cout << m_board.puzzle << "\n\n";
                if (winingPuzzle() == m_board.puzzle.m_playArray)
                {
                    std::cout << "YOU WON!!\n";
                    return;
                }

                break;
            case 'q':
                std::cerr << "Bye!\n\n";
                return;

            default:
                std::cerr << "Invalid control!\n";
                break;
            }
        }
    }

    PuzzleGenerator getWiningPuzzle(){
        return Wpuzzle;
    }
};

int main()
{
    // this is for display only
    PuzzleGenerator b;
    Play display{b};
    std::cout << "Welcome to the 14-puzzle game, you have to move the empty squere until every number is in order. heres how the puzzle has to be :\n ";
    display.winingPuzzle();
    std::cout << display.getWiningPuzzle();
    std::cout << '\n';

    std::cout << "|| Press any key (To continue) || Press Q (To quit) || : ";
    char input;
    std::cin >> input;
    std::cin.ignore(100, '\n');
    std::cout << '\n';

    if (input == 'Q' || input == 'q'){
        std::cout << "Bye!\n";
        return 0;
    }
    std::cout << "Use (W) (A) (S) (D) to move, press (Q) to quit, WITHOUT CAPSLOCK\n";
    std::cout << '\n';
    PuzzleGenerator a;
    a.initalizeAndShuffle();
    std::cout << a;
    Play player1{a};
    player1.winingPuzzle();
    player1.play();

    return 0;
}