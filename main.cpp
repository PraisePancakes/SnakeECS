#include <iostream>
#include "snakeecs/benchmark/bmark_page_storage.cpp"

int main(int argc, char **argv)
{
    BMARK_PAGE_STORAGE_INSERT();
    BMARK_PAGE_STORAGE_GET();
    return 0;
}