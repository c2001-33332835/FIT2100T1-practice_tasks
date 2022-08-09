#include <stdio.h>
#include <string.h>

typedef struct {
    char* FacID;
    int UnitID;
} UnitCode;

int unitcodecomp(UnitCode* x, UnitCode* y){
    return (strcmp(x->FacID, y->FacID)) && (x->UnitID == y->UnitID);
}

// this program creates 2 unit codes and compares them
int main(){
    // FIT2100 (Operating Systems)
    UnitCode code_1;
    code_1.FacID = "FIT";
    code_1.UnitID = 2100;

    // FIT1045 (Algo and ProgFun with Python)
    UnitCode code_2;
    code_2.FacID = "FIT";
    code_2.UnitID = 1045;

    // compare them (should be false)
    printf("result (0=false, 1=true): %d", unitcodecomp(&code_1, &code_2));
}
