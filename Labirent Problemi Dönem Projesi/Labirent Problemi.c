#include <stdio.h>
#include <stdlib.h>
#include <windows.h>



int point = 0;

// labirentteki yapýlarý bi enumda belirtiyoruz.
enum terrain {
	empty,		//labirentteki boþ yerleri temsil ediyoruz.
	wall,		//labirentteki duvarlarý temsil ediyoruz.
	goal,		//labirentteki hedefi(çýkýþý) temsil ediyoruz.
	apple,		//labirentteki elmalarý temsil ediyoruz.
	crumb		//labirentte ilerlerken ki býraktýðýmýz kýrýntýlarý temsil ediyoruz.
};

//labirenti iki boyutlu bir matriste tutacaðýz.
//bu yüzden iki boyutlu bir matris tahsis ediyoruz.
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

//labirentte gezdiðimiz yerleri tutmak için
//ayrý bir matris tahsis ediyoruz.
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

//adý, satýr ve sütun deðerleri alýnan dosyadan, labirentimizi
//tahsis edilen matrise yazýyoruz.
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

//labirentin baþlangýç noktasýný buluyoruz ve bulunan noktayý 
//bir dizi olarak döndürüyoruz.
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

//labirentte gezilen yerleri visited matrisinde iþaretliyoruz
//ve iþaretlenen matrisi döndürüyoruz.
int** init_visited(char** maze, int rows, int cols)
{
	int** visited = alloc_visited(rows, cols);

	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			if (maze[i][j] == '|' || maze[i][j] == '-' || maze[i][j] == '+') {
				visited[i][j] = wall;			//duvarlar iþaretleniyor.
			} else if (maze[i][j] == 'c') {
				visited[i][j] = goal;			//hedef iþaretleniyor.
			} else if(maze[i][j] == 'O') {
				visited[i][j] = apple;			//elmalar iþaretleniyor.
			} else {
				visited[i][j] = empty;			//geri kalan yerler boþluk olarak iþaretleniyor.
			}
		}
	}
	return visited;
}	

//labirenti ekrana yazdýrýyoruz.
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

//labirette gezilen yerlerde kýrýntý býrakýyoruz.
//labirette kýrýntýlar '*' ile gösteriliyor.
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

//o anki toplam puaný ekrana yaazdýrýyoruz.
void print_point(int point)
{
	printf("\nPoint: %d", point);	
}

//Depth First Search algoritmasýný burda gerçekleþtiriyoruz.
int dfs(int row, int col, char** maze, int** visited, int rows, int cols)
{
	int* current = &visited[row][col];		// gezdiðimiz yerleri 'current' iþaretçisi ile geziyoruz.
	int m=0;								//çýkmaza gelip gelmediðimizi anlamak için bir m deðiþkeni tanýmlýyoruz.
	if (*current == goal) {					//hedefe geldiysek algoritma sonlanýyor.
		return 1;
	}
	
	if (*current == apple){					//elmalar ile karþýlaþmýþ isek puaný güncelliyoruz.
		point = point + 10;
	}
	
	if (*current == empty || *current == apple) {
		add_crumbs(maze, visited, rows, cols);		//gezdiðimiz yerlere kýrýntý býrakýyoruz.
		print_maze(maze, rows, cols);				//anlýk deðiþimleri göremek için labirenti  ve 
		print_point(point);							//puaný ekrana yazdýrýyoruz.
		Sleep(100);									// deðiþimleri yakalayabilmek için 100 milisaniye bekliyoruz ve
		system("cls");								// ekraný her seferinde temizliyoruz.
		*current = crumb;		
		if (visited[row][col-1] != wall && visited[row][col-1] != crumb){		// dört farklý yönde özyinelemeli olarak
			m=1;																// bulunduðumuz konumu kontrol ediyoruz.
			*current = crumb;													// eðer girdiysek kýrýntý býrakýyoruz ve 	
			if (dfs(row, col-1, maze, visited, rows, cols))						// m deðiiþkenine 1 deðerini atýyoruz.	
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
		if(m==0){									//çýkmaza girip girmediðimizi m deðerini kontrol ederek anlýyoruz.
			point = point - 5;						//ve duruma göre puanýmýzý güncelliyoruz.
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
	
	printf("--Welcome to the maze--\n\n");		// kullanýcýdan dosya adýný
	printf("Enter the filename: ");
	scanf("%s",fileName);
	printf("\nEnter the count of rows: ");		// labirentin satýr
	scanf("%d", &rows);
	printf("\nEnter the count of columns: ");	//ve sütun deðerlerini alýyoruz.
	scanf("%d", &cols);
	
	maze = get_maze(fileName, rows, cols);		//okunan dosyadan labirenti alýyoruz.
	visited = init_visited(maze, rows, cols);	// gezilen yerlerin iþaretlendiði matris
	start = find_start(maze, rows, cols);		// ve labirentin baþlangýç noktasýný buluyoruz.
	print_maze(maze, rows, cols);				// labirenti boþ bir þekilde yazýyoruz

	if (!dfs(start[0], start[1], maze, visited, rows, cols)) {	//dfs algoritmamýzý çalýþtýrýp çýkýþý 
		printf("No path to the goal could be found.\n");		//bulan yolu arýyoruz.
	} else {
		print_maze(maze, rows, cols);							//son olarak yolu bulunan labirenti 
		print_point(point);										//ve puaný yazdýrýyoruz.
		printf("\nThe End of  the Maze");
	}
	return 0;
}


