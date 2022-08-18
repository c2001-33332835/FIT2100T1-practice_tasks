#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define input 0
#define output 1
#define error 2

struct Unit{
    int unitid;
    char unitcode[8];
};

int main(){
    char filebufferid[sizeof(int)];
    char filebuffercode[8];
    struct Unit unit;

    // open the files
    int filedesc = open("recordfile", O_RDONLY);
    if (filedesc < 0){
        write(error, "task2: Unable to open source file\n", 34);
        return 1;
    }
    int oddfd = open("odd", O_WRONLY | O_CREAT | O_TRUNC, 0664);
    int evenfd = open("even", O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (oddfd < 0 || evenfd < 0){
        write(error, "task2: unable to open result files\n", 35);
        return 1;
    }

    for (int i = 0; i < 4; i ++){
        lseek(filedesc, i * sizeof(struct Unit), SEEK_SET);
        read(filedesc, filebufferid , sizeof(int)); 
        read(filedesc, filebuffercode , sizeof(char) * 8);
        write(output, filebuffercode, 8);
        
        unit.unitid = (int) *filebufferid;
        strcpy(unit.unitcode, filebuffercode);

        write(output, "\n", 1);

        lseek((unit.unitid % 2 == 0) ? evenfd : oddfd, ((int) (unit.unitid / 2)) * sizeof(char) * 8, SEEK_SET);
        write((unit.unitid % 2 == 0) ? evenfd : oddfd, unit.unitcode, sizeof(char) * 8);
    }

    close(filedesc);
    close(oddfd);
    close(evenfd);
    return 0;
}
