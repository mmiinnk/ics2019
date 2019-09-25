#include "monitor/watchpoint.h"
#include "monitor/expr.h"

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
    wp_pool[i].last_value = 0;
    wp_pool[i].hit_times = 0;
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(){
	if (free_ == NULL){
		printf("No free Watch Point structure!\n");
		assert(0);
	}
	WP* new_node = free_;
	unsigned index = 0;
	while(new_node->next != NULL){
		new_node = new_node->next;
		index++;
	}
	if (new_node == free_)
		free_ = NULL;
	else{
		wp_pool[index-1].next = NULL;
	}
	new_node->NO = wp_NO;
	wp_NO++;
	return new_node;
}

void free_wp(WP *wp){
	WP* last_node = NULL;
	if (free_ == NULL)
		last_node = free_;
	else{
		while (last_node->next != NULL)
			last_node = last_node->next;
	}
	wp->next = NULL;
	last_node->next = wp;
}
