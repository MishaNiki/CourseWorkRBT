package main

import (
	"fmt"
	"./rbtree"
	"time"
	"os"
	"bufio"
)

type Testing struct {
	inputArr, deleteArr []int
	sizeInput, sizeDelete int
}


func IntComparator(a, b interface{}) int {
	aAsserted := a.(int)
	bAsserted := b.(int)
	switch {
	case aAsserted > bAsserted:
		return 1
	case aAsserted < bAsserted:
		return -1
	default:
		return 0
	}
}

func (test *Testing) ArrayFilling(idxPath, numTest int) *Testing {

	var idxPathIn int
	if idxPath > 2 {
		idxPathIn = 1
	} else {
		idxPathIn = idxPath
	}
	path := [5]string{"../test/rand/", "../test/sort/", "../test/swap/", "../test/decsort/", "../test/insort/"}
	pathInputData := fmt.Sprintf("%vinput%v", path[idxPathIn], numTest)
	pathDeleteData := fmt.Sprintf("%vdelete%v", path[idxPath], numTest)
	
	fin, err := os.Open(pathInputData)
	if err != nil {
		panic(err)
	} 
	defer fin.Close()
	fdel, err := os.Open(pathDeleteData)
	if err != nil {
		panic(err)
	}
	defer fdel.Close()

	test = &Testing{}

	reader_in := bufio.NewReader(fin)
	reader_del := bufio.NewReader(fdel)

	fmt.Fscan(reader_in, &test.sizeInput)
	fmt.Fscan(fdel, &test.sizeDelete)
	test.inputArr = make([]int, test.sizeInput)
	test.deleteArr = make([]int, test.sizeDelete)

	for i := 0; i < test.sizeInput; i++ {
		fmt.Fscan(reader_in, &test.inputArr[i])
	}

	for i := 0; i < test.sizeDelete; i++ {
		fmt.Fscan(reader_del, &test.deleteArr[i])
	}

	return test	
}

func main() {

	var (
		result_path = [5]string{"../result/Go/rand", "../result/Go/sort", "../result/Go/swap", "../result/Go/decsort", "../result/Go/insort"}
		result[10] float32
	)

	for i := 3; i < 5; i++ {
		for j := 0; j < 10; j++ {
			
			var test *Testing
			st := time.Now()
			test = test.ArrayFilling(i, j)
			en := time.Now()
			fmt.Printf("время работы загрузки %.3f\n", float32(en.Sub(st).Seconds()))

			start := time.Now()
			tree := rbtree.CreateRBTree(IntComparator)
			for k := 0; k < test.sizeInput; k++ {
				tree.InsertNode(test.inputArr[k])
			}
			for k := 0; k < test.sizeDelete; k++ {
				node, e := tree.FindNode(test.deleteArr[k])
				if e {
					tree.Remove(node)
				}
			}
			//println(tree.String())
			tree.Clear()

			end := time.Now()
			result[j] = float32(end.Sub(start).Seconds())
			fmt.Printf("время = %.3f, test %v %v\n", result[j], i, j)
		}

		fres, err := os.Create(result_path[i])
		if err != nil {
			panic(err)
		}
		for k := 0; k < 10; k++ {
			fmt.Fprintf(fres, "%v %.3f\n", (k+1)*1000000, result[k])
		} 
		fres.Close()
	}
}
