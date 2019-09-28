#include "window.h"

window::window()
{
    QFrame* frame = new QFrame{ this };
    setCentralWidget(frame);
}
