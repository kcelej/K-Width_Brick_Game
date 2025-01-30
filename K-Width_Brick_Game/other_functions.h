
/*
 * Generates a random integer within a specified range.
 * @details This function returns a random number in the range [min, max].
 *          It initializes the random seed using `srand(time(NULL))` only once
 *          to ensure different random sequences on each program run.
 * @param min The lower bound of the range (inclusive).
 * @param max The upper bound of the range (inclusive).
 * @return A random integer between `min` and `max`.
 */
int random(int min, int max) {
    // Ensures seed initialization only once.
    static bool first = true;
    if (first) {
        // Seed the random number generator with the current time.
        srand(time(NULL));
        // Prevent re-seeding on subsequent calls.
        first = false;
    }
    // Generate a random number in the specified range.
    return min + rand() % ((max + 1) - min);
}