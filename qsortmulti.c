/***************************************************************************
 *
 * Multithreaded version of Quick sort
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define KILO (1024)
#define MEGA (1024 * 1024)
#define MAX_ITEMS (64 * MEGA)
#define swap(v, a, b) \
    {                 \
        unsigned tmp; \
        tmp = v[a];   \
        v[a] = v[b];  \
        v[b] = tmp;   \
    }

static int *v;

static void
print_array(void)
{
    int i;
    for (i = 0; i < MAX_ITEMS; i++)
        printf("%d ", v[i]);
    printf("\n");
}

bool assert_sorted()
{
    int last_item = v[0];
    for (int i = 1; i < MAX_ITEMS; i++)
    {
        if (v[i] < last_item)
        {
            return false;
        }
        last_item = v[i];
    }
    return true;
}

static void
init_array(void)
{
    int i;
    v = (int *)malloc(MAX_ITEMS * sizeof(int));
    for (i = 0; i < MAX_ITEMS; i++)
        v[i] = rand();
}

static unsigned
partition(int *v, unsigned low, unsigned high, unsigned pivot_index)
{
    /* move pivot to the bottom of the vector */
    if (pivot_index != low)
        swap(v, low, pivot_index);

    pivot_index = low;
    low++;

    /* invariant:
     * v[i] for i less than low are less than or equal to pivot
     * v[i] for i greater than high are greater than pivot
     */

    /* move elements into place */
    while (low <= high)
    {
        if (v[low] <= v[pivot_index])
            low++;
        else if (v[high] > v[pivot_index])
            high--;
        else
            swap(v, low, high);
    }

    /* put pivot back between two groups */
    if (high != pivot_index)
        swap(v, pivot_index, high);
    return high;
}

static void
_quick_sort_seq(int *v, unsigned low, unsigned high)
{
    unsigned pivot_index;

    /* no need to sort a vector of zero or one element */
    if (low >= high)
        return;

    /* select the pivot value */
    pivot_index = (low + high) / 2;

    /* partition the vector */
    pivot_index = partition(v, low, high, pivot_index);

    /* sort the two sub arrays */
    if (low < pivot_index)
        _quick_sort_seq(v, low, pivot_index - 1);
    if (pivot_index < high)
        _quick_sort_seq(v, pivot_index + 1, high);
}

typedef struct
{
    int *v;
    unsigned low;
    unsigned high;
    unsigned current_threads;
    unsigned max_threads;
} ThreadData;

static void *_quick_sort(void *arg)
{
    ThreadData data = *(ThreadData *)arg;
    int *v = data.v;
    unsigned low = data.low;
    unsigned high = data.high;
    unsigned current_threads = data.current_threads;
    unsigned max_threads = data.max_threads;

    unsigned pivot_index;

    /* no need to sort a vector of zero or one element */
    if (low >= high)
        return NULL;

    /* select the pivot value */
    pivot_index = (low + high) / 2;

    /* partition the vector */
    pivot_index = partition(v, low, high, pivot_index);

    pthread_t thread1;
    ThreadData part1;
    part1.v = v;
    part1.low = low;
    part1.high = pivot_index - 1;
    part1.current_threads = current_threads * 2;
    part1.max_threads = max_threads;

    pthread_t thread2;
    ThreadData part2;
    part2.v = v;
    part2.low = pivot_index + 1;
    part2.high = high;
    part2.current_threads = current_threads * 2;
    part2.max_threads = max_threads;

    if (current_threads * 2 <= max_threads)
    {
        if (low < pivot_index)
        {
            pthread_create(&thread1, NULL, _quick_sort, &part1);
        }
        if (pivot_index < high)
        {
            pthread_create(&thread2, NULL, _quick_sort, &part2);
        }
        if (low < pivot_index)
        {
            pthread_join(thread1, NULL);
        }
        if (pivot_index < high)
        {
            pthread_join(thread2, NULL);
        }
    }
    else
    {
        /* sort the two sub arrays */
        if (low < pivot_index)
            // _quick_sort(&part1);
            _quick_sort_seq(v, low, pivot_index - 1);
        if (pivot_index < high)
            // _quick_sort(&part2);
            _quick_sort_seq(v, pivot_index + 1, high);
    }
    return NULL;
}

static void
quick_sort(int *v, unsigned low, unsigned high)
{
    ThreadData part1;
    part1.v = v;
    part1.low = low;
    part1.high = high;
    part1.current_threads = 1;
    part1.max_threads = 8192;
    _quick_sort(&part1);
}

int main(int argc, char **argv)
{
    init_array();
    // print_array();
    quick_sort(v, 0, MAX_ITEMS - 1);
    // printf("%s\n", assert_sorted() ? "Sort was successfull" : "Failed to sort");
}
