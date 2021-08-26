package main

import (
    "fmt"
)

func main() {
    fmt.Printf("%v\n", 300)
    for z := 0; z < 10; z++ {
        for x := 0; x < 10; x++ {
            fmt.Printf("%v 0 %v\n%v 0 %v\n%v 0 %v\n", x, z, x + 1, z, x, z + 1)
        }
    }

    fmt.Printf("%v\n", 100)
    for i := 0; i < 299; i += 3 {
        fmt.Printf("%v %v %v\n", i, i + 1, i + 2)
    }
}

