#include <stdio.h>
#include <stdlib.h>
#include <windows.h>



int point = 0;

// labirentteki yap�lar� bi enumda belirtiyoruz.
enum terrain {
	empty,		//labirentteki bo� yerleri temsil ediyoruz.
	wall,		//labirentteki duvarlar� temsil ediyoruz.
	goal,		//labirentteki hedefi(��k���) temsil ediyoruz.
	apple,		//labirentteki elmalar� temsil ediyoruz.
	crumb		//labirentte ilerlerken ki b�rakt���m�z k�r�nt�lar� temsil ediyoruz.
};

//labirenti iki boyutlu bir matriste tutaca��z.
//bu y�zden iki boyutlu bir matris tahsis ediyoruz.
char** alloc_maze(int rows, int cols)
{
	char** maze;
	maze = malloc(rows * sizeof(char*));
	
	int i;
	for (i = 0; i < rows; ++i){
		maze[i] = malloc(cols * sizeof(char*));
	}
	return maze;
}

//labirentte gezdi�imiz yerleri tutmak i�in
//ayr� bir matris tahsis ediyoruz.
int** alloc_visited(int rows, int cols)
{
	int** visited;
	visited = malloc(rows * sizeof(int*));
	int i;
	for (i = 0; i < rows; ++i){
		visited[i] = malloc(cols * sizeof(int*));
	}
	return visited;
}

//ad�, sat�r ve s�tun de�erleri al�nan dosyadan, labirentimizi
//tahsis edilen matrise yaz�yoruz.
char** get_maze(char* file_name, int rows, int cols)
{
	char c;
	
	FILE* maze_file = fopen(file_name, "r");

	char** maze = alloc_maze(rows, cols);
	
	int i,j;

	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {

			if ((c = getc(maze_file)) == '\n') {
				c = getc(maze_file);
			}

			maze[i][j] = c;
		}
	}
	
	fclose(maze_file);
	return maze;
}

//labirentin ba�lang�� noktas�n� buluyoruz ve bulunan noktay� 
//bir dizi olarak d�nd�r�yoruz.
int* find_start(char** maze, int rows, int cols){
	int i,j;
	int* start;
	start = malloc(2 * sizeof(int*));

	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			
			if (maze[i][j] =='b') {
				start[0] = i;
				start[1] = j;
			}
		}
	}
	
	return start;
}

//labirentte gezilen yerleri visited matrisinde i�aretliyoruz
//ve i�aretlenen matrisi d�nd�r�yoruz.
int** init_visited(char** maze, int rows, int cols)
{
	int** visited = alloc_visited(rows, cols);

	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			if (maze[i][j] == '|' || maze[i][j] == '-' || maze[i][j] == '+') {
				visited[i][j] = wall;			//duvarlar i�aretleniyor.
			} else if (maze[i][j] == 'c') {
				visited[i][j] = goal;			//hedef i�aretleniyor.
			} else if(maze[i][j] == 'O') {
				visited[i][j] = apple;			//elmalar i�aretleniyor.
			} else {
				visited[i][j] = empty;			//geri kalan yerler bo�luk olarak i�aretleniyor.
			}
		}
	}
	return visited;
}	

//labirenti ekrana yazd�r�yoruz.
void print_maze(char** maze, int rows, int cols)
{
	int i, j;
	printf("-- Maze --\n\n");
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}	
	printf("\n");
	
}

//labirette gezilen yerlerde k�r�nt� b�rak�yoruz.
//labirette k�r�nt�lar '*' ile g�steriliyor.
void add_crumbs(char** maze, int** visited, int rows, int cols)
{
	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			if (maze[i][j] != 'b' && visited[i][j] == crumb) {
				maze[i][j] = '*';
			}
		}
	}
}

//o anki toplam puan� ekrana yaazd�r�yoruz.
void print_point(int point)
{
	printf("\nPoint: %d", point);	
}

