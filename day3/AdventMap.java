// This is my first time writing Java in years. Definitely some rust
// to knock off.
import java.io.*;
import java.util.*;
import java.util.stream.*;

// Represents a map of the route to the airport.  Coordinates increase going to
// the right and down, with origin at 0, 0. X coordinates are wrapped modulo the
// width of the map fragement. Y coordinates are also wrapped, but for this
// problem is not a necessary requirement.
public class AdventMap {
    public enum MapContent {
        OPEN,
        TREE;

        static MapContent fromChar(char c) {
            if (c == '#') {
                return MapContent.TREE;
            } else {
                return MapContent.OPEN;
            }
        }

        public String toString() {
            if (this == MapContent.TREE) {
                return "#";
            } else {
                return ".";
            }
        }
    };

    private MapContent[][] map_cells;

    // I will be candid: it's been several years since I wrote Java. Java 7 was
    // the last version that I did any sort of interesting work on, and there
    // have been a number of additions to the language. Notably lambdas and
    // Streams. This is likely not the best way to do this. But I'm learning the
    // ropes. :)
    public AdventMap(Stream<String> input) {
        this.map_cells = input.
            map(line -> parseLineToGridRow(line)).
            toArray(MapContent[][]::new);
    }

    private MapContent[] parseLineToGridRow(String line) {
        return line.
            chars().
            mapToObj(c -> (char)c).
            map(c -> MapContent.fromChar(c)).
            toArray(MapContent[]::new);
    }

    public MapContent getMapContent(int x, int y) {
        return map_cells[y % getHeight()][x % getWidth()];
    }

    public int getHeight() {
        return map_cells.length;
    }

    public int getWidth() {
        if (map_cells.length > 0) {
            return map_cells[0].length;
        } else {
            return 0;
        }
    }

    public static AdventMap loadFromFile(String input_file_name)
        throws FileNotFoundException
    {
        BufferedReader input_reader =
            new BufferedReader(
                new FileReader(input_file_name)
            );
        return new AdventMap(input_reader.lines());
    }
}
