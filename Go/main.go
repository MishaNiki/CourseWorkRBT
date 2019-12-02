package main

import (
	"fmt"
	"./rbtree"
	"time"
	"os"
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

	path := [3]string{"../test/rand/", "../test/sort/", "../test/swap/"}
	pathInputData := fmt.Sprintf("%vinput%v", path[idxPath], numTest)
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

	fmt.Fscan(fin, &test.sizeInput)
	fmt.Fscan(fdel, &test.sizeDelete)
	test.inputArr = make([]int, test.sizeInput)
	test.deleteArr = make([]int, test.sizeDelete)

	for i := 0; i < test.sizeInput; i++ {
		fmt.Fscan(fin, &test.inputArr[i])
	}

	for i := 0; i < test.sizeDelete; i++ {
		fmt.Fscan(fdel, &test.deleteArr[i])
	}

	return test	
}

func main() {

	var (
		result_path = [3]string{"../result/Go/rand", "../result/Go/sort", "../result/Go/swap"}
		result[10] int
	)

	for i := 0; i < 3; i++ {
		for j := 0; j < 10; j++ {
			
			var test *Testing
			st := time.Now()
			test = test.ArrayFilling(i, j)
			en := time.Now()
			fmt.Println("время работы загрузки", int(en.Sub(st).Seconds()))

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
			result[j] = int(end.Sub(start).Seconds())
			fmt.Printf("время = %v, test %v %v\n", result[j], i, j)
		}

		fres, err := os.Create(result_path[i])
		if err != nil {
			panic(err)
		}
		for k := 0; k < 10; k++ {
			fmt.Fprintf(fres, "%v\n", result[k])
		} 
		fres.Close()
	}
}