//Depth First Search algoritmas�n� burda ger�ekle�tiriyoruz.
int dfs(int row, int col, char** maze, int** visited, int rows, int cols)
{
	int* current = &visited[row][col];		// gezdi�imiz yerleri 'current' i�aret�isi ile geziyoruz.
	int m=0;								//��kmaza gelip gelmedi�imizi anlamak i�in bir m de�i�keni tan�ml�yoruz.
	if (*current == goal) {					//hedefe geldiysek algoritma sonlan�yor.
		return 1;
	}
	
	if (*current == apple){					//elmalar ile kar��la�m�� isek puan� g�ncelliyoruz.
		point = point + 10;
	}
	
	if (*current == empty || *current == apple) {
		add_crumbs(maze, visited, rows, cols);		//gezdi�imiz yerlere k�r�nt� b�rak�yoruz.
		print_maze(maze, rows, cols);				//anl�k de�i�imleri g�remek i�in labirenti  ve 
		print_point(point);							//puan� ekrana yazd�r�yoruz.
		Sleep(100);									// de�i�imleri yakalayabilmek i�in 100 milisaniye bekliyoruz ve
		system("cls");								// ekran� her seferinde temizliyoruz.
		*current = crumb;		
		if (visited[row][col-1] != wall && visited[row][col-1] != crumb){		// d�rt farkl� y�nde �zyinelemeli olarak
			m=1;																// bulundu�umuz konumu kontrol ediyoruz.
			*current = crumb;													// e�er girdiysek k�r�nt� b�rak�yoruz ve 	
			if (dfs(row, col-1, maze, visited, rows, cols))						// m de�ii�kenine 1 de�erini at�yoruz.	
				return 1;
		} 
		if (visited[row+1][col] != wall && visited[row+1][col] != crumb){
			m=1;
			*current = crumb;										
			if (dfs(row + 1, col, maze, visited, rows, cols))									
				return 1;
		} 
		if (visited[row][col+1] != wall && visited[row][col+1] != crumb){
			m=1;
			*current = crumb;							
			if (dfs(row, col+1, maze, visited, rows, cols))									
				return 1;		
		}
		if (visited[row-1][col] != wall && visited[row-1][col] != crumb){
			m=1;
			*current = crumb;
			if (dfs(row - 1, col, maze, visited, rows, cols))									
				return 1;	
		}
		if(m==0){									//��kmaza girip girmedi�imizi m de�erini kontrol ederek anl�yoruz.
			point = point - 5;						//ve duruma g�re puan�m�z� g�ncelliyoruz.
		}
	} 			
	
	return 0;
}

// main fonksiyonumuz.
int main() {
	char** maze;
	int** visited;
	int* start;
	char fileName[100];
	int rows;
	int cols;
	
	printf("--Welcome to the maze--\n\n");		// kullan�c�dan dosya ad�n�
	printf("Enter the filename: ");
	scanf("%s",fileName);
	printf("\nEnter the count of rows: ");		// labirentin sat�r
	scanf("%d", &rows);
	printf("\nEnter the count of columns: ");	//ve s�tun de�erlerini al�yoruz.
	scanf("%d", &cols);
	
	maze = get_maze(fileName, rows, cols);		//okunan dosyadan labirenti al�yoruz.
	visited = init_visited(maze, rows, cols);	// gezilen yerlerin i�aretlendi�i matris
	start = find_start(maze, rows, cols);		// ve labirentin ba�lang�� noktas�n� buluyoruz.
	print_maze(maze, rows, cols);				// labirenti bo� bir �ekilde yaz�yoruz

	if (!dfs(start[0], start[1], maze, visited, rows, cols)) {	//dfs algoritmam�z� �al��t�r�p ��k��� 
		printf("No path to the goal could be found.\n");		//bulan yolu ar�yoruz.
	} else {
		print_maze(maze, rows, cols);							//son olarak yolu bulunan labirenti 
		print_point(point);										//ve puan� yazd�r�yoruz.
		printf("\nThe End of  the Maze");
	}
	return 0;
}


