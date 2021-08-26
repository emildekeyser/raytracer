package main

import (
    "math"
    "log"
    "bufio"
    "os"
    "strconv"
	"fmt"

    "github.com/ungerik/go3d/float64/vec3"

	// "container/list"
)

const N = 10

type triangle struct {
    v1 vec3.T
    v2 vec3.T
    v3 vec3.T
}

type box struct {
    vecbox vec3.Box
    left *box
    right *box
    ltriangles []triangle
    rtriangles []triangle
}

func parse_mesh() []triangle {
    scanner := bufio.NewScanner(os.Stdin)
    scanner.Split(bufio.ScanWords)

    scanner.Scan(); word := scanner.Text()
	n_vertices, _ := strconv.Atoi(word)
    all_vertices := make([]vec3.T, n_vertices)

    for i := 0; i < n_vertices; i++ {
        scanner.Scan(); word = scanner.Text()
        all_vertices[i][0], _ = strconv.ParseFloat(word, 64)
        scanner.Scan(); word = scanner.Text()
        all_vertices[i][1], _ = strconv.ParseFloat(word, 64)
        scanner.Scan(); word = scanner.Text()
        all_vertices[i][2], _ = strconv.ParseFloat(word, 64)
	}

    scanner.Scan(); word = scanner.Text()
	n_triangles, _ := strconv.Atoi(word)
    all_triangles  := make([]triangle, n_triangles)

    for i := 0; i < n_triangles; i++ {
        scanner.Scan(); word = scanner.Text()
        vi1, _ := strconv.Atoi(word)
        scanner.Scan(); word = scanner.Text()
        vi2, _ := strconv.Atoi(word)
        scanner.Scan(); word = scanner.Text()
        vi3, _ := strconv.Atoi(word)

        all_triangles[i].v1 = all_vertices[vi1]
        all_triangles[i].v2 = all_vertices[vi2]
        all_triangles[i].v3 = all_vertices[vi3]

        log.Println(all_triangles[i])
	}

    return all_triangles
}

func find_outer_box(triangles []triangle) (vec3.Box) {
    var retbox vec3.Box
    retbox.Max = triangles[0].v1
    retbox.Min = triangles[0].v1
    for _, t := range triangles {
        retbox.Max[0] = math.Max(
            math.Max(retbox.Max[0], t.v1[0]),
            math.Max(t.v2[0], t.v3[0]),
        )
        retbox.Max[1] = math.Max(
            math.Max(retbox.Max[1], t.v1[1]),
            math.Max(t.v2[1], t.v3[1]),
        )
        retbox.Max[2] = math.Max(
            math.Max(retbox.Max[2], t.v1[2]),
            math.Max(t.v2[2], t.v3[2]),
        )
        retbox.Min[0] = math.Min(
            math.Min(retbox.Min[0], t.v1[0]),
            math.Min(t.v2[0], t.v3[0]),
        )
        retbox.Min[1] = math.Min(
            math.Min(retbox.Min[1], t.v1[1]),
            math.Min(t.v2[1], t.v3[1]),
        )
        retbox.Min[2] = math.Min(
            math.Min(retbox.Min[2], t.v1[2]),
            math.Min(t.v2[2], t.v3[2]),
        )
    }
    log.Println("B: ", retbox)
    return retbox
}

func half_box(center, edge vec3.T) vec3.Box {
    var b vec3.Box
    edge.Scale(0.5)
    b.Min = vec3.Sub(&center, &edge)
    b.Max = vec3.Add(&center, &edge)
    return b
}

func divide_in_two(b vec3.Box) (*vec3.Box, *vec3.Box) {
    x := math.Abs(b.Max[0] - b.Min[0])
    y := math.Abs(b.Max[1] - b.Min[1])
    z := math.Abs(b.Max[2] - b.Min[2])
    longest := math.Max(math.Max(x, y),z)

    lcenter := b.Center()
    rcenter := b.Center()

    if x == longest {
        x /= 2
        lcenter[0] += x / 2
        rcenter[0] -= x / 2
    } else if y == longest {
        y /= 2
        lcenter[1] += y / 2
        rcenter[1] -= y / 2
    } else if z == longest {
        z /= 2
        lcenter[2] += z / 2
        rcenter[2] -= z / 2
    }

    left := half_box(lcenter, vec3.T{x, y, z})
    right := half_box(rcenter, vec3.T{x, y, z})
    log.Println("LEFT: ", left)
    log.Println("RIGHT: ", right)
    return &left, &right
}

func divide_triangles(triangles []triangle, rbox *vec3.Box) ([]triangle, []triangle) {
    var trianglesl, trianglesr []triangle
    n_vertices_right := 0

    log.Println("RBOX: ", rbox)
    for _, t := range triangles {
        if rbox.ContainsPoint(&t.v1) {
            n_vertices_right++
        }
        if rbox.ContainsPoint(&t.v2) {
            n_vertices_right++
        }
        if rbox.ContainsPoint(&t.v3) {
            n_vertices_right++
        }
        log.Println("NVR: ", n_vertices_right)

        if n_vertices_right >= 2 {
            trianglesr = append(trianglesr, t)
        } else {
            trianglesl = append(trianglesl, t)
        }

        n_vertices_right = 0
    }
    return trianglesl, trianglesr
}

func make_box(triangles []triangle) *box {
    log.Println(triangles)
    var emptybox box
    emptybox.vecbox = find_outer_box(triangles)
    _, rightvecbox := divide_in_two(emptybox.vecbox)
    triangles_left, triangles_right := divide_triangles(
        triangles,
        rightvecbox,
    )

    log.Println("L: ", len(triangles_left))
    if len(triangles_left) > N {
        emptybox.left = make_box(triangles_left)
    } else {
        emptybox.ltriangles = triangles_left
    }

    log.Println("R: ", len(triangles_right))
    if len(triangles_right) > N {
        emptybox.right = make_box(triangles_right)
    } else {
        emptybox.rtriangles = triangles_right
    }

    return &emptybox // which is filled know :)
}

func print_triangles(triangles []triangle) {
    for _, t := range triangles {
        fmt.Printf(
            "triangle %v %v %v %v %v %v %v %v %v\n",
            t.v1[0], t.v1[1], t.v1[2], 
            t.v2[0], t.v2[1], t.v2[2], 
            t.v3[0], t.v3[1], t.v3[2], 
        )
    }
}

func print_optimized_mesh(rootbox *box) {
    log.Println(rootbox)
    if rootbox != nil {
        print_triangles(rootbox.ltriangles)
        print_optimized_mesh(rootbox.left)
        print_triangles(rootbox.rtriangles)
        print_optimized_mesh(rootbox.right)
    }
    fmt.Println("box")
}

func main() {
    triangles := parse_mesh()
    optimized := make_box(triangles)
    log.Println("OPT: ", optimized)
    print_optimized_mesh(optimized)
}

