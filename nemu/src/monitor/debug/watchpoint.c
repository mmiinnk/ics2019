#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include <malloc.h>

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;
static unsigned wp_NO = 1;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
    wp_pool[i].expression[0] = '\0';
    wp_pool[i].old_value = 0;
    wp_pool[i].hit_times = 0;
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(){
	//no node in free_
	if (free_ == NULL){
		printf("No free Watch Point structure!\n");
		assert(0);
	}
	
	//take a node at the end of free_
	WP* new_node = free_;
	
	while(new_node->next != NULL){
		new_node = new_node->next;
	}

	if (new_node == free_)
		free_ = NULL;
	else{
		WP *p = free_;
		while(p->next->next != NULL)
			p = p->next;
		p->next = NULL;
	}

	//add the new node to the end of head
	if (head == NULL){
		head = new_node;
	}
	else {
		WP *p = head;
		for (; p->next != NULL; p = p->next);
		p->next = new_node;
	}
	new_node->NO = wp_NO;
	wp_NO++;
	return new_node;
}

void free_wp(WP *wp){
	
	wp->next = NULL;
	
	//add into free
	if (free_ == NULL)
		free_ = wp;
	else {
		WP* last_node = free_;
		while (last_node->next != NULL)
			last_node = last_node->next;
		last_node->next = wp;
	}
}

bool check_watchpoint(){
	if (head == NULL)
		return false;
	WP *p = head;
	bool stop = false;
	while (p != NULL){
		bool *success = (bool*)malloc(1);
		*success = true;
		uint32_t new_value = expr(p->expression, success);
		if (!*success){
			printf("Evaluation failed!\n");
			assert(0);
		}
		if (p->old_value != new_value){
			stop = true;
			printf("\nWatchpoint %d: %s\n\n", p->NO,p->expression);
			printf("Old value = %u\n", p->old_value);
			printf("New value = %u\n\n", new_value);
			p->old_value = new_value;
			p->hit_times++;
		}
		p = p->next;
	}
	return stop;

}

void watchpoint_display(){
	if (head == NULL){
		printf("No watchpoints!\n");
		return;
	}
	WP *p = head;
	printf("%-10s%s\n", "Num", "What");
	for (;p != NULL; p = p->next){
		printf("%-10d%s\n",p->NO,p->expression);
		if (p->hit_times != 0){
			if (p->hit_times == 1)
				printf("%-10sbreakpoint already hit 1 time\n"," ");
			else
				printf("%-10sbreakpoint already hit %d times\n", " ", p->hit_times);
		}
	}
}

void delete_watchpoint(int n){
	if (n == -1){
		while (head != NULL){
			WP *p = head;
			head = head->next;
			free_wp(p);
		}
	}
	else{
		WP *p = head;
		while (p != NULL && p->NO != n)
			p = p->next;
		if (p == NULL)
			printf("No watchpoint of number %d\n!", n);
		else{
			//free from head
			if (p == head)
				head = p->next;
			else{
				WP *last_wp = head;
				while(last_wp->next != p)
					last_wp = last_wp->next;
				last_wp->next = p->next;
			}
			free_wp(p);
		}
	}
}
