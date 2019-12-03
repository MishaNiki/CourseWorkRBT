
package rbtree

import (
	"fmt"
)

type Comparator func(a, b interface{}) int //тип интерфейса сравнения данных в дереве

type color bool;//тип цвета узла


const (
	black, red color = true, false
)

type Rbtree struct {
	Root *Rbnode;
	Comparator  Comparator
}

type Rbnode struct {

	Data	interface{}
	color	color
	Left	*Rbnode
	Right	*Rbnode
	Parent	*Rbnode
}


//создание дерева, присваивание интерфеса сравнения ключей
func CreateRBTree(Comp Comparator) *Rbtree {
	return &Rbtree{nil, Comp}
}

func (tree *Rbtree) Clear() {
	tree.Root = nil;
}

//добавление нового элемента
func (tree *Rbtree) InsertNode(data interface{}){
	var (
		current, parent, insertedNode *Rbnode = tree.Root, nil, nil
	)
	for current != nil {
		compare := tree.Comparator(data, current.Data)
		if compare == 0 { return }
		parent = current
		if compare < 0 {
			current = current.Left
		} else {
			current = current.Right
		}
	}
	insertedNode = &Rbnode{data, red, nil, nil, parent}
	if parent != nil {
		if tree.Comparator(data, parent.Data) < 0 {
			parent.Left = insertedNode
		} else {
			parent.Right = insertedNode
		}
	} else {
		tree.Root = insertedNode
	}
	tree.insertCase1(insertedNode)
}

func (tree *Rbtree) Remove(node *Rbnode) {
	var child *Rbnode
	if node == nil {
		return
	}
	if node.Left != nil && node.Right != nil {
		pred := node.Left.maximumNode()
		node.Data = pred.Data
		node = pred
	}
	if node.Left == nil || node.Right == nil {
		if node.Right == nil {
			child = node.Left
		} else {
			child = node.Right
		}
		if node.color == black {
			node.color = nodeColor(child)
			tree.deleteCase1(node)
		}
		tree.replaceNode(node, child)
		if node.Parent == nil && child != nil {
			child.color = black
		}
	}
}

//поиск
func (tree *Rbtree) FindNode(data interface{}) (*Rbnode, bool){
	current := tree.Root
	for current != nil {
		compare := tree.Comparator(data, current.Data)
		switch {
		case compare == 0:
			return current, true
		case compare < 0:
			current = current.Left
		case compare > 0:
			current = current.Right
		}
	}
	return nil, false
}

func (tree *Rbtree) PrintRBTree() {
	PrintRBNode(tree.Root);
}


func PrintRBNode(x *Rbnode) {
	if(x == nil){return;}
	PrintRBNode(x.Left);
	fmt.Printf("%v ", x.Data.(int));
	PrintRBNode(x.Right);
}


func (tree *Rbtree) String() string {
	str := "RedBlackTree\n"
	if tree.Root != nil {
		output(tree.Root, "", true, &str)
	}
	return str
}


func (node *Rbnode) String() string {
	if node.color == red {
		return fmt.Sprintf("%v-R", node.Data)
	} else {
		return fmt.Sprintf("%v-B", node.Data)
	}
}

func output(node *Rbnode, prefix string, isTail bool, str *string) {
	if node.Right != nil {
		newPrefix := prefix
		if isTail {
			newPrefix += "│   "
		} else {
			newPrefix += "    "
		}
		output(node.Right, newPrefix, false, str)
	}
	*str += prefix
	if isTail {
		*str += "└── "
	} else {
		*str += "┌── "
	}
	*str += node.String() + "\n"
	if node.Left != nil {
		newPrefix := prefix
		if isTail {
			newPrefix += "    "
		} else {
			newPrefix += "│   "
		}
		output(node.Left, newPrefix, true, str)
	}
}

func (node *Rbnode) maximumNode() *Rbnode {
	if node == nil {
		return nil
	}
	for node.Right != nil {
		node = node.Right
	}
	return node
}

func (node *Rbnode) grandparent() *Rbnode {
	if node != nil && node.Parent != nil {
		return node.Parent.Parent
	}
	return nil
}

func (node *Rbnode) uncle() *Rbnode {
	if node == nil || node.Parent == nil || node.Parent.Parent == nil {
		return nil
	}
	return node.Parent.sibling()
}

func (node *Rbnode) sibling() *Rbnode {
	if node == nil || node.Parent == nil {
		return nil
	}
	if node == node.Parent.Left {
		return node.Parent.Right
	}
	return node.Parent.Left
}

func (tree *Rbtree) rotateLeft(node *Rbnode) {
	right := node.Right
	tree.replaceNode(node, right)
	node.Right = right.Left
	if right.Left != nil {
		right.Left.Parent = node
	}
	right.Left = node
	node.Parent = right
}

