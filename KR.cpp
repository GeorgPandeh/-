 #include<sys/wait.h>

#include<stdio.h>

#include<sys/types.h>

#include<stdlib.h>

#include<unistd.h>

#include<signal.h>

#include<time.h>

#include<sys/time.h>

#include<errno.h>

//переменные для хранение pid процессов

pid_t pid0,pid1,dd0,dd1,dd2,dd3,dd4,dd5,dd6,dd7,dd8;

//массивы для файловых дескрипторов неименованых каналов

int pipe31[2],pipe41[2],pipe51[2],pipe62[2],pipe73[2],pipe84[2],status;

int pipe61[2],pipe71[2],pipe81[2];

//прототипы функций

void vivod(int,int);

pid_t enter(void);//ветвление процесса с обработкой ошибок

void proc0();void proc1();void proc2();void proc3();void proc4();

void proc5();void proc6();void proc7();void proc8();

//обработчик сигналов SIGUSR1, SIGUSR2 и SIGTERM

static void handler1(int signo)

{

if(signo==SIGUSR1){vivod(1,0);}

else if(signo==SIGUSR2){ vivod(2,0);}

else if(signo==SIGTERM)

{

vivod(0,0);exit(0);

}

else printf("недопустимый сигнал %d ",signo);

}

//--------------------------main-----------------------------

int main(void)

{

pid0=pid1=dd0=dd1=dd2=dd3=dd4=dd5=dd6=dd7=dd8=-1;

//регистрация обработчика сигнала SIGUSR1

if(signal(SIGUSR1,handler1)==SIG_ERR)

{

perror("Невозможно обработать SIGUSR1");

exit(EXIT_FAILURE);

}

//регистрация обработчика сигнала SIGUSR2

if(signal(SIGUSR2,handler1)==SIG_ERR)

{

perror("Невозможно обработать SIGUSR2");

exit(EXIT_FAILURE);

}

//регистрация обработчика сигнала SIGTERM

if(signal(SIGTERM,handler1)==SIG_ERR)

{

perror("Невозможно обработать SIGTERM");

exit(EXIT_FAILURE);

}

nice(15);//уровень любезности

//Создание неименованых каналов для передачи pid

if(pipe(pipe31)==-1) {perror("Ошибка создания канала");exit(EXIT_FAILURE);}

if(pipe(pipe41)==-1) {perror("Ошибка создания канала");exit(EXIT_FAILURE);}

if(pipe(pipe51)==-1) {perror("Ошибка создания канала");exit(EXIT_FAILURE);}

if(pipe(pipe61)==-1) {perror("Ошибка создания канала");exit(EXIT_FAILURE);}

if(pipe(pipe62)==-1) {perror("Ошибка создания канала");exit(EXIT_FAILURE);}

if(pipe(pipe71)==-1) {perror("Ошибка создания канала");exit(EXIT_FAILURE);}

if(pipe(pipe73)==-1) {perror("Ошибка создания канала");exit(EXIT_FAILURE);}

if(pipe(pipe81)==-1) {perror("Ошибка создания канала");exit(EXIT_FAILURE);}

if(pipe(pipe84)==-1) {perror("Ошибка создания канала");exit(EXIT_FAILURE);}

dd0=getpid();

pid0=enter();//создать процесс1

if(pid0) //если поцесс 0

{

dd1=pid0;proc0();//номер процесса 1 для процесса 0

}

else //если поцесс 1

{

dd1=getpid();//номер процесса 1 для потомков

pid0=enter();//создать процесс2

if(pid0)//если поцесс 1

{

dd1=getpid();

dd2=pid0;proc1();//номер процесса 2 для процесса 1

}

else //если поцесс 2

{

dd2=getpid();//номер процесса 2 для потомков

pid0=enter();//создать процесс3

if(pid0){ //если процесс2

dd3=pid0;

pid1=enter();//создать процесс4

if(pid1)//если процесс2

{

dd4=pid1;proc2();//номер процесса 4 для процесса 2

}

else //если процесс4

{

dd2=getppid();//номер процесса 2 для потомков

dd4=getpid();

pid0=enter();//создать процесс5

if(pid0)//если процесс4

{

//номер процесса 5 для процесса 4

dd5=pid0;proc4();

}

else//если процесс5

{

//номер процесса 4 для потомков

dd5=getpid();proc5();

}

}

}

else//если процесс3

{

dd3=getpid();//номер процесса 3 для потомков

pid0=enter();//создать процесс6

if(pid0)//если процесс3

{

dd6=pid0;proc3();//номер процесса 6 для процесса 3

}

else //если процесс6

{

dd6=getpid();//номер процесса 6 для потомков

pid0=enter();//создать процесс7

if(pid0)//если процесс6

{

//номер процесса 7 для процесса 6

dd7=pid0;proc6();

}

else //если процесс7

{

dd7=getpid();//номер процесса 7 для потомков

pid0=enter();//создать процесс8

if(pid0)

{

dd8=pid0;proc7();//если процесс7

}

else //если процесс8

{

dd8=getpid();proc8();

}

}

}

}

}

}

return(0);

}

