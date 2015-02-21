
#include <stdlib.h>

#include <allegro.h>

#include <ec_allegro.h>

int main(int ac, char *av[])
{
    (void)ac, (void)av;

    ec_allegro_init();

    allegro_message("Tout marche !");

    return EXIT_SUCCESS;
}
