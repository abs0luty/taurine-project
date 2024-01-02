typedef int eboueur_object_kind_t;

typedef struct eboueur_object {
    eboueur_object_kind_t kind;
    unsigned char marked;

    struct eboueur_object *next;

    void *data;
} eboueur_object_t;

typedef struct {
    eboueur_object_t* stack[1024];
    size_t stack_size;

    eboueur_object_t* first_object;

    size_t objects_count;
} eboueur_gc_manager_t;

eboueur_gc_manager_t* eboueur_init_gc_manager();

void eboueur_destroy_gc_manager(eboueur_gc_manager_t* manager);
