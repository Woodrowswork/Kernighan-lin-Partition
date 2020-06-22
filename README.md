# Kernighan-lin-Partition

This program accepts as input from the command line a list of "nodes" along with the connections between them. It will split them into two equal groups and use a greedy algorithm to partition using as few cuts as possible. http://users.ece.northwestern.edu/~haizhou/357/lec2.pdf 

This code works, but is not best practices. I underscoped the project and sat down to start coding without clearly understanding the algorithm. If I were to do it again it would be much more approachable. I kept track of the connections using a matrix so space is N^2 as long as the nodes are densely connected this is fine, but would be waseful for a sparsely connected partition.  
