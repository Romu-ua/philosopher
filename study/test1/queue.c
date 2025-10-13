#include "philo.h"

void	waiting_in_line(void *args)
{
	t_args	*a;
	t_node	*new_node;

	a = (t_args *)args;

	new_node = (t_node *)malloc(sizeof(t_node));
	new_node->data = a->tid;
	new_node->next = NULL;
	pthread_mutex_lock(&a->shared->is_waiting);
	if (!a->shared->q->front)
	{
		a->shared->q->front = new_node;
		a->shared->q->rear = new_node;
	}
	else
	{
		a->shared->q->rear->next = new_node;
		a->shared->q->rear = new_node;
	}
	pthread_mutex_unlock(&a->shared->is_waiting);
}

void	get_out_of_line(void *args)
{
	t_args	*a;
	t_node	*tmp;

	a = (t_args *)args;
	if (is_empty(a->shared->q))
	{
		printf("queue is empty.\n");
		return ;
	}
	tmp = a->shared->q->front;
	a->shared->q->front = tmp->next;
	if (a->shared->q->front == NULL)
		a->shared->q->rear = NULL;
	free(tmp);
}
