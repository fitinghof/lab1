# Multithreading

## Gaussian Elimination

### Running program

To compile, run `gcc -O2 -o out guassianmulti.c -lpthread`

### Approach

When solving this problem the first part was recognising what parts of the algorithm could be ran while not touching the same data. I found that the only part that could be trivially multithreaded was the elimination step, where every row below the current one had calculations done. After this it was a simple matter of dividing up the rows among the threads and running that job on multiple threads instead of on one.

### Reflections

This solution was without a doubt not an optimal one, it spawns thousands of threads while executing and could almost certainly be made more efficiently. It also doesn't multithread the entire algorithm but only the elimination step, even if this is the heaviest step. One option to make this more effiecient might be to create a thread pool and reuse the threads through that pool, such that there would be no need for thousands of threads to be created and joined. This would however be much more complex to implement.

### Results

The multithreaded version completed in 0.731s and the sequential version in 1.352 giving a ~85% speedup while using 8 threads at a time during the elimination step.

## Quicksort

### Running program

To compile, run `gcc -O2 -o out qsortmulti.c -lpthread`

### Approach

For quicksort I decided to have the threads split themself as the function calles itself recursivly. This is done until doing so anymore will result in more threads spawned than wanted, in this case that number is 16. It is worth noting though that since quicksort doesn't garantee a even split among the partition parts, increasing the number of threads beyond how many cores are available can still help, since it will allow the processor to continue using all threads even if a single thread exits much earlier than another due to the distribution of data.

### Reflections

The multithreading of the quicksort algorithm was much more effiecient than that of the guassian elimination, likely in large due to the reduced overhead from creating many threads and instantly throwing them away. It is also worth noting that the `max_threads` variable in this is the max threads that are working actively, that is, max threads at the lowest level in the quicksort tree. Since the parents will only wait for the children to finish they don't actually do any work and would not take up resources the way an active thread would. Because of this I chose to only count the threads that will end up doing active work, that is the threads spawned lowest in the quicksort tree.

### Results

The multithreaded version completed in 1.891s and the sequential version completed in 4.663s giving a ~247% speedup.