func (tree *Rbtree) rotateRight(node *Rbnode) {
	left := node.Left
	tree.replaceNode(node, left)
	node.Left = left.Right
	if left.Right != nil {
		left.Right.Parent = node
	}
	left.Right = node
	node.Parent = left
}

func (tree *Rbtree) replaceNode(old *Rbnode, new *Rbnode) {
	if old.Parent == nil {
		tree.Root = new
	} else {
		if old == old.Parent.Left {
			old.Parent.Left = new
		} else {
			old.Parent.Right = new
		}
	}
	if new != nil {
		new.Parent = old.Parent
	}
}

func (tree *Rbtree) insertCase1(node *Rbnode) {
	if node.Parent == nil {
		node.color = black
	} else {
		tree.insertCase2(node)
	}
}

func (tree *Rbtree) insertCase2(node *Rbnode) {
	if nodeColor(node.Parent) == black {
		return
	}
	tree.insertCase3(node)
}

func (tree *Rbtree) insertCase3(node *Rbnode) {
	uncle := node.uncle()
	if nodeColor(uncle) == red {
		node.Parent.color = black
		uncle.color = black
		node.grandparent().color = red
		tree.insertCase1(node.grandparent())
	} else {
		tree.insertCase4(node)
	}
}

func (tree *Rbtree) insertCase4(node *Rbnode) {
	grandparent := node.grandparent()
	if node == node.Parent.Right && node.Parent == grandparent.Left {
		tree.rotateLeft(node.Parent)
		node = node.Left
	} else if node == node.Parent.Left && node.Parent == grandparent.Right {
		tree.rotateRight(node.Parent)
		node = node.Right
	}
	tree.insertCase5(node)
}

func (tree *Rbtree) insertCase5(node *Rbnode) {
	node.Parent.color = black
	grandparent := node.grandparent()
	grandparent.color = red
	if node == node.Parent.Left && node.Parent == grandparent.Left {
		tree.rotateRight(grandparent)
	} else if node == node.Parent.Right && node.Parent == grandparent.Right {
		tree.rotateLeft(grandparent)
	}	
}


func (tree *Rbtree) deleteCase1(node *Rbnode) {
	if node.Parent == nil {
		return
	}
	tree.deleteCase2(node)
}

func (tree *Rbtree) deleteCase2(node *Rbnode) {
	sibling := node.sibling()
	if nodeColor(sibling) == red {
		node.Parent.color = red
		sibling.color = black
		if node == node.Parent.Left {
			tree.rotateLeft(node.Parent)
		} else {
			tree.rotateRight(node.Parent)
		}
	}
	tree.deleteCase3(node)
}

func (tree *Rbtree) deleteCase3(node *Rbnode) {
	sibling := node.sibling()
	if nodeColor(node.Parent) == black &&
		nodeColor(sibling) == black &&
		nodeColor(sibling.Left) == black &&
		nodeColor(sibling.Right) == black {
		sibling.color = red
		tree.deleteCase1(node.Parent)
	} else {
		tree.deleteCase4(node)
	}
}

func (tree *Rbtree) deleteCase4(node *Rbnode) {
	sibling := node.sibling()
	if nodeColor(node.Parent) == red &&
		nodeColor(sibling) == black &&
		nodeColor(sibling.Left) == black &&
		nodeColor(sibling.Right) == black {
		sibling.color = red
		node.Parent.color = black
	} else {
		tree.deleteCase5(node)
	}
}

func (tree *Rbtree) deleteCase5(node *Rbnode) {
	sibling := node.sibling()
	if node == node.Parent.Left &&
		nodeColor(sibling) == black &&
		nodeColor(sibling.Left) == red &&
		nodeColor(sibling.Right) == black {
		sibling.color = red
		sibling.Left.color = black
		tree.rotateRight(sibling)
	} else if node == node.Parent.Right &&
		nodeColor(sibling) == black &&
		nodeColor(sibling.Right) == red &&
		nodeColor(sibling.Left) == black {
		sibling.color = red
		sibling.Right.color = black
		tree.rotateLeft(sibling)
	}
	tree.deleteCase6(node)
}

func (tree *Rbtree) deleteCase6(node *Rbnode) {
	sibling := node.sibling()
	sibling.color = nodeColor(node.Parent)
	node.Parent.color = black
	if node == node.Parent.Left && nodeColor(sibling.Right) == red {
		sibling.Right.color = black
		tree.rotateLeft(node.Parent)
	} else if nodeColor(sibling.Left) == red {
		sibling.Left.color = black
		tree.rotateRight(node.Parent)
	}
}

func nodeColor(node *Rbnode) color {
	if node == nil {
		return black
	}
	return node.color
}
