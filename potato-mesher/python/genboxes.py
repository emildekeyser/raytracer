#!/bin/python

# doos(driehoeken):

#     randen = bereken randen (driehoeken)
#     linkerdriehoeken = bereken linkerdriehoeken (driehoeken)
#     rechterdriehoeken = bereken rechterdriehoeken (driehoeken)

#     als # linkerdriehoeken > N:
#         linkerdoos = doos(linkerdriehoeken)
#     anders
#         linkerdoos = linkerdriehoeken

#     als # rechterdriehoeken > N:
#         rechterdoos = doos(rechterdriehoeken)
#     anders:
#         rechterdoos = rechterdriehoeken

#     return (randen, linkerdoos, rechterdoos)


# rootdoos = doos(alle driehoeken)

import armadillo
import sys
import operator

N = 10

def find_minmax_vertices(triangles):
    min_x, min_y, min_z = sys.maxsize, sys.maxsize, sys.maxsize
    max_x, max_y, max_z = -min_x, -min_y, -min_z
    for triangle in triangles:
        xs = [armadillo.vertices[vi][0] for vi in triangle]
        ys = [armadillo.vertices[vi][1] for vi in triangle]
        zs = [armadillo.vertices[vi][2] for vi in triangle]
        max_x = max([max_x] + xs)
        max_y = max([max_y] + ys)
        max_z = max([max_z] + zs)
        min_x = min([min_x] + xs)
        min_y = min([min_y] + ys)
        min_z = min([min_z] + zs)

    bv1 = (max_x, max_y, max_z)
    bv2 = (min_x, min_y, min_z)
    return bv1, bv2

def divide_box(boxvertex1, boxvertex2):
    print(f"dividing {boxvertex1} and {boxvertex2}")

    x1, y1, z1 = boxvertex1
    x2, y2, z2 = boxvertex2
    x = ('x', abs(x1 - x2))
    y = ('y', abs(y1 - y2))
    z = ('z', abs(z1 - z2))

    print(x)
    print(y)
    print(z)

    longest = max(x, y, z, key=operator.itemgetter(1))
    x = x[1] if longest[0] != 'x' else longest[1] / 2
    y = y[1] if longest[0] != 'y' else longest[1] / 2
    z = z[1] if longest[0] != 'z' else longest[1] / 2

    emptyleftbox = (boxvertex1, (
        x1 - x,
        y1 - y,
        z1 - z
    ))
    emptyrightbox = ((
        x2 + x,
        y2 + y,
        z2 + z
    ), boxvertex2)

    return emptyleftbox, emptyrightbox

def find_inside(triangles, box):
    n = len(triangles)
    print(f"finding {n} triangles in {box}")

    (x1, y1, z1), (x2, y2, z2) = box
    for triangle in triangles:
        xs = [armadillo.vertices[vi][0] for vi in triangle]
        ys = [armadillo.vertices[vi][1] for vi in triangle]
        zs = [armadillo.vertices[vi][2] for vi in triangle]
        inside = x1 >= max(xs) \
            and x2 <= min(xs) \
            and y1 >= max(ys) \
            and y2 <= min(ys) \
            and z1 >= max(zs) \
            and z2 <= min(zs)
        if(inside):
            yield triangle

def box(triangles):

    n = len(triangles)
    print(f"Starting box with: {n} triangles")

    boxvertex1, boxvertex2 = find_minmax_vertices(triangles)
    emptyleftbox, emptyrightbox = divide_box(boxvertex1, boxvertex2)
    left_triangles = list(find_inside(triangles, emptyleftbox))
    right_triangles = list(find_inside(triangles, emptyrightbox))

    n = len(left_triangles)
    print(f"left: {n} triangles")
    n = len(right_triangles)
    print(f"right: {n} triangles")

    if len(left_triangles) > N:
        leftbox = box(left_triangles)
    else:
        leftbox = left_triangles

    if len(right_triangles) > N:
        rightbox = box(right_triangles)
    else:
        rightbox = right_triangles

    return (boxvertex1, boxvertex2, leftbox, rightbox)

# TODO
# def with_triangle_data(box):
#     v1, v2, leftbox, rightbox = box
#     if(all([isinstance(x, int) for x in leftbox])):

