#include "eboueur.h"

eboueur_gc_manager_t* eboueur_init_gc_manager()
{
    eboueur_gc_manager_t* manager = malloc(sizeof(eboueur_gc_manager_t));
    manager->objects_count = 0;
    manager->stack_size = 0;
    manager->first_object = (void*) 0;
    return manager;
}
