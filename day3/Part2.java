// This is my first time writing Java in years. Definitely some rust
// to knock off.
import java.io.FileNotFoundException;
import java.util.Arrays;

class Part2 {
    public static void main(String[] args) {
        AdventMap advent_map = null;

        try {
            advent_map = AdventMap.loadFromFile("input1.txt");
        } catch (FileNotFoundException exc) {
            System.out.println("Could not find input.txt");
        }

        TreeCounter tree_counter = new TreeCounter(advent_map);

        Slope[] all_slopes = new Slope[] {
            new Slope(1, 1),
            new Slope(3, 1),
            new Slope(5, 1),
            new Slope(7, 1),
            new Slope(1, 2)
        };

        int product = Arrays.stream(all_slopes).
            map(slope -> tree_counter.countTreesForSlope(slope)).
            reduce((x, y) -> x * y).
            orElse(-1);

        System.out.println(product);
    }
}
