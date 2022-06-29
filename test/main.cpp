#include <iostream>
#include "datapak.hpp"
#include "wren.hpp"

static void writeFn(WrenVM* vm, const char* text)
{
    printf("%s", text);
}

void errorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
    switch (errorType)
    {
        case WREN_ERROR_COMPILE:
        {
        printf("[%s line %d] [Error] %s\n", module, line, msg);
        } break;
        case WREN_ERROR_STACK_TRACE:
        {
        printf("[%s line %d] in %s\n", module, line, msg);
        } break;
        case WREN_ERROR_RUNTIME:
        {
        printf("[Runtime Error] %s\n", msg);
        } break;
    }
}



int main()
{
    Datapak dat("bin/file.datapak");
    dat.purge();

    WrenConfiguration config;
    wrenInitConfiguration(&config);
        config.writeFn = &writeFn;
        config.errorFn = &errorFn;
    WrenVM* vm = wrenNewVM(&config);

    const char* module = "main";
    const char* script = "System.print(34)\0";
    dat.write("main.wren", script);
    std::cout << dat.read("main.wren") << std::endl;
    WrenInterpretResult result = wrenInterpret(vm, module, dat.read("main.wren").c_str());

    switch (result) {
        case WREN_RESULT_COMPILE_ERROR:
        { printf("Compile Error!\n"); } break;
        case WREN_RESULT_RUNTIME_ERROR:
        { printf("Runtime Error!\n"); } break;
        case WREN_RESULT_SUCCESS:
        { printf("Success!\n"); } break;
    }

    wrenFreeVM(vm);
}