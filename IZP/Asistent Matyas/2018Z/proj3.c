/**
 * Kostra programu pro 3. projekt IZP 2018/19
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 * Vytvoøil Jiøí Žák
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id; // identifikator
    float x; // souradnice x
    float y; // souradnice y
};

struct cluster_t {
    int size; // pocet objektu ve shluku
    int capacity; // kapacita shluku
    struct obj_t *obj; // pole objektu pro dany shluk
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
 */
void init_cluster(struct cluster_t *c, int cap) {

    assert(c != NULL);
    assert(cap >= 0);

    c->size = 0;
    if (cap > 0) {
        if ((c->obj = malloc(cap * sizeof (struct obj_t)))) {
            c->capacity = cap;
        }
    } else {
        c->capacity = 0;
        c->obj = NULL;
    }

}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c) {
    free(c->obj); // odstraneni vsech objektu shluku
    init_cluster(c, 0); // inicializace na prazdny shluk
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap) {
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof (struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*) arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj) {

    if (c->capacity <= c->size) {
        resize_cluster(c, c->capacity + CLUSTER_CHUNK);
    } else {
        c->obj[c->size++] = obj;
    }

}

// pomocna funkce pro razeni shluku

static int obj_sort_compar(const void *a, const void *b) {
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *) a;
    const struct obj_t *o2 = (const struct obj_t *) b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
 */
void sort_cluster(struct cluster_t *c) {
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof (struct obj_t), &obj_sort_compar);
}

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2) {
    assert(c1 != NULL);
    assert(c2 != NULL);

    for (int i = 0; i < c2->size; i++) {
        append_cluster(c1, c2->obj[i]);
    }
    //serazeni clusteru
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx) {
    assert(idx < narr);
    assert(narr > 0);

    int newNarr = narr - 1;
    clear_cluster(&carr[idx]);
    for (int i = idx; i < newNarr; i++) {
        carr[i] = carr[i + 1];
    }
    return newNarr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2) {
    assert(o1 != NULL);
    assert(o2 != NULL);

    int rozdilX = o1->x - o2->x;
    int rozdilY = o1->y - o2->y;
    rozdilX *= rozdilX;
    rozdilY *= rozdilY;
    return sqrt(rozdilX + rozdilY);
}

/*
 Pocita vzdalenost dvou shluku.
 */
/*float cluster_distance(struct cluster_t *c1, struct cluster_t *c2) {
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO
}*/

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
 */
/*void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2) {
    assert(narr > 0);

    // TODO
}*/

/*
 Tisk shluku 'c' na stdout.
 */
void print_cluster(struct cluster_t *c) {
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++) {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
 */
int load_clusters(char *filename, struct cluster_t **arr) {
    assert(arr != NULL);

    FILE *soubor = fopen(filename, "r");
    if (soubor == NULL) {
        printf("Soubor se nepodarilo otevrit\n");
        return -1;
    }

    int radek = 0, pocetObjektu = 0;
    char text[100];
    struct obj_t obj;
    struct cluster_t *cluster;
    int id, x, y;
    char end;

    while (fgets(text, 100, soubor)) {
        // prvni radek
        radek++;
        if (radek == 1) {
            sscanf(text, "count=%d%[^\n]", &pocetObjektu, &end);
            for (int i = 0; i < pocetObjektu; i++) {
                init_cluster(&(*arr)[i], 1);
            }
            continue;
        }

        // nacitani jednotlivych objektu
        if (sscanf(text, "%d %d %d", &id, &x, &y) == 3 && x < 1000 && x > 0 && y < 1000 && y > 0) {
            obj.id = id;
            obj.x = x;
            obj.y = y;
        } else {
            printf("Spatne zadany objekt\n");
            fclose(soubor);
            return -1;
        }

        if((radek-1)>pocetObjektu){
            printf("V souboru je vice objektu nez je pocet zadany na zacatku\n");
            return -1;
        }
                
        // pridani objektu do shluku
        cluster = &(*arr)[radek - 2];
        append_cluster(cluster, obj);
    }

    fclose(soubor);
    return radek - 1;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
 */
void print_clusters(struct cluster_t *carr, int narr) {
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++) {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[]) {

    struct cluster_t *clusters;

    if (argc == 3) {
        char *ptr;
        int pocetShluku = (int)strtol(argv[2], &ptr, 10);
        if (pocetShluku == 0)
            pocetShluku = 1;
        load_clusters(argv[1], &clusters);
        print_clusters(clusters, pocetShluku);
        for (int i = 0; i < pocetShluku; i++) {
            clear_cluster(&clusters[i]);
        }
    }
}
