/**
 * @mainpage Projekt 3
 * @link
 * proj3.h
 * @endlink
 * 
 * @author Jiri Zak
 */

/**
 * @brief Struktura reprezentuje objekt se souradnicemi x a y
 */
struct obj_t {
    int id;
    float x;
    float y;
};

/**
 * @brief Struktura reprezentuje shluk objektu
 */
struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/**
 * @defgroup clusters Shluky
 * @{
 */

/**
 * Inicializace shluku 'c', kapacita dana cap
 * 
 * @post
 * Shluk 'c' bude alokovat pamet pro 'cap' objektu,
 * jestlize nenastane zadna chyba 
 * 
 * @param c kvuli inicializaci
 * @param cap kvuli kapacite shluku
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * Vymaze vsechny objekty shluku 'c' a inicializuje se na prazdny shluk
 * 
 * @post
 * Uvolni alokovanou pamet shluku 'c'
 * 
 * @param c shluk pro vymazani
 */
void clear_cluster(struct cluster_t *c);

/// doporucena hodnota pro realokaci shluku
extern const int CLUSTER_CHUNK;

/**
 * Zmena kapacity shluku 'c'
 * 
 * @pre 
 * Shluk 'c' musi jiz existovat
 * a jeho kapacita bude vetsi nebo rovna 0 
 * 
 * @post
 * Shluk 'c' bude mit novou kapcitu 'new_cap',
 * jestlize nenastane chyba
 * 
 * @param c dany shluk
 * @param new_cap nova kapacita
 * @return shluk s novou kapacitou, jinak NULL kdyz nastane chyba
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * Priradi objektu 'obj' na konec shluku 'c',
 * Pokud by se tam nevesel, tak shluk rozsiri.
 * 
 * @pre
 * Pocet objektu ve shluku bude vetsi nebo roven 0
 * 
 * @post
 * Prida objekt 'obj' na posledni pozici shluku 'c'
 * pokudnenastane zadna chyba
 * 
 * @param c shluk do ktereho se prida objekt
 * @param obj objekt, ktery se prida do shluku
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren. 
 * Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla. 
 * Shluk 'c2' bude nezmenen.
 * 
 * @pre
 * Pocet objektu ve shluku 'c2' bude vetsi nebo roven 0
 * 
 * @post
 * Shluk 'c1' bude rozsiren o objekty shluku 'c2'
 * a objekty shluku 'c1' budou serazeny podle ID
 * 
 * @param c1 shluk, do ktereho se pridaji objekty shluku 'c2'
 * @param c2 shluk, jehoz objekty se pridaji do shluku 'c1'
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Serazeni objektu v danem shluku 'c' vzestupne podle jejich ID
 * 
 * @post
 * Obejkty ve shluku 'c' budou serazeny vzestupne podle ID
 * 
 * @param c shluk pro serazeni
 */
void sort_cluster(struct cluster_t *c);

/**
 * Vypise shluk 'c' do konzole
 * 
 * @post 
 * Objekty shluku 'c' budou vypsany do konzole
 * 
 * @param c pro vypsani
 */
void print_cluster(struct cluster_t *c);

/**
 * @}
 */

/**
 * @defgroup array_clusters Pole shluku
 * @{
 */

/**
 * Odstrani shluk z pole shluku 'carr'
 * 
 * @post
 * Z pole 'carr' bude odstranen shluk na pozici 'idx'
 * 
 * @param carr pole shluku
 * @param narr pocet shluku v poli
 * @param idx index shluku, ktery se ma odstranit
 * @return novy pocet shluku v poli
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * Vypocita Euklidovskou vzdalenost mezi dvema objekty
 * 
 * @param o1 objekt 1
 * @param o2 objekt 2
 * @return Euklidovska vzdalenost mezi objekty 'o1' a 'o2'
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * Vypocita vzdalenost mezi shlukem 'c1' a 'c2'
 * 
 * @pre
 * Pocet objektu v obou shlucich bude vetsi nez 0
 * 
 * @param c1 shluk 1
 * @param c2 shluk 2
 * @return vzdalenost shluku 'c1' a 'c2'
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Najde dva nejblizsi shluky
 * 
 * @post
 * Indexy dvou nejblizsich shluku budou ulozeny v 'c1' a 'c2'
 * 
 * @param carr pole shluku
 * @param narr pocet shluku v poli
 * @param c1 index prvniho z nalezenych shluku
 * @param c2 index druheho z nalezenych shluku
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * Ze souboru 'filename' nacte objekty
 * Pro kazdy objekt vytvori shluk a ulozi jej do pole shluku
 * Alokuje se prostor pro pole vsech shluku a ukazatel na prvni polozku pole
 * (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 * kam se odkazuje parametr 'arr'
 * 
 * @pre
 * Soubor 'filename' musi existovat a program musi mit prava pro cteni
 * Data musi byt ve spravnem formatu
 * 
 * @post
 * Pro kazdy objekt ze vstupniho souboru se vytvori shluk
 * Tyto shluky se ulozy do pole shluku 'arr'
 * 
 * @param filename nazev souboru ze ktereho se budou cist objekty
 * @param arr ukazatel na pole shluku
 * @return pocet nactenych shluku nebo -1 kdyz nastane chyba
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * Vypise pole shluku
 * 
 * @post
 * Vsechny shluku v poli 'carr' budou vypsany i s jejich objekty
 * 
 * @param carr pole shluku
 * @param narr pocet shluku v poli
 */
void print_clusters(struct cluster_t *carr, int narr);

/**
 * @}
 */
