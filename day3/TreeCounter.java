class TreeCounter {
    private AdventMap advent_map;

    public TreeCounter(AdventMap advent_map) {
        this.advent_map = advent_map;
    }

    public int countTreesForSlope(Slope slope) {
        int number_of_trees = 0;

        for (int y = 0, x = 0; y < advent_map.getHeight(); y += slope.y, x += slope.x) {
            if (isTreeAt(x, y)) {
                number_of_trees++;
            }
        }

        return number_of_trees;
    }

    private boolean isTreeAt(int x, int y) {
        return advent_map.getMapContent(x, y) == AdventMap.MapContent.TREE;
    }
}
