/**
 * Implementace My MALloc
 * Demonstracni priklad pro 1. ukol IPS/2019
 * Ales Smrcka
 */
 // xzakji02
 // xmicul08

#include "mmal.h"
#include <sys/mman.h> // mmap
#include <stdbool.h> // bool

#include <stdio.h>
#include <stddef.h>

#include <string.h>

#ifdef NDEBUG
/**
 * The structure header encapsulates data of a single memory block.
 */
typedef struct header Header;
struct header {

    /**
     * Pointer to the next header. Cyclic list. If there is no other block,
     * points to itself.
     */
    Header *next;

    /// size of the block
    size_t size;

    /**
     * Size of block in bytes allocated for program. asize=0 means the block
     * is not used by a program.
     */
    size_t asize;
};

/**
 * The arena structure.
 */
typedef struct arena Arena;
struct arena {

    /**
     * Pointer to the next arena. Single-linked list.
     */
    Arena *next;

    /// Arena size.
    size_t size;
};

#define PAGE_SIZE 128*1024

#endif


#define MAP_ANONYMOUS    0x20
// ^ Don't use file for mmap

#define LONG_BYTES sizeof(long)

#define ALIGN_TO(x, len) (((((x) - 1) / (len)) * (len)) + (len))
// ^ round up (x) to nearest multiple of (len)
// inspired by https://wiki-prog.infoprepa.epita.fr/images/0/04/Malloc_tutorial.pdf , section 5.1


/**
 * First address past some structure:
 *
 *  &arena[1] --> arena + sizeof(struct arena)
 *
 *  &header[1] --> header + sizeof(struct header)
 */

Arena *first_arena = NULL;

static void hdr_ctor(Header *hdr, size_t size);

/**
 * Return size alligned to PAGE_SIZE
 */
static
size_t allign_page(size_t size)
{
	return ALIGN_TO(size, PAGE_SIZE);
}

/**
 * Allocate a new arena using mmap.
 * @param req_size requested size in bytes. Should be alligned to PAGE_SIZE.
 * @return pointer to a new arena, if successfull. NULL if error.
 */

/**
 *   +-----+------------------------------------+
 *   |Arena|....................................|
 *   +-----+------------------------------------+
 *
 *   |--------------- Arena.size ---------------|
 */
