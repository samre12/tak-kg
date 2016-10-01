def partition(n):
    '''Generates all permutations of all partitions
    of n
    '''

    part_list = []
    part_list.append([n])
    for x in xrange(1, n):
        for y in partition(n - x):
            part_list.append([x] + y)
    return part_list

print partition(5)
