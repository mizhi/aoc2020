// This is my first time writing Java in years. Definitely some rust
// to knock off.
import java.io.FileNotFoundException;

class Part1 {
    public static void main(String[] args) {
        AdventMap advent_map = null;

        try {
            advent_map = AdventMap.loadFromFile("input1.txt");
        } catch (FileNotFoundException exc) {
            System.out.println("Could not find input.txt");
        }

        TreeCounter tree_counter = new TreeCounter(advent_map);
        System.out.println(tree_counter.countTreesForSlope(new Slope(3, 1)));
    }
}
