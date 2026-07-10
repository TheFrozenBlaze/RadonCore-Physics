#include <window.h>
#include <thread>
int main() {
    
    Window win;
    win.Init();
    win.Mainloop();
    win.Clean();
}