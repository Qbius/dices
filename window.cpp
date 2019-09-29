#include "window.h"

#include "scoreboards.hpp"
QGridLayout* gridder::layout_ = nullptr;
QWidget* gridder::parent_ = nullptr;
int gridder::row_ = 0;
int gridder::col_ = 0;

window::window()
{
    QFrame* frame = new QFrame{ this };
    QGridLayout* main_layout = new QGridLayout;

    gridder::set(main_layout, this);
    construct_scoreboard();

    frame->setLayout(main_layout);
    setCentralWidget(frame);
}
