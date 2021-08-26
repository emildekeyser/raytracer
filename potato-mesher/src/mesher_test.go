package main

import (
    "testing"
    "fmt"

    "github.com/ungerik/go3d/float64/vec3"
)

func TestBoxhalving(t *testing.T) {
    b, _ := vec3.ParseBox("-4 -3 -1 0 3.5 1")
    // b, _ := vec3.ParseBox("0 3.5 1 -4 -3 -1")
    v1 := vec3.T{-3, 1, 1}
    v2 := vec3.T{-2, 2.5, 1}
    v3 := vec3.T{-1, 1, 1}

    fmt.Println(b.ContainsPoint(&v1))
    fmt.Println(b.ContainsPoint(&v2))
    fmt.Println(b.ContainsPoint(&v3))
}
