#include <iostream>

int rows, cols;
int const MAX_AGE = 6;

void inputGridSize(){
    int r, c;
    std::cout << "Enter the sizes of the grid" << std::endl;
    std::cin >> r >> c;
    while ((r < 10 || r > 30) || (c < 10 || c > 30))
    {
        std::cout << "Invalid sizes. Try again:" << std::endl;
        std::cin >> r >> c;
    }
    rows = r;
    cols = c;
}


void initializeGrid(bool** grid, int** resilience, int** age){
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            grid[i][j] = false;
            resilience[i][j] = 0;
            age[i][j] = 0;
        }
    }
}


void InitializeRandomCells(bool** grid){
    //int randCellsCount = (rows * cols) / 4; //Около 25% от клетките
    //random_number = min + (rand() % (max − min + 1)) 
    int randCellsCount = (rand() % ((rows * cols) + 1)); //използвам горната формула за произволно число в интервал като min = 0, max = rows*cols
    for (int i = 0; i < randCellsCount; i++)
    {
        int x = rand() % rows;
        int y = rand() % cols;
        grid[x][y] = true;
    }
}


void cellsFromUser(bool** grid){
    //Координати на клетките които ще са живи от потребителя
    std::cout << "How many live cels you want to enter?" << std::endl;
    int count;
    std::cin >> count;

    for (int i = 0; i < count; i++)
    {
        int x, y;
        std::cin >> x >> y;
        if (x >= 0 && x < rows && y >= 0 && y < cols)
        {
            grid[x][y] = true;    
        } else {
            std::cout << "Invalid coordinates!" << std::endl;
            std::cout << "Try again:" << std::endl;
            i--;
        }
    }
}


//устойчивост на клетките
void initializeResilience(bool** grid, int** resilience){
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (grid[i][j])
            {
                int aliveCellsInRow = 0;
                int aliveCellsInCol = 0;

                //броя живите клетки в съответната колона
                for (int row = 0; row < rows; row++)
                {
                    if (grid[row][j]) aliveCellsInCol++;
                }
                
                //броя живите клетки в съответния ред
                for (int col = 0; col < cols; col++)
                {
                    if (grid[i][col]) aliveCellsInRow++;    
                }
                
                //сбора им е равен на устойчивостта на клетката
                resilience[i][j] = aliveCellsInRow + aliveCellsInCol;
            }
        } 
    }
}


int countLiveNeighbors(bool** grid, int x, int y){
    int counter = 0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0) continue;

            int neighborRow = x + i;
            int neighborCol = y + j;
        
            //проверявам дали съседните ред и колона са в полето и ако са дали клетката е жива
            if (neighborRow >= 0 && neighborRow < rows &&
                neighborCol >= 0 && neighborCol < cols &&
                grid[neighborRow][neighborCol])
            {
                counter++;
            }
        }
    }
    
    return counter;
}


void copyGrid(bool** input, bool** dest){
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            dest[i][j] = input[i][j];
        }
    }
}


//приемам, че устойчивостта е с по-голяма важност от възрастта на клетката
void nextGen(bool** grid, bool** nextGrid, int** resilience, int** age){
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int liveNeighbors = countLiveNeighbors(grid, i, j);
            if (grid[i][j]) // ако клетката е жива
            {
                if (liveNeighbors < 2 || liveNeighbors > 3)
                {
                    if (resilience[i][j] > 0)
                    {
                        nextGrid[i][j] = true;
                        resilience[i][j]--;
                        age[i][j]++;
                    } else {
                        nextGrid[i][j] = false;
                    }
                } else {
                    nextGrid[i][j] = true;
                    age[i][j]++;
                    if (age[i][j] >= MAX_AGE) //проверка за възрастта на клетката
                    {
                        nextGrid[i][j] = false;
                        age[i][j] = 0;
                    }
                }
            } else { // ако клетката е мъртва
                if (liveNeighbors == 3)
                {
                    nextGrid[i][j] = true;
                } else {
                    nextGrid[i][j] = false;
                }
            }
        }
    }
}


void printGrid(bool** grid){
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << (grid[i][j] ? '#' : '.') << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

//проверявам дали има живи клетки в поколението
bool isGridEmpty(bool** grid) {
    bool empty = true;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j]) {
                empty = false;
            }
        }
    }
    return empty;
}


int main(){
    
    //Променлива за нова игра или прекъсване на програмата
    int newGame;

    do
    {

    //Въвеждане на размерите на полето
    inputGridSize();

    //инициализиране на нужните двумерни масиви
    //използвам динамични масиви, защото само така даваме възможност на потребителя да дава размери на полето 
    bool** grid = new bool*[rows];
    bool** nextGrid = new bool*[rows];
    int** resilience = new int*[rows];
    int** age = new int*[rows];

    for (int i = 0; i < rows; i++)
    {
        grid[i] = new bool[cols];
        nextGrid[i] = new bool[cols];
        resilience[i] = new int[cols];
        age[i] = new int[cols];
    }
    

    initializeGrid(grid, resilience, age);
    InitializeRandomCells(grid);
    cellsFromUser(grid);
    initializeResilience(grid, resilience);


    //The game itself
    std::cout << "How many generations you want to see?" << std::endl;
    int generations;
    std::cin >> generations;

    //Принтирам броя поколения въведени от потребителя
    for (int gen = 0; gen < generations; gen++)
    {
        std::cout << "Gen " << gen + 1 << ":" << std::endl;
        printGrid(grid);

        nextGen(grid, nextGrid, resilience, age);
        copyGrid(nextGrid, grid);
    }
    
    //Ако все още има живи клетки на дадена итерация, се избира дали да се визуализират и останалия брой поколения
    if (!isGridEmpty(grid))
    {
        std::cout << "Do you want to see the rest of the generations? (1 - Yes, 0 - No)" << std::endl;
        int choice;
        std::cin >> choice;

        if (choice == 1)
        {
            //Принтирам останалите поколения с живи клетки
            int counter = 1;
            while (!isGridEmpty(grid))
            {
                std::cout << "Gen " << generations + counter << ":" << std::endl;
                printGrid(grid);
                counter++;

                nextGen(grid, nextGrid, resilience, age);
                copyGrid(nextGrid, grid);
            }   
        }
        
    } else {
        std::cout << "The next generation is empty" << std::endl;
    }
    

    //освобождаваме използваната памет
    for (int i = 0; i < rows; ++i) {
        delete[] grid[i];
        delete[] nextGrid[i];
        delete[] resilience[i];
        delete[] age[i];
    }
    delete[] grid;
    delete[] nextGrid;
    delete[] resilience;
    delete[] age;


    //Възможност за започване на нова игра или цялостно прекъсване на програмата
    std::cout << "Do you want to play again? (1 - Yes, 0 - No)" << std::endl;
    std::cin >> newGame;

    } while (newGame == 1);

}