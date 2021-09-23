**Disclaimer: This project is WIP. Visualizations for the shortest path are still under work**

# Travelling Guitarist
Algorithm to optimize movement for chord progressions on the guitar. This is based on [a paper I wrote in May of 2018][1].

This serves as a practical proof-of-concept of my proposed method in the paper. Currently, the chords respository does not contain all positions of chords, as expressed in the paper. Work needs to be done to source a more thorough chords repository in order to expand functionality.

# Status
Currently, tasks finished are:
- Parse chords repository
- Create chords datatype
- Determine distance between chords using chords datatype
- Use distances to create adjacency graph, and perform Dijkstra's algorithm

What still needs to be done:
- Visualize the output of Dijkstra's algorithm into guitar tablature.

# Resources
Chords sourced from https://github.com/tombatossals/chords-db/blob/master/lib/guitar.json

[1]: Math_IA_Final.pdf