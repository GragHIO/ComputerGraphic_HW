#pragma once
#ifndef LIST_TREE
#define LIST_TREE
template <typename Data> struct tree;
template <typename Data> struct tree_node;

template <typename Data> struct tree
{
	struct tree_node<typename Data> * root;	
};

template <typename Data> struct tree_node
{
	struct list_head <tree_node <typename Data> *> * dad;
	struct list_head <tree_node <typename Data> *> * son;
	Data data;
};

template <typename Data> inline struct tree_node<Data> * create_tree_node(Data data) {
	struct tree_node<Data> * tree_ptr = new struct tree_node<Data>;
	tree_ptr->dad = create_list<tree_node<Data> *>();
	tree_ptr->son = create_list<tree_node<Data> *>();
	tree_ptr->data = data;
	return tree_ptr;
}

template <typename Data> struct tree_node<Data> * add_tree_node(struct tree_node<Data> * node, Data data)
{
	struct tree_node<Data> * node_ptr = create_tree_node(data);
	add_data(node->son, node_ptr);
	add_data(node_ptr->dad, node);
	return node_ptr;
}

template <typename Data> struct tree_node<Data> * add_subtree(struct tree_node<Data> * node, struct tree_node<Data> * subtree)
{
	add_data(node->son, subtree);
	add_data(subtree->dad, node);
	return subtree;
}

#endif