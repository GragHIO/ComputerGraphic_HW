#pragma once
#ifndef LINKED_LIST_H
#define LINKED_LIST_H
template <typename Data> struct list_head;
template <typename Data> struct list_node;

template <typename Data> struct list_head {
	struct list_node <typename Data> * first;
	struct list_node <typename Data> * tail;
	size_t count;
};

template <typename Data> struct list_node {
	struct list_head <typename Data> * head;
	struct list_node <typename Data> * next;
	struct list_node <typename Data> * last;
	Data data;
};

template <typename Data> inline struct list_node<Data> * create_node(Data new_data) {
	struct list_node<Data> * node_ptr = new struct list_node<Data>;
	node_ptr->head = nullptr;
	node_ptr->last = nullptr;
	node_ptr->next = nullptr;
	node_ptr->data = new_data;
	return node_ptr;
}

template <typename Data> inline struct list_head<Data> * create_list() {
	struct list_head<Data> * list_ptr = new struct list_head<Data>;
	list_ptr->count = 0;
	list_ptr->first = nullptr;
	list_ptr->tail = nullptr;
	return list_ptr;
}

template <typename Data> struct list_node<Data> * add_data(struct list_head<Data> * head, Data new_data) {
	struct list_node<Data> * new_node = create_node(new_data);
	if (head->count) {
		new_node->head = head;
		if (head->count == 1) {
			new_node->last = head->first;
			head->first->next = new_node;
		} else {
			struct list_node<Data> * last_node = head->tail;
			new_node->last = last_node;
			last_node->next = new_node;
		}
	} else {
		new_node->head = head;
		head->first = new_node;
	}
	head->tail = new_node;
	head->count++;
	return new_node;
}

template <typename Data> Data delete_node(struct list_node<Data> ** old_node, Data default_val) {
	if (!(*old_node))return default_val;
	struct list_head<Data> * head = (*old_node)->head;
	Data old_data = (*old_node)->data;
	if (head) {
		if (head->count) {
			if (head->count==1) {
				(*old_node)->head = nullptr;
				delete (*old_node);
				head->first = nullptr;
				head->tail = nullptr;
			} else if (head->count == 2) {
				if ((*old_node) == head->first) {
					auto last_tail = head->tail;
					(*old_node)->next = nullptr;
					(*old_node)->head = nullptr;
					delete (*old_node);
					head->first = last_tail;
					head->first->last = nullptr;
				} else if ((*old_node) == head->tail) {
					auto last_first = head->first;
					(*old_node)->last = nullptr;
					(*old_node)->head = nullptr;
					delete (*old_node);
					head->tail = last_first;
					head->tail->next = nullptr;
				} else {
					//error
					delete (*old_node);
					*old_node = nullptr;
					return old_data;
				}
			} else if (head->count > 2) {

				if ((*old_node) == head->first) {
					auto new_head = (*old_node)->next;
					(*old_node)->next = nullptr;
					(*old_node)->head = nullptr;
					delete (*old_node);
					head->first = new_head;
					head->first->last = nullptr;
				}
				else if ((*old_node) == head->tail) {
					auto new_tail = (*old_node)->last;
					(*old_node)->last = nullptr;
					(*old_node)->head = nullptr;
					delete (*old_node);
					head->tail = new_tail;
					head->tail->next = nullptr;
				}
				else {
					(*old_node)->last->next = (*old_node)->next;
					(*old_node)->next->last = (*old_node)->last;
					(*old_node)->next = nullptr;
					(*old_node)->last = nullptr;
					(*old_node)->head = nullptr;
					delete (*old_node);
				}
			}
		}
	}
	*old_node = nullptr;
	head->count--;
	return old_data;
}


#endif // !LINKED_LIST_H