//------------------------------------------

pid_t enter()

{

pid_t pidd=fork();

if(pidd==-1){perror("Ошибка создания процесса");exit(1);}

else return(pidd);

}

//-------------------------------------------

void proc0()

{

while(waitpid(dd1,&status,WNOHANG)==0);

}

//----

void proc1()

{

int i;

read(pipe31[0],&dd3,sizeof(pid_t));

read(pipe41[0],&dd4,sizeof(pid_t));

read(pipe51[0],&dd5,sizeof(pid_t));

read(pipe61[0],&dd6,sizeof(pid_t));

read(pipe71[0],&dd7,sizeof(pid_t));

read(pipe81[0],&dd8,sizeof(pid_t));

while(getpgid(dd5)!=dd2);//Ждать перевода пр5 в другую группу

for(i=0;i<=101;i++)

{

kill(-dd2,SIGUSR2);

vivod(2,1);

}

sleep(2);

kill(dd8,SIGTERM);kill(dd7,SIGTERM);kill(dd6,SIGTERM);kill(-dd2,SIGTERM);

while(waitpid(dd2,&status,WNOHANG)==0);

}

//----

void proc2()

{

int i,j;

read(pipe62[0],&dd6,sizeof(pid_t));//pid процесса 6

setpgid(getpid(),getpid());

setpgid(dd3,getpid());

setpgid(dd4,getpid());

for(i=0;i<=101;i++)

{

kill(dd6,SIGUSR1);

vivod(1,1);

}

while(1) pause();

}

//----

void proc3()

{

int i,j;

write(pipe31[1],&dd3,sizeof(pid_t));

read(pipe73[0],&dd7,sizeof(pid_t));

for(i=0;i<=101;i++)

{

kill(dd7,SIGUSR1);

vivod(1,1);

}

while(1) pause();

}

//---------------------------------

void proc4()

{

int i;

write(pipe41[1],&dd4,sizeof(pid_t));

read(pipe84[0],&dd8,sizeof(pid_t));

for(i=0;i<=101;i++)

{

kill(dd8,SIGUSR1);

vivod(1,1);

}

while(1) pause();

}

//----------------------------

void proc5()

{

int j;

dd5=getpid();

write(pipe51[1],&dd5,sizeof(pid_t));//отправка своего pid процессу 1

while(getpgid(dd5)!=dd2)setpgid(dd5,dd2);//Ждать создания группы в пр2

while(1)pause();

}

//----

void proc6()

{

int j;

write(pipe61[1],&dd6,sizeof(pid_t));

write(pipe62[1],&dd6,sizeof(pid_t));

while(1) pause();

}

//----

void proc7()

{

int j;

write(pipe71[1],&dd7,sizeof(pid_t));

write(pipe73[1],&dd7,sizeof(pid_t));

while(1) pause();

}

//----

void proc8()

{

int i,j;

write(pipe81[1],&dd8,sizeof(pid_t));

write(pipe84[1],&dd8,sizeof(pid_t));

for(i=0;i<=101;i++)

{

kill(dd1,SIGUSR1);

vivod(1,1);

}

while(1) pause();

}

//-------------------------------

void vivod(int a,int flag)

{

time_t t;

time(&t);

struct timeval tm1;

gettimeofday(&tm1,NULL);

if(flag==1)

{

if(a==2)

printf("ppid:%d pid:%d послал SIGUSR2 время:%ldмксек %s\n",getppid(),getpid(),tm1.tv_usec,ctime(&t));

if(a==1)

{

printf("ppid:%d pid:%d послал SIGUSR1 время:%ldмксек %s\n",getppid(),getpid(),tm1.tv_usec,ctime(&t));

}

if(a==0)

{

printf("ppid:%d pid:%d послал SIGTERM время:%ldмксек %s\n",getppid(),getpid(),tm1.tv_usec,ctime(&t));

}

}

else

{

if(a==2)

printf("ppid:%d pid:%d принял SIGUSR2 время:%ldмксек %s\n",getppid(),getpid(),tm1.tv_usec,ctime(&t));

if(a==1)

{

printf("ppid:%d pid:%d принял SIGUSR1 время:%ldмксек %s\n",getppid(),getpid(),tm1.tv_usec,ctime(&t));

}

if(a==0)

{

printf("ppid:%d pid:%d завершил работу по сигналу SIGTERM время:%ldмксек %s\n",getppid(),getpid(),tm1.tv_usec,ctime(&t));

}

}

}
