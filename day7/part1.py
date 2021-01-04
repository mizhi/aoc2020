# At its heart, this problem is about graph traversal. Each bag
# type represents a node in a directed-acyclic graph. The solution
# is the number of nodes for which a path can be constructed that
# gets from that node to the node with the shiny gold bag. For now,
# we can ignore the specific quantities of bags.
#
# Each line in the input file describes a part of the graph in the form of
# a tree of height 1.
#
# The general format is (and being very loose with notation)
#
# <bag_type> contain (<digit> <bag_type>(, <digit> <bag_type>)*|no other bags).
#
# The first bag_type is the root of the tree, the remaining bag descriptions are
# the leaves. The full graph is obtained by taking the full union of these
# trees.
#
# General algorithm
# 1) For each line, produce the small tree representing the rules.
# 2) Union all the trees together to produce the graph.
# 3) For each node (excluding the shiny gold bag):
# 3a) If there is a path from the node to the shiny gold bag,
#     then increment a counter by 1

import re

# This produces a tuple of the form:
#
# (root bag type, [(quantity, bag_type)])
def parse_bag_rule(line):
    # split around the word "contain"
    (root_bag_type, contained_bags) = line.split("contain")

    # root bag types always end with ' bags' so let's get rid of that
    root_bag_type = root_bag_type.replace("bags", "").strip()

    contained_bag_types = parse_contained_bags(contained_bags)

    return (root_bag_type, contained_bag_types)

# this produces a list of the form:
#
# [(quantity, bag_type), ...]
def parse_contained_bags(line):
    # contained bags are specified by either the words "no other bags"
    # or by a list of specs separated by a comma.
    if line == " no other bags.":
        return []

    bag_specs = line.split(",")
    return [parse_bag_spec(bag_spec) for bag_spec in bag_specs]

# bag specs have the following form:
#
# <digit> <adjective> <color> bags?
#
# This will return a tuple of the form:
#
# (quantity, bag_type)
BAG_SPEC_REX = re.compile(r"(?P<quantity>\d) (?P<bag_type>.+)")
def parse_bag_spec(bag_spec):
    # we could have the regular expression handle this, but then it would get uglier. So
    # we just strip out inconvenient bits
    bag_spec = bag_spec.replace("bags", "bag").replace("bag", "").replace(".", "").strip()
    
    m = re.match(BAG_SPEC_REX, bag_spec)
    return (int(m.group("quantity")), m.group("bag_type"))

# we'll represent a graph as a dictionary of the form:
# <bag_type> -> [bag_type [, ...]]
#
# where the key is the originating node and the list is the set of nodes that can be traversed to from the originating node.
def build_rule_graph(sub_trees):
    rule_graph = {}
    for (root, leaves) in sub_trees:
        rule_graph[root] = leaves
    return rule_graph

# This is a straight forward depth first search. We're going to do
# a recursive algorithm for now. This has the danger of violating max stack depth.
def can_reach(rule_graph, starting_bag_type, destination_bag_type = "shiny gold", already_visited = None):
    # if we've reached the node, then return true!
    if starting_bag_type == destination_bag_type:
        return True

    # if there are no bag types that this bag type can contain,
    # then this is false.
    if len(rule_graph[starting_bag_type]) == 0:
        return False

    # iterate through all the reachable bag types and immediately return True if
    # any of the subcalls returns True
    if already_visited is None:
        already_visited = set()
        
    for reachable_bag_type in rule_graph[starting_bag_type]:
        if can_reach(rule_graph, reachable_bag_type[1], destination_bag_type, already_visited | set(starting_bag_type)):
            return True

    return False

with open("input.txt", "r") as input_file:
    sub_trees = [parse_bag_rule(line.strip()) for line in input_file.readlines()]

rule_graph = build_rule_graph(sub_trees)

count = 0
for bag_type in rule_graph.keys():
    if bag_type == "shiny gold":
        continue
    if can_reach(rule_graph, bag_type):
        count += 1

print(count)
    