static
Arena *arena_alloc(size_t req_size)
{
	Arena *new_arena;
	size_t arena_allocated_size = allign_page(req_size + sizeof(Header) + sizeof(Arena));

	if ((new_arena = mmap(NULL, arena_allocated_size, PROT_READ | PROT_WRITE,
						  MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED)
	{
		return NULL;
	}

	new_arena->next = NULL;
	new_arena->size = arena_allocated_size - sizeof(Arena);

	if (first_arena == NULL)
	{
		first_arena = new_arena;
	} else
	{
		Arena *tmp = first_arena;
		while (tmp->next != NULL)
			tmp = tmp->next;

		tmp->next = new_arena;
	}

	Header *first_header = (Header *) &(new_arena[1]);
	hdr_ctor(first_header, arena_allocated_size - sizeof(Arena) - sizeof(Header));

	first_header->next = first_header; // see line 22-23

	return new_arena;
}

/**
 * Header structure constructor (alone, not used block).
 * @param hdr       pointer to block metadata.
 * @param size      size of free block
 */
/**
 *   +-----+------+------------------------+----+
 *   | ... |Header|........................| ...|
 *   +-----+------+------------------------+----+
 *
 *                |-- Header.size ---------|
 */
static
void hdr_ctor(Header *hdr, size_t size)
{
		hdr->asize = 0;
		hdr->size = size;
		hdr->next = NULL;
}

/**
 * Splits one block into two.
 * @param hdr       pointer to header of the big block
 * @param req_size  requested size of data in the (left) block.
 * @pre   (req_size % PAGE_SIZE) = 0
 * @pre   (hdr->size >= req_size + 2*sizeof(Header))
 * @return pointer to the new (right) block header.
 */
/**
 * Before:        |---- hdr->size ---------|
 *
 *    -----+------+------------------------+----
 *         |Header|........................|
 *    -----+------+------------------------+----
 *             \----hdr->next--------------^
 */
/**
 * After:         |- req_size -|
 *
 *    -----+------+------------+------+----+----
 *     ... |Header|............|Header|....|
 *    -----+------+------------+------+----+----
 *             \---next--------^  \--next--^
 */
static
Header *hdr_split(Header *hdr, size_t req_size)
{
	// TODO add req_size assertion
	// (req_size % PAGE_SIZE) = 0
	// ^ the provided condition doesn't make sense, though

	if (hdr->size <= req_size + sizeof(Header))
	{
		return NULL;
	}

	// original free space block is split into two
	// original block's header now becomes the header of data-block
	// rest of free space is filled with second block (a free space block)

	Header *new_header = (hdr + sizeof(Header) + req_size); // second block header's address

	hdr_ctor(new_header, hdr->size - req_size - sizeof(Header));
	// create second block's header

	new_header->next = hdr->next; // link up to the header list

	hdr->size = req_size;
	hdr->asize = req_size;
	hdr->next = new_header;

	return new_header;
}

/**
 * Detect if two blocks adjacent blocks could be merged.
 * @param left      left block
 * @param right     right block
 * @return true if two block are free and adjacent in the same arena.
 */
static
bool hdr_can_merge(Header *left, Header *right)
{
	return (left && right)    // neither are NULL
		   && (left->asize == 0 && right->asize == 0) // both are not allocated
		   && (left->next == right) // they are successors
		   && (left + left->size + sizeof(Header) == right); // they are next to each other
	// TODO check if last condition is needed
}

/**
 * Merge two adjacent free blocks.
 * @param left      left block
 * @param right     right block
 */
static
void hdr_merge(Header *left, Header *right)
{
	if (hdr_can_merge(left, right))
	{
		left->next = right->next;
		left->size += right->size + sizeof(Header);
	} else
	{
		fprintf(stderr, "cannot merge blocks\n");
	}
}

/**
 * Allocate memory. Use best-fit search of available block.
 * @param size      requested size for program
 * @return pointer to allocated data or NULL if error.
 */
void *mmalloc(size_t size)
{
	if (size == 0 || size > PAGE_SIZE)
	{
		return NULL;
	}

	Header *temp_header = NULL;
	Header *best_header = NULL;
	Arena *temp_arena = NULL;

	temp_arena = first_arena;

	bool found = false;

	while (temp_arena != NULL)
	{
		temp_header = (Header *) &temp_arena[1];
		best_header = temp_header;

		do
		{
			if (temp_header->asize == 0                                        // free block
				&& temp_header->size >= (size + sizeof(Header))                    // big enough
				&& (temp_header->size < best_header->size ||
					found == false))    // first suitable OR smallest of all suitable ones
			{
				best_header = temp_header;
				found = true;
			}

			temp_header = temp_header->next;

		} while (temp_header != (Header *) (&temp_arena[1]));
		// 					 ^ header list is cyclic

		// do-while condition - 2nd candidate: (temp_header < temp_header->next)


		temp_arena = temp_arena->next;
	}

	if (!found)
	{
		// free block not found -> need to allocate more space

		if ((temp_arena = arena_alloc(size)) == NULL)
		{
			fprintf(stderr, "failed to allocate arena\n");
			return NULL;
		}

		temp_header = (Header *) &temp_arena[1];
	} else
	{
		temp_header = best_header;
	}

	if (hdr_split(temp_header, size) == NULL)
	{
		// could not split
		// TODO should something happen?
	}

	temp_header->asize = size; // TODO useless, remove?

	return &temp_header[1]; // we need to return space for content --> address just past the header
}

/**
 * Free memory block.
 * @param ptr       pointer to previously allocated data
 */
void mfree(void *ptr)
{
	if(ptr == NULL)
	{
		return;
	}
	Header *tmp = (Header *) (ptr - sizeof(Header));
	Header *prev = (Header *) (&first_arena[1]);

    if(tmp == prev){
        if(hdr_can_merge(tmp, tmp->next))
        {
            hdr_merge(tmp, tmp->next);
        }
    }
	if(prev != prev->next)
	{
        while (prev && prev->next != tmp && prev->next > prev)
        {
            prev = prev->next;
        }
        if(hdr_can_merge(tmp, tmp->next))
        {
            hdr_merge(tmp, tmp->next);
        }
        if(hdr_can_merge(prev, tmp))
        {
            hdr_merge(prev, tmp);
        }
    }
}

/**
 * Reallocate previously allocated block.
 * @param ptr       pointer to previously allocated data
 * @param size      a new requested size. Size can be greater, equal, or less
 * then size of previously allocated block.
 * @return pointer to reallocated space.
 */
void *mrealloc(void *ptr, size_t size)
{
	if(size != 0)
	{
	    void *tmp = mmalloc(size);
	    if(tmp != NULL)
	    {
	        memcpy(tmp,  ptr, size);
	        mfree(ptr);
            return tmp;
	    }
	}
    return NULL;
}
