#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fstream>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUF_SIZE  4096
char buffer [BUF_SIZE];
using namespace std; 

void mycopyfile (const char *source_file, const char *destination_file)
{   
    int infd, outfd;
    ssize_t bytes;
    
    infd = open (source_file, O_RDONLY);
    if (infd == -1) 
       {
           fprintf (stderr, "Не открывается входящий файл ""(%s)\n", source_file);
           exit(1);
       }
    outfd = open (destination_file, O_WRONLY | O_CREAT | O_TRUNC, 0640);
    if (outfd == -1) 
    {
        fprintf (stderr, "Не открывается исходящий файл ""(%s)\n", destination_file);
      	exit(2);
    }
    while ((bytes = read(infd, buffer, BUF_SIZE)) > 0)
        write (outfd, buffer, bytes);

    close (infd);
    close (outfd);

}
int main (void)
{
    DIR *dir1, *dir2;
struct dirent *entry1, *entry2;
struct stat *buf; // Структура Stat - информация о файле
    int flag, flag_name, k, i, m, cur, NPROC;
    string in_str, out_str;
    pid_t pids[1024], pid_end;
    int file_o;
    char str_dir1[80], str_dir2[80]; // Массив для названий каталогов
    
    printf("Dir1: \n"); // Введите Dir1
    scanf("%s", str_dir1); // Название в переменную str_dir1
    printf("Dir2: \n"); // Введите Dir2
    scanf("%s", str_dir2); // Название в переменную str_dir2
    printf("Число процессов N: \n"); // Введите число процессов
    scanf("%d", &NPROC); // Число процессов по адресу NPROC
       
    dir1 = opendir (str_dir1); // Результат открытия каталога Dir1
    dir2 = opendir (str_dir2); // Результат открытия каталога Dir2
    if ((!dir1)||(!dir2)) // Если неудачное открытие
      {
        perror ("diropen");
        return 1;
      }
      
    i = 0;
       
    while ((entry1 = readdir(dir1)) != NULL) // Пока нет ошибки в Dir1
       {
        flag = 1;
        while ((entry2 = readdir(dir2)) != NULL) // Пока нет ошибки в Dir2
          {
           for (flag_name = 1, k = 0; (k < MAXNAMLEN) &&  
                         (entry1->d_name[k] != '\0');k++)
              {  
               if(entry1->d_name[k] != entry2->d_name[k]) // Если кат. не равны...
                  flag_name = 0;
              }
           if ((flag_name == 1) && (entry1->d_type == entry2->d_type) &&
                                   (entry1->d_reclen == entry2->d_reclen)) // Если равны, то...
              flag = 0;
          }
        
          rewinddir (dir2); // Сброс потока кат., дескриптор указ. на начало
          
          if (flag)  // Если 1
                       {
              if (i == NPROC)  // Если равен числу процессов
                {
                 pid_end = wait(NULL);//wait end process (Завершение)       
                 for (m = 0; (m < NPROC) && (pids[m] != pid_end); m++); // Поиск ном. оконч. процесса
                 cur = m;
                }
                else cur = i;
                
              pids[cur] = fork();  // Порождается процесс
              if (pids[cur] < 0) 
                 {
                  perror("fork");
                  return 1;
                 } 
                 else 
                    if (!pids[cur]) 
                       {     

                        in_str = str_dir1;         // Присвоить назв. кат.
                        in_str += "/";             // Добавить косую черту
                        in_str +=  entry1->d_name; // Присв. назв. файла
                        out_str = str_dir2;        // Присвоить назв. кат.
                        out_str += "/";            // Добавить косую черту
                        out_str += entry1->d_name; // Присвоить назв. файла
                        mycopyfile (in_str.c_str(), out_str.c_str()); // Копир.
                        file_o = open (in_str.c_str(), O_RDONLY);
                        fstat (file_o, buf);
                        close (file_o);
                        printf ("%d %s %ld \n", getpid(), entry1->d_name, buf->st_size);// pid, название, размер  
			free(buf);
                        if (i < NPROC) i++;
                       }
             }
       }
    closedir (dir1);
    closedir (dir2);
    
    /*wait end all process */
    for (i = 0; i < NPROC; i++)
       waitpid (pids[i], NULL, 0);
    return 0;
};
