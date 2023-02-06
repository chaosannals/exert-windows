#include <Windows.h>
#include <stdio.h>
#include <JSON_checker.h>
#include "msys2.h"

BOOL CheckConfJson(FILE *f)
{
    JSON_checker jc = new_JSON_checker(20);
    for (;;) {
        int c = fgetc(f);
        if (c <= 0) {
            break;
        }
        if (!JSON_checker_char(jc, c)) {
            return FALSE;
        }
    }
    return TRUE;
}


INT WINAPI wWinMain(
    _In_ HINSTANCE instance,
    _In_opt_ HINSTANCE prevInstance,
    _In_ LPWSTR    cmdLine,
    _In_ int       cmdShow
) {
    InitMsys2();

    FILE* f = fopen("conf.json", "r");
    if (f == NULL) {
        return -2;
    }
    BOOL cr = CheckConfJson(f);
    fclose(f);
    if (cr) {

    }
    else {
        return -1;
    }
    return 0;
}