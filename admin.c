#include "admin.h"
void options_menu(){
    int i;
    for(i=0;i<sizeof(options)/sizeof(char*);i++){
        fprintf(stdout, "%s\n", options[i]);
    }
    int opt;
    fprintf(stdout, "Optiune dorita...>");
    scanf("%d", &opt);
    switch(opt){
        case 0: // opresc client admin
            opt0();
            break;
        case 1:
            send_option(opt);
            break;
        case 2:
            send_option(opt);
            break;
        case 3:
            send_option(opt);
            break;
        case 4:
            send_option(opt);
            break;
        default:
            fprintf(stdout, "Optiune gresit aleasa! Incercati din nou...");
            break;
    }
}
int main(){
    options_menu();
